#include <petal/petal.hpp>

#include <iostream>

namespace petal {

void hello(std::string_view name) { std::cout << "Hello, " << name << "!\n"; }

} // namespace petal
