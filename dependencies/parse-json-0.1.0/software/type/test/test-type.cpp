// test-type.cpp
// Type enum class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdlib>  // exit, EXIT_SUCCESS

// json library
#include <iostream> // cout
#include <ostream>  // endl
#include <Type.hpp> // Type

int main(int argc, char** argv) {
  std::cout << "Testing all enum class values..."
            << std::endl;
  const json::Type TYPE_ARRAY  = json::Type::ARRAY;
  const json::Type TYPE_BOOL   = json::Type::BOOL;
  const json::Type TYPE_NUL    = json::Type::NUL;
  const json::Type TYPE_NUMBER = json::Type::NUMBER;
  const json::Type TYPE_OBJECT = json::Type::OBJECT;
  const json::Type TYPE_STRING = json::Type::STRING;
  std::cout << "  Test successful!"
            << std::endl;
  std::exit(EXIT_SUCCESS);
}
