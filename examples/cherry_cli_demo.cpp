#include <petal/petal.hpp>

int main(int argc, char **argv) {
  petal::hello(argc > 1 ? argv[1] : "Cherry");
  return 0;
}
