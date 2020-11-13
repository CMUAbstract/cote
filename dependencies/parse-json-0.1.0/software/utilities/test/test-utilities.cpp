// test-utilities.cpp
// Utiltiy function tests
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>     // max
#include <cstdlib>       // exit, EXIT_SUCCESS
#include <filesystem>    // path, directory_iterator
#include <iostream>      // cout
#include <ostream>       // endl
#include <string>        // string

// json library
#include <utilities.hpp> // readFile, writeFile

int main(int argc, char** argv) {
  // Variables
  std::filesystem::path odir;
  // Declare test objects
  std::string testParseString = "";
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
        std::max(0,static_cast<int>(pathStr.size())-21),21
       )=="test-parse-string.txt"
      ) {
        testParseString = json::util::readFile(pathStr);
      } else if(
       pathStr.substr(
        std::max(0,static_cast<int>(pathStr.size())-21),21
       )=="test-something-else.txt"
      ) {
        // reserved for future tests
      }
      it++;
    }
    // Sanity check
    if(
     testParseString==""
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

  //// parseString
  std::cout << "Test: parseString..."
            << std::endl;
  std::string malleableTestParseString(testParseString);
  std::string str = json::util::parseString(malleableTestParseString);
  pass = (str=="ABC/123/abc\\\"\\\\\\/\\b\\f\\n\\r\\t\\uB4fa");
  std::cout << "  " << (pass ? "PASS" : "FAIL")
            << std::endl;

  //// parseNumber
  std::cout << "Test: parseNumber..."
            << std::endl;
  std::vector<std::string> numbers = {
   "0", "0E0", "0E10", "0E+0", "0E+10", "0E-0", "0E-10", "0e0", "0e10", "0e+0",
   "0e+10", "0e-0", "0e-10", "0.0", "0.0E0", "0.0E10", "0.0E+0", "0.0E+10",
   "0.0E-0", "0.0E-10", "0.0e0", "0.0e10", "0.0e+0", "0.0e+10", "0.0e-0",
   "0.0e-10", "0.10", "0.10E0", "0.10E10", "0.10E+0", "0.10E+10", "0.10E-0",
   "0.10E-10", "0.10e0", "0.10e10", "0.10e+0", "0.10e+10", "0.10e-0",
   "0.10e-10", "10", "10E0", "10E10", "10E+0", "10E+10", "10E-0", "10E-10",
   "10e0", "10e10", "10e+0", "10e+10", "10e-0", "10e-10", "10.0", "10.0E0",
   "10.0E10", "10.0E+0", "10.0E+10", "10.0E-0", "10.0E-10", "10.0e0", "10.0e10",
   "10.0e+0", "10.0e+10", "10.0e-0", "10.0e-10", "10.10", "10.10E0", "10.10E10",
   "10.10E+0", "10.10E+10", "10.10E-0", "10.10E-10", "10.10e0", "10.10e10",
   "10.10e+0", "10.10e+10", "10.10e-0", "10.10e-10", "-0", "-0E0", "-0E10",
   "-0E+0", "-0E+10", "-0E-0", "-0E-10", "-0e0", "-0e10", "-0e+0", "-0e+10",
   "-0e-0", "-0e-10", "-0.0", "-0.0E0", "-0.0E10", "-0.0E+0", "-0.0E+10",
   "-0.0E-0", "-0.0E-10", "-0.0e0", "-0.0e10", "-0.0e+0", "-0.0e+10", "-0.0e-0",
   "-0.0e-10", "-0.10", "-0.10E0", "-0.10E10", "-0.10E+0", "-0.10E+10",
   "-0.10E-0", "-0.10E-10", "-0.10e0", "-0.10e10", "-0.10e+0", "-0.10e+10",
   "-0.10e-0", "-0.10e-10", "-10", "-10E0", "-10E10", "-10E+0", "-10E+10",
   "-10E-0", "-10E-10", "-10e0", "-10e10", "-10e+0", "-10e+10", "-10e-0",
   "-10e-10", "-10.0", "-10.0E0", "-10.0E10", "-10.0E+0", "-10.0E+10",
   "-10.0E-0", "-10.0E-10", "-10.0e0", "-10.0e10", "-10.0e+0", "-10.0e+10",
   "-10.0e-0", "-10.0e-10", "-10.10", "-10.10E0", "-10.10E10", "-10.10E+0",
   "-10.10E+10", "-10.10E-0", "-10.10E-10", "-10.10e0", "-10.10e10",
   "-10.10e+0", "-10.10e+10", "-10.10e-0", "-10.10e-10"
  };
  std::vector<double> doubles = {
   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.1,
   1000000000.0, 0.1, 1000000000.0, 0.1, 1e-11, 0.1, 1000000000.0, 0.1,
   1000000000.0, 0.1, 1e-11, 10.0, 10.0, 100000000000.0, 10.0, 100000000000.0,
   10.0, 1e-09, 10.0, 100000000000.0, 10.0, 100000000000.0, 10.0, 1e-09, 10.0,
   10.0, 100000000000.0, 10.0, 100000000000.0, 10.0, 1e-09, 10.0,
   100000000000.0, 10.0, 100000000000.0, 10.0, 1e-09, 10.1, 10.1,
   101000000000.0, 10.1, 101000000000.0, 10.1, 1.01e-09, 10.1, 101000000000.0,
   10.1, 101000000000.0, 10.1, 1.01e-09, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0,
   -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0,
   -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.1, -0.1, -1000000000.0, -0.1,
   -1000000000.0, -0.1, -1e-11, -0.1, -1000000000.0, -0.1, -1000000000.0, -0.1,
   -1e-11, -10.0, -10.0, -100000000000.0, -10.0, -100000000000.0, -10.0, -1e-09,
   -10.0, -100000000000.0, -10.0, -100000000000.0, -10.0, -1e-09, -10.0, -10.0,
   -100000000000.0, -10.0, -100000000000.0, -10.0, -1e-09, -10.0,
   -100000000000.0, -10.0, -100000000000.0, -10.0, -1e-09, -10.1, -10.1,
   -101000000000.0, -10.1, -101000000000.0, -10.1, -1.01e-09, -10.1,
   -101000000000.0, -10.1, -101000000000.0, -10.1, -1.01e-09
  };
  bool parseNumberPass = true;
  for(std::size_t i=0; i<156; i++) {
    std::string malleableTestParseNumber(numbers.at(i));
    double dbl = json::util::parseNumber(malleableTestParseNumber);
    parseNumberPass = (parseNumberPass && (dbl==doubles.at(i)));
  }
  std::cout << "  " << (parseNumberPass ? "PASS" : "FAIL")
            << std::endl;

  // Write out files
  std::cout << "Writing out files..."
            << std::endl;
  pass = json::util::writeFile(
   odir.string()+"output-test-parse-string.txt",testParseString
  );
  std::cout << "  output-test-parse-string.txt: " << (pass ? "PASS" : "FAIL")
            << std::endl;
  // Clean up
  std::exit(EXIT_SUCCESS);
}
