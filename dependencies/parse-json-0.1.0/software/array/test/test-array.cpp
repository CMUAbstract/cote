// test-array.cpp
// Array class test
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdlib>       // exit, EXIT_SUCCESS
#include <filesystem>    // path, directory_iterator
#include <iostream>      // cout
#include <ostream>       // endl
#include <vector>        // vector

// json library
#include <Array.hpp>     // Array
#include <Bool.hpp>      // Bool
#include <Nul.hpp>       // Nul
#include <Number.hpp>    // Number
#include <String.hpp>    // String
#include <utilities.hpp> // readFile, writeFile
#include <Value.hpp>     // Value

int main(int argc, char** argv) {
  // Variables
  std::filesystem::path odir;
  // Declare test objects
  json::Array* input00 = nullptr;
  // Initialize test objects
  if(argc!=3) {
    std::cout << "Usage: " << argv[0]
              << " /path/to/idir/"
              << " /path/to/odir/"
              << std::endl;
    std::exit(EXIT_SUCCESS);
  } else {
    std::filesystem::path idir(argv[1]);
    std::filesystem::directory_iterator it(idir);
    while(it!=std::filesystem::end(it)) {
      std::string pathStr = it->path().string();
      if(pathStr.substr(pathStr.size()-12,12)=="input00.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input00 = new json::Array(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input01.json") {
        // Reserved for future use
      }
      it++;
    }
    // Sanity check
    if(
     input00==nullptr
    ) {
      std::cout << "Expected input file missing. Test failed."
                << std::endl;
      std::exit(EXIT_FAILURE);
    }
    // Output directory
    odir = std::filesystem::path(argv[2]);
  }
  // Perform tests
  bool pass;
  std::vector<json::Value*> arrayValue = input00->getValue();
  std::cout << "Array entry 0 is true?"
            << std::endl;
  pass = (dynamic_cast<json::Bool*>(arrayValue.at(0))->getValue()==true);
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;
  std::cout << "Array entry 1 is null?"
            << std::endl;
  pass = (dynamic_cast<json::Nul*>(arrayValue.at(1))->getValue()==nullptr);
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;
  std::cout << "Array entry 2 is 0.59?"
            << std::endl;
  pass = (dynamic_cast<json::Number*>(arrayValue.at(2))->getValue()==0.59);
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;
  std::cout << "Array entry 3 is \"Hello world!\"?"
            << std::endl;
  pass = (
   dynamic_cast<json::String*>(arrayValue.at(3))->getValue()=="Hello world!"
  );
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // Write out files
  std::cout << "Writing out files..."
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output00.json",input00->toString()
  );
  std::cout << "  input00: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // Clean up
  delete input00;
  std::exit(EXIT_SUCCESS);
}
