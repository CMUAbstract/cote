// json-util.cpp
// json-util implementation file
//
// Written by Bradley Denby
// Other contributors: Mason Xiao
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>       // size_t
#include <fstream>       // ifstream
#include <map>           // map
#include <string>        // string
#include <utility>       // pair, make_pair
#include <vector>        // vector

// json library
#include <Array.hpp>     // Array
#include <Bool.hpp>      // Bool
#include <Nul.hpp>       // Nul
#include <Number.hpp>    // Number
#include <Object.hpp>    // Object
#include <String.hpp>    // String
#include <json-util.hpp> // utilities header file
#include <Value.hpp>     // Value

namespace json { namespace util {
  std::string readFile(const std::string& filePathStr) {
    // std::ios::binary ensures no lost chars
    // std::ios::ate seeks to the end of file
    std::ifstream fileHandle(filePathStr,std::ios::binary|std::ios::ate);
    // tellg() at this point indicates char count of file
    std::size_t charCount = fileHandle.tellg();
    // Set up file string equal to file length filled with \0 placeholder chars
    std::string fileStr(charCount,'\0');
    // Seek the file handle back to the beginning
    fileHandle.seekg(0);
    // Read file into string
    fileHandle.read(&fileStr[0],charCount);
    // Close the file handle
    fileHandle.close();
    // Return the file string
    return fileStr;
  }

  bool writeFile(
   const std::string& filePathStr, const std::string& fileContentStr
  ) {
    bool success = true;
    std::ofstream fileHandle(filePathStr,std::ios::out);
    if(fileHandle.rdstate() & std::ios::failbit) {
      success = false;
    } else {
      fileHandle << fileContentStr;
    }
    fileHandle.close();
    return success;
  }

  std::string parseString(std::string& malleableString) {
    std::string outstr = "";
    if(malleableString.substr(0,1)!="\"") {
      return outstr;
    } else {
      malleableString.erase(0,1);
    }
    bool endTokenFound = false;
    if(malleableString.substr(0,1)=="\"") {
      malleableString.erase(0,1);
      endTokenFound = true;
    }
    while(!endTokenFound && malleableString.size()>0) {
      if(malleableString.substr(0,1)=="\"") {
        malleableString.erase(0,1);
        endTokenFound = true;
      } else {
        outstr += parseCharacter(malleableString);
      }
    }
    return outstr;
  }

  std::string parseCharacter(std::string& malleableString) {
    if(malleableString.substr(0,1)=="\\") {
      return parseEscape(malleableString);
    } else {
      std::string outchar = malleableString.substr(0,1);
      malleableString.erase(0,1);
      return outchar;
    }
  }

  std::string parseEscape(std::string& malleableString) {
    std::string outchar = "";
    if(malleableString.substr(0,1)=="\\") {
      std::string escchar = malleableString.substr(1,1);
      if(
       escchar=="\"" || escchar=="\\" || escchar=="/" || escchar=="b" ||
       escchar=="f"  || escchar=="n"  || escchar=="r" || escchar=="t"
      ) {
        outchar = ("\\"+escchar);
        malleableString.erase(0,2);
      } else if(escchar=="u") {
        return parseHex(malleableString);
      }
    }
    return outchar;
  }

  std::string parseHex(std::string& malleableString) {
    std::string outchar = "";
    if(malleableString.substr(0,2)=="\\u") {
      std::string hexchars = "";
      if(malleableString.size()>=6) {
        for(std::size_t i=0; i<4; i++) {
          char ch = std::tolower(malleableString.substr(2+i,1)[0]);
          if(
           ch=='a' || ch=='b' || ch=='c' || ch=='d' || ch=='e' || ch=='f' ||
           std::isdigit(ch)
          ) {
            hexchars += malleableString.substr(2+i,1);
          }
        }
      }
      if(hexchars.size()==4) {
        outchar = ("\\u"+hexchars);
        malleableString.erase(0,6);
      }
    }
    return outchar;
  }

  double parseNumber(std::string& malleableString) {
    std::string numstr = "";
    // parse integer
    if(malleableString.size()>0) {
      char ch0 = malleableString.substr(0,1)[0];
      if(std::isdigit(ch0)) {
        numstr += parseInteger(malleableString);
      } else if(ch0=='-' && malleableString.size()>1) {
        char ch1 = malleableString.substr(1,1)[0];
        if(std::isdigit(ch1)) {
          numstr += parseInteger(malleableString);
        }
      }
    }
    // parse fraction
    if(numstr.size()>0 && malleableString.substr(0,1)==".") {
      numstr += parseFraction(malleableString);
    }
    // parse exponent
    if(
     numstr.size()>0 &&
     (malleableString.substr(0,1)=="E" || malleableString.substr(0,1)=="e")
    ) {
      numstr += parseExponent(malleableString);
    }
    return std::stod(numstr);
  }

  std::string parseInteger(std::string& malleableString) {
    // Initialize integer string
    std::string intstr = "";
    // Deal with the first one or two chars specially
    if(malleableString.size()>0) {
      char ch0 = malleableString.substr(0,1)[0];
      if(ch0=='0') {
        if(
         malleableString.size()>1 &&
         std::isdigit(malleableString.substr(1,1)[0])
        ) {
          return intstr; // error
        } else {
          intstr += malleableString.substr(0,1);
          malleableString.erase(0,1);
        }
      } else if(ch0=='-' && malleableString.size()>1) {
        char ch1 = malleableString.substr(1,1)[0];
        if(ch1=='0') {
          if(
           malleableString.size()>2 &&
           std::isdigit(malleableString.substr(2,1)[0])
          ) {
            return intstr; // error
          } else {
            intstr += malleableString.substr(0,2);
            malleableString.erase(0,2);
          }
        } else if(std::isdigit(ch1)) {
          intstr += malleableString.substr(0,2);
          malleableString.erase(0,2);
        } else {
          return intstr; // error
        }
      } else if(std::isdigit(ch0)) {
        intstr += malleableString.substr(0,1);
        malleableString.erase(0,1);
      } else {
        return intstr; // error
      }
    }
    // After first one or two chars, while loop until isdigit fails
    while(
     malleableString.size()>0 && std::isdigit(malleableString.substr(0,1)[0])
    ) {
      intstr += malleableString.substr(0,1);
      malleableString.erase(0,1);
    }
    // Return integer string
    return intstr;
  }

  std::string parseFraction(std::string& malleableString) {
    // Initialize fraction string
    std::string fracstr = "";
    // Deal with the first two chars specially
    if(
     malleableString.substr(0,1)=="." &&
     malleableString.size()>1 && std::isdigit(malleableString.substr(1,1)[0])
    ) {
      fracstr += malleableString.substr(0,2);
      malleableString.erase(0,2);
    } else {
      return fracstr;
    }
    // After first two chars, while loop until isdigit fails
    while(
     malleableString.size()>0 && std::isdigit(malleableString.substr(0,1)[0])
    ) {
      fracstr += malleableString.substr(0,1);
      malleableString.erase(0,1);
    }
    // Return fraction string
    return fracstr;
  }

  std::string parseExponent(std::string& malleableString) {
    // Initialize exponent string
    std::string expstr = "";
    // Deal with the first two or three chars specially
    if(malleableString.substr(0,1)=="E" || malleableString.substr(0,1)=="e") {
      // Either consume a sign and a digit or consume a digit
      if(
       (malleableString.substr(1,1)=="+" || malleableString.substr(1,1)=="-") &&
       malleableString.size()>2 && std::isdigit(malleableString.substr(2,1)[0])
      ) {
        expstr += malleableString.substr(0,3);
        malleableString.erase(0,3);
      } else if(
       malleableString.size()>1 && std::isdigit(malleableString.substr(1,1)[0])
      ) {
        expstr += malleableString.substr(0,2);
        malleableString.erase(0,2);
      } else {
        return expstr;
      }
    } else {
      return expstr;
    }
    // After first two or three chars, while loop until isdigit fails
    while(
     malleableString.size()>0 && std::isdigit(malleableString.substr(0,1)[0])
    ) {
      expstr += malleableString.substr(0,1);
      malleableString.erase(0,1);
    }
    // Return exponent string
    return expstr;
  }

  std::vector<json::Value*> parseArray(std::string& malleableString) {
    // Initialize out vector
    std::vector<json::Value*> outVector;
    // Parse start token
    if(malleableString.substr(0,1)!="[") {
      return outVector; // error
    } else {
      malleableString.erase(0,1);
    }
    // Establish end token flag and check for end token
    bool endTokenFound = false;
    parseWhitespace(malleableString);
    if(malleableString.substr(0,1)=="]") {
      malleableString.erase(0,1);
      endTokenFound = true;
    }
    // While no end token found and string remains, parse value
    while(!endTokenFound && malleableString.size()>0) {
      parseWhitespace(malleableString);
      if(malleableString.substr(0,1)=="]") {
        malleableString.erase(0,1);
        endTokenFound = true;
        break;
      } else {
        outVector.push_back(parseValue(malleableString));
      }
      parseWhitespace(malleableString);
      if(malleableString.substr(0,1)==",") {
        malleableString.erase(0,1);
      }
    }
    // Return out vector
    return outVector;
  }

  void parseWhitespace(std::string& malleableString) {
    while(
     malleableString.size()>0 &&
     (
      malleableString.substr(0,1)==" "  ||
      malleableString.substr(0,1)=="\n" ||
      malleableString.substr(0,1)=="\r" ||
      malleableString.substr(0,1)=="\t"
     )
    ) {
      malleableString.erase(0,1);
    }
    return;
  }

  json::Value* parseValue(std::string& malleableString) {
    json::Value* value = nullptr;
    // Array
    if(malleableString.substr(0,1)=="[") {
      value = new json::Array(malleableString);
    }
    // Bool
    else if(
     malleableString.substr(0,4)=="true" || malleableString.substr(0,5)=="false"
    ) {
      value = new json::Bool(malleableString);
    }
    // Nul
    else if(malleableString.substr(0,4)=="null") {
      value = new json::Nul(malleableString);
    }
    // Number
    else if(
     malleableString.size()>0 && (
      std::isdigit(malleableString.substr(0,1)[0]) ||
      malleableString.substr(0,1)[0]=='-'
     )
    ) {
      value = new json::Number(malleableString);
    }
    // Object
    else if(malleableString.substr(0,1)=="{") {
      value = new json::Object(malleableString);
    }
    // String
    else if(malleableString.substr(0,1)=="\"") {
      value = new json::String(malleableString);
    }
    return value;
  }

  std::map<std::string,Value*> parseObject(std::string& malleableString) {
    // Initialize out map
    std::map<std::string,Value*> outMap;
    // Parse start token
    if(malleableString.substr(0,1)!="{") {
      return outMap; // error
    } else {
      malleableString.erase(0,1);
    }
    // Establish end token flag and check for end token
    bool endTokenFound = false;
    parseWhitespace(malleableString);
    if(malleableString.substr(0,1)=="}") {
      malleableString.erase(0,1);
      endTokenFound = true;
    }
    // While no end token found and string remains, parse member
    while(!endTokenFound && malleableString.size()>0) {
      parseWhitespace(malleableString);
      if(malleableString.substr(0,1)=="}") {
        malleableString.erase(0,1);
        endTokenFound = true;
        break;
      } else {
        outMap.insert(parseMember(malleableString));
      }
      parseWhitespace(malleableString);
      if(malleableString.substr(0,1)==",") {
        malleableString.erase(0,1);
      }
    }
    // Return out map
    return outMap;
  }

  std::pair<std::string,Value*> parseMember(std::string& malleableString) {
    // Initialize variables for output pair
    std::string  key   = "";
    json::Value* value = nullptr;
    // Parse the key string
    parseWhitespace(malleableString);
    key = parseString(malleableString);
    parseWhitespace(malleableString);
    // Consume colon
    if(malleableString.substr(0,1)==":") {
      malleableString.erase(0,1);
    } else {
      return std::make_pair(key,value); // error
    }
    parseWhitespace(malleableString);
    // Parse value
    if(malleableString.substr(0,1)=="[") {
      value = new json::Array(malleableString);
    }
    // Bool
    else if(
     malleableString.substr(0,4)=="true" || malleableString.substr(0,5)=="false"
    ) {
      value = new json::Bool(malleableString);
    }
    // Nul
    else if(malleableString.substr(0,4)=="null") {
      value = new json::Nul(malleableString);
    }
    // Number
    else if(
     malleableString.size()>0 && (
      std::isdigit(malleableString.substr(0,1)[0]) ||
      malleableString.substr(0,1)[0]=='-'
     )
    ) {
      value = new json::Number(malleableString);
    }
    // Object
    else if(malleableString.substr(0,1)=="{") {
      value = new json::Object(malleableString);
    }
    // String
    else if(malleableString.substr(0,1)=="\"") {
      value = new json::String(malleableString);
    }
    return std::make_pair(key,value);
  }

  json::Value* parseJson(std::string& malleableString) {
    json::Value* value = nullptr;
    parseWhitespace(malleableString);
    value = parseValue(malleableString);
    parseWhitespace(malleableString);
    return value;
  }
}}
