#pragma once

#include <cerrno>
#include <csignal>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

namespace petal {

enum class key { up, down, left, right, enter, escape, other, eof };

namespace detail {

struct raw_mode_state {
  termios saved{};
  volatile std::sig_atomic_t active{0};
};

inline raw_mode_state g_raw_state;

inline void restore_terminal_and_reraise(int sig) {
  if (g_raw_state.active)
    tcsetattr(STDIN_FILENO, TCSANOW, &g_raw_state.saved);

  raise(sig);
}

} // namespace detail

class raw_mode {
public:
  raw_mode() {
    auto &st = detail::g_raw_state;
    if (st.active)
      return;

    if (!isatty(STDIN_FILENO) || tcgetattr(STDIN_FILENO, &st.saved) != 0)
      return;

    st.active = 1;
    _install_handlers();

    termios raw = st.saved;
    raw.c_lflag &= ~static_cast<tcflag_t>(ECHO | ICANON);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0) {
      _remove_handlers();
      st.active = 0;
      return;
    }

    _owner = true;
  }

  ~raw_mode() {
    if (!_owner)
      return;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &detail::g_raw_state.saved);
    _remove_handlers();
    detail::g_raw_state.active = 0;
  }

  raw_mode(const raw_mode &) = delete;
  raw_mode &operator=(const raw_mode &) = delete;

private:
  struct sigaction _old_int{};
  struct sigaction _old_term{};
  bool _owner{false};

  void _install_handlers() {
    struct sigaction sa{};
    sa.sa_handler = detail::restore_terminal_and_reraise;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = static_cast<int>(SA_RESETHAND);
    sigaction(SIGINT, &sa, &_old_int);
    sigaction(SIGTERM, &sa, &_old_term);
  }

  void _remove_handlers() {
    sigaction(SIGINT, &_old_int, nullptr);
    sigaction(SIGTERM, &_old_term, nullptr);
  }
};

static inline bool byte_ready(int ms) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  timeval tv{ms / 1000, (ms % 1000) * 1000};
  return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
}

namespace detail {

inline bool read_byte(char &out) { return read(STDIN_FILENO, &out, 1) == 1; }

constexpr key arrow_for(char final_byte) noexcept {
  switch (final_byte) {
  case 'A':
    return key::up;

  case 'B':
    return key::down;

  case 'C':
    return key::right;

  case 'D':
    return key::left;

  default:
    return key::other;
  }
}

inline key read_escape() {
  if (!byte_ready(20))
    return key::escape;

  char intro{};
  if (!read_byte(intro))
    return key::escape;

  if (intro != '[' && intro != 'O')
    return key::other;

  bool has_params = false;
  char c{};
  while (byte_ready(20) && read_byte(c)) {
    if (c >= '@' && c <= '~')
      return has_params ? key::other : arrow_for(c);

    has_params = true;
  }

  return key::other;
}

} // namespace detail

inline bool read_key(key &out) {
  if (!byte_ready(0))
    return false;

  char c{};
  const ssize_t n = read(STDIN_FILENO, &c, 1);
  if (n == 0 || (n < 0 && errno != EINTR && errno != EAGAIN)) {
    out = key::eof;
    return true;
  }

  if (n < 0)
    return false;

  if (c == 27) {
    out = detail::read_escape();
    return true;
  }

  out = (c == '\n' || c == '\r') ? key::enter : key::other;
  return true;
}

} // namespace petal
