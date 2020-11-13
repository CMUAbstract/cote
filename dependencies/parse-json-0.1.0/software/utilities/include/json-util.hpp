// json-util.hpp
// json-util header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <map>       // map
#include <string>    // string
#include <utility>   // pair
#include <vector>    // vector

// json library
#include <Value.hpp> // Value

namespace json { namespace util {
  std::string readFile(const std::string& filePathStr);
  bool writeFile(
   const std::string& filePathStr, const std::string& fileContentStr
  );
  std::string parseString(std::string& malleableString);
  std::string parseCharacter(std::string& malleableString);
  std::string parseEscape(std::string& malleableString);
  std::string parseHex(std::string& malleableString);
  double parseNumber(std::string& malleableString);
  std::string parseInteger(std::string& malleableString);
  std::string parseFraction(std::string& malleableString);
  std::string parseExponent(std::string& malleableString);
  std::vector<json::Value*> parseArray(std::string& malleableString);
  void parseWhitespace(std::string& malleableString);
  json::Value* parseValue(std::string& malleableString);
  std::map<std::string,Value*> parseObject(std::string& malleableString);
  std::pair<std::string,Value*> parseMember(std::string& malleableString);
  json::Value* parseJson(std::string& malleableString);
}}
