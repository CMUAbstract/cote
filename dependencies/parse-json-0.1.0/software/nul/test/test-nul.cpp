// test-nul.cpp
// Nul class test
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

// json library
#include <Nul.hpp>       // Nul
#include <utilities.hpp> // readFile, writeFile
#include <Value.hpp>     // Value

int main(int argc, char** argv) {
  // Variables
  std::filesystem::path odir;
  // Declare test objects
  json::Value* input00 = nullptr;
  json::Value* input01 = nullptr;
  json::Nul*   input02 = nullptr;
  json::Nul*   input03 = nullptr;
  json::Value* input04 = nullptr;
  json::Nul*   input05 = nullptr;
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
        input00 = new json::Nul(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input01.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input01 = new json::Nul(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input02.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input02 = new json::Nul(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input03.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input03 = new json::Nul(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input04.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input04 = new json::Nul(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input05.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input05 = new json::Nul(fileStr);
      }
      it++;
    }
    // Sanity check
    if(
     input00==nullptr || input01==nullptr || input02==nullptr ||
     input03==nullptr || input04==nullptr || input05==nullptr
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
  //// isValid
  std::cout << "Test: isValid..."
            << std::endl;
  pass = input00->isValid();
  std::cout << "  input00 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input01->isValid();
  std::cout << "  input01 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input02->isValid();
  std::cout << "  input02 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input03->isValid();
  std::cout << "  input03 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = !(input04->isValid());
  std::cout << "  input04 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = !(input05->isValid());
  std::cout << "  input05 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;

  //// operator==
  std::cout << "Test: operator==..."
            << std::endl;
  pass = ((*input00)==(*input01));
  std::cout << "  input00==input01 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input00)==(*input02));
  std::cout << "  input00==input02 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input02)==(*input00));
  std::cout << "  input02==input00 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input02)==(*input03));
  std::cout << "  input02==input03 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;

  //// getValue
  std::cout << "Test: getValue..."
            << std::endl;
  pass = dynamic_cast<json::Nul*>(input00)->getValue()==nullptr;
  std::cout << "  input00 null: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = dynamic_cast<json::Nul*>(input01)->getValue()==nullptr;
  std::cout << "  input01 null: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input02->getValue()==nullptr;
  std::cout << "  input02 null: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input03->getValue()==nullptr;
  std::cout << "  input03 null: " << (pass ? "PASS" : "FAIL")
            << std::endl;

  //// toString
  std::cout << "Test: toString..."
            << std::endl;
  pass = input00->toString()=="null";
  std::cout << "  input00 \"null\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input01->toString()=="null";
  std::cout << "  input01 \"null\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input02->toString()=="null";
  std::cout << "  input02 \"null\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input03->toString()=="null";
  std::cout << "  input03 \"null\": " << (pass ? "PASS" : "FAIL")
            << std::endl;

  // Write out files
  std::cout << "Writing out files..."
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output00.json",input00->toString()
  );
  std::cout << "  input00: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output01.json",input01->toString()
  );
  std::cout << "  input01: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output02.json",input02->toString()
  );
  std::cout << "  input02: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output03.json",input03->toString()
  );
  std::cout << "  input03: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // Clean up
  delete input00;
  delete input01;
  delete input02;
  delete input03;
  delete input04;
  delete input05;
  std::exit(EXIT_SUCCESS);
}
