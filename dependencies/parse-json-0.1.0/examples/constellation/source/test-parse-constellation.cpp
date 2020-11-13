// test-parse-constellation.cpp
// Parse constellation configuration file test
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>       // size_t
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
#include <Object.hpp>    // Object
#include <String.hpp>    // String
#include <utilities.hpp> // readFile, writeFile
#include <Value.hpp>     // Value

int main(int argc, char** argv) {
  // Variables
  std::filesystem::path odir;
  // Declare test objects
  json::Object* constellation = nullptr;
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
      if(
       pathStr.substr(
        std::max(0,static_cast<int>(pathStr.size())-29),29
       )=="constellation-0000000001.json"
      ) {
        std::string fileStr = json::util::readFile(pathStr);
        constellation = new json::Object(fileStr);
      }
      it++;
    }
    // Sanity check
    if(constellation==nullptr) {
      std::cout << "Expected input file missing. Test failed."
                << std::endl;
      std::exit(EXIT_FAILURE);
    }
    // Output directory
    odir = std::filesystem::path(argv[2]);
  }
  // Perform tests
  bool pass;
  std::map<std::string,json::Value*> objectValue = constellation->getValue();
  // id
  std::cout << "Object entry \"id\": "
            << dynamic_cast<json::String*>(objectValue["id"])->getValue()
            << std::endl;
  pass = (objectValue["id"]->toString()=="\"0000000001\"");
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // data-collection
  std::cout << "Object entry \"data-collection\": "
            << dynamic_cast<json::String*>(
                objectValue["data-collection"]
               )->getValue()
            << std::endl;
  pass = (objectValue["data-collection"]->toString()=="\"frame-spaced\"");
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // data-processing
  std::cout << "Object entry \"data-processing\": "
            << dynamic_cast<json::String*>(
                objectValue["data-processing"]
               )->getValue()
            << std::endl;
  pass = (objectValue["id"]->toString()=="\"frame-parallel\"");
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // satellite-ids
  std::vector<json::Value*> satIds =
   dynamic_cast<json::Array*>(objectValue["satellite-ids"])->getValue();
  std::cout << "Object entry \"satellite-ids\":"
            << std::endl
            << "["
            << std::endl;
  for(std::size_t i=0; i<satIds.size(); i++) {
    std::cout << "  "
              << dynamic_cast<json::String*>(satIds.at(i))->getValue();
    if(i+1<satIds.size()) {
      std::cout << ",";
    }
    std::cout << std::endl;
  }
  std::cout << "]"
            << std::endl;
  // Write out files
  std::cout << "Writing out files..."
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output-constellation-0000000001.json",
   constellation->toString()
  );
  std::cout << "  constellation: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // Clean up
  delete constellation;
  std::exit(EXIT_SUCCESS);
}
