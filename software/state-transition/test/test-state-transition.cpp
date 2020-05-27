// test-state-transition.cpp
// StateTransition class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication with this
// software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

// Standard library
#include <cstddef>             // size_t
#include <cstdlib>             // exit, EXIT_SUCCESS
#include <map>                 // map
#include <string>              // string, to_string
#include <vector>              // vector

// satsim
#include <Log.hpp>             // Log
#include <LogLevel.hpp>        // LogLevel
#include <StateTransition.hpp> // StateTransition

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,"../test");
  std::string description =
   "transition:WORK->SLEEP:node-voltage>=5.5,node-voltage<5.75,claimed-task-count>=0";
  cote::StateTransition stateTransition(description);
  std::map<std::string,double> conditionValues = {
   {"node-voltage",10.6}, {"claimed-task-count",1.0}
  };
  for(size_t i=0; i<25; i++) {
    log.meas(
     cote::LogLevel::INFO,
     std::to_string(conditionValues.at("node-voltage")),
     "transition-check",
     std::to_string(stateTransition.checkConditions(conditionValues))
    );
    conditionValues.at("node-voltage") -= 0.2;
  }
  log.meas(
   cote::LogLevel::INFO,
   std::to_string(conditionValues.at("node-voltage")),
   "transition-check",
   std::to_string(stateTransition.checkConditions(conditionValues))
  );
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
