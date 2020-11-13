// test-string.cpp
// String class test
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
#include <String.hpp>    // String
#include <Value.hpp>     // Value
#include <utilities.hpp> // readFile, writeFile

int main(int argc, char** argv) {
  // Variables
  std::filesystem::path odir;
  // Declare test objects
  json::Value*  input00 = nullptr;
  json::Value*  input01 = nullptr;
  json::String* input02 = nullptr;
  json::String* input03 = nullptr;
  json::Value*  input04 = nullptr;
  json::Value*  input05 = nullptr;
  json::String* input06 = nullptr;
  json::String* input07 = nullptr;
  json::Value*  input08 = nullptr;
  json::Value*  input09 = nullptr;
  json::String* input10 = nullptr;
  json::String* input11 = nullptr;
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
        input00 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input01.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input01 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input02.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input02 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input03.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input03 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input04.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input04 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input05.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input05 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input06.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input06 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input07.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input07 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input08.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input08 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input09.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input09 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input10.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input10 = new json::String(fileStr);
      } else if(pathStr.substr(pathStr.size()-12,12)=="input11.json") {
        std::string fileStr = json::util::readFile(pathStr);
        input11 = new json::String(fileStr);
      }
      it++;
    }
    // Sanity check
    if(
     input00==nullptr || input01==nullptr || input02==nullptr ||
     input03==nullptr || input04==nullptr || input05==nullptr ||
     input06==nullptr || input07==nullptr || input08==nullptr ||
     input09==nullptr || input10==nullptr || input11==nullptr
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
  pass = input04->isValid();
  std::cout << "  input04 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input05->isValid();
  std::cout << "  input05 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input06->isValid();
  std::cout << "  input06 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input07->isValid();
  std::cout << "  input07 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = !(input08->isValid());
  std::cout << "  input08 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = !(input09->isValid());
  std::cout << "  input09 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = !(input10->isValid());
  std::cout << "  input10 valid: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = !(input11->isValid());
  std::cout << "  input11 valid: " << (pass ? "PASS" : "FAIL")
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
  pass = ((*input04)==(*input05));
  std::cout << "  input04==input05 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input04)==(*input06));
  std::cout << "  input04==input06 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input06)==(*input04));
  std::cout << "  input06==input04 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input06)==(*input07));
  std::cout << "  input06==input07 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;

  //// operator!=
  std::cout << "Test: operator!=..."
            << std::endl;
  pass = ((*input00)!=(*input04));
  std::cout << "  input00!=input04 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input00)!=(*input06));
  std::cout << "  input00!=input06 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input02)!=(*input04));
  std::cout << "  input02!=input04 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = ((*input02)!=(*input04));
  std::cout << "  input02!=input04 TRUE: " << (pass ? "PASS" : "FAIL")
            << std::endl;

  //// getValue
  std::cout << "Test: getValue..."
            << std::endl;
  pass = dynamic_cast<json::String*>(input00)->getValue()=="Hello ";
  std::cout << "  input00 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = dynamic_cast<json::String*>(input01)->getValue()=="Hello ";
  std::cout << "  input01 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input02->getValue()=="Hello ";
  std::cout << "  input02 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input03->getValue()=="Hello ";
  std::cout << "  input03 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = dynamic_cast<json::String*>(input04)->getValue()=="World!";
  std::cout << "  input04 \"World!\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = dynamic_cast<json::String*>(input05)->getValue()=="World!";
  std::cout << "  input05 \"World!\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input06->getValue()=="World!";
  std::cout << "  input06 \"World!\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input07->getValue()=="World!";
  std::cout << "  input07 \"World!\": " << (pass ? "PASS" : "FAIL")
            << std::endl;

  //// toString
  std::cout << "Test: toString..."
            << std::endl;
  pass = input00->toString()=="\"Hello \"";
  std::cout << "  input00 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input01->toString()=="\"Hello \"";
  std::cout << "  input01 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input02->toString()=="\"Hello \"";
  std::cout << "  input02 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input03->toString()=="\"Hello \"";
  std::cout << "  input03 \"Hello \": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input04->toString()=="\"World!\"";
  std::cout << "  input04 \"World!\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input05->toString()=="\"World!\"";
  std::cout << "  input05 \"World!\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input06->toString()=="\"World!\"";
  std::cout << "  input06 \"World!\": " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = input07->toString()=="\"World!\"";
  std::cout << "  input07 \"World!\": " << (pass ? "PASS" : "FAIL")
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
  pass = json::util::writeFile(
   odir.string()+"output04.json",input04->toString()
  );
  std::cout << "  input04: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output05.json",input05->toString()
  );
  std::cout << "  input05: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output06.json",input06->toString()
  );
  std::cout << "  input06: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output07.json",input07->toString()
  );
  std::cout << "  input07: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // Clean up
  delete input00;
  delete input01;
  delete input02;
  delete input03;
  delete input04;
  delete input05;
  delete input06;
  delete input07;
  delete input08;
  delete input09;
  delete input10;
  delete input11;
  std::exit(EXIT_SUCCESS);
}
