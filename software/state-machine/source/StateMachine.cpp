// StateMachine.cpp
// StateMachine class implementation file
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
#include <cstdint>             // uint32_t
#include <fstream>             // ifstream
#include <map>                 // map
#include <string>              // string, getline, find, substr, npos, stod
#include <utility>             // move
#include <vector>              // vector

#include <iostream> // cout
#include <ostream>  // endl

// cote library
#include <Log.hpp>             // Log
#include <StateMachine.hpp>    // StateMachine
#include <StateTransition.hpp> // StateTransition

namespace cote {
  StateMachine::StateMachine(
   const std::string& configFile, const uint32_t& id, Log* const log
  ) : id(id), log(log) {
    // Set up to parse configuration file
    std::ifstream configHandle(configFile);
    std::string line = "";
    std::size_t delimiterIndex = 0;
    // Parse the first line, which should follow the pattern initial-state:STATE
    std::getline(configHandle,line);
    delimiterIndex = line.find(":",0);
    std::string initialStateLabel = line.substr(0,delimiterIndex);
    this->currentState = "";
    if(initialStateLabel=="initial-state") {
      this->currentState = line.substr(
       0+delimiterIndex+std::string(":").size(),std::string::npos
      );
    }
    // Parse the conditions and initial values
    while(
     std::getline(configHandle,line) &&
     line.substr(0,line.find(":",0))=="condition"
    ) {
      line =
       line.substr(line.find(":",0)+std::string(":").size(),std::string::npos);
      std::string conditionName = line.substr(0,line.find(":",0));
      double conditionInitialValue = std::stod(
       line.substr(line.find(":",0)+std::string(":").size(),std::string::npos)
      );
      this->conditionValues[conditionName] = conditionInitialValue;
    }
    // Parse the transitions
    if(line.substr(0,line.find(":",0))=="transition") {
      // Parse the source state
      std::size_t startIndex = line.find(":",0)+std::string(":").size();
      std::size_t endIndex = line.find("->",0);
      std::string srcStateString = line.substr(startIndex,endIndex-startIndex);
      if(this->stateTransitions.count(srcStateString)==0) {
        this->stateTransitions[srcStateString] = std::vector<StateTransition>();
      }
      this->stateTransitions.at(srcStateString).push_back(
       cote::StateTransition(line)
      );
      while(
       std::getline(configHandle,line) &&
       line.substr(0,line.find(":",0))=="transition"
      ) {
        startIndex = line.find(":",0)+std::string(":").size();
        endIndex = line.find("->",0);
        std::string srcStateString =
         line.substr(startIndex,endIndex-startIndex);
        if(this->stateTransitions.count(srcStateString)==0) {
          this->stateTransitions[srcStateString] =
           std::vector<StateTransition>();
        }
        this->stateTransitions.at(srcStateString).push_back(
         cote::StateTransition(line)
        );
      }
    }
    configHandle.close();
  }

  StateMachine::StateMachine(const StateMachine& stateMachine) :
   currentState(stateMachine.getCurrentState()),
   stateTransitions(stateMachine.getStateTransitions()),
   conditionValues(stateMachine.getConditionValues()), id(stateMachine.getID()),
   log(stateMachine.getLog()) {}

  StateMachine::StateMachine(StateMachine&& stateMachine) :
   currentState(stateMachine.currentState),
   stateTransitions(stateMachine.stateTransitions),
   conditionValues(stateMachine.conditionValues), id(stateMachine.id),
   log(stateMachine.log) {
    stateMachine.log = NULL;
  }

  StateMachine::~StateMachine() {
    this->log = NULL;
  }

  StateMachine& StateMachine::operator=(const StateMachine& stateMachine) {
    StateMachine temp(stateMachine);
    *this = std::move(temp);
    return *this;
  }

  StateMachine& StateMachine::operator=(StateMachine&& stateMachine) {
    this->currentState     = stateMachine.currentState;
    this->stateTransitions = stateMachine.stateTransitions;
    this->conditionValues  = stateMachine.conditionValues;
    this->id               = stateMachine.id;
    this->log              = stateMachine.log;
    stateMachine.log       = NULL;
    return *this;
  }

  StateMachine* StateMachine::clone() const {
    return new StateMachine(*this);
  }

  std::string StateMachine::getCurrentState() const {
    return this->currentState;
  }

  std::map<std::string,std::vector<StateTransition>>
   StateMachine::getStateTransitions() const {
    return this->stateTransitions;
  }

  std::map<std::string,double> StateMachine::getConditionValues() const {
    return this->conditionValues;
  }

  uint32_t StateMachine::getID() const {
    return this->id;
  }

  Log* StateMachine::getLog() const {
    return this->log;
  }

  void StateMachine::setCondition(
   const std::string& condition, const double& value
  ) {
    if(this->conditionValues.count(condition)!=0) {
      this->conditionValues.at(condition) = value;
    }
  }

  void StateMachine::updateState() {
    if(this->stateTransitions.count(this->currentState)!=0) {
      std::vector<StateTransition> transitions =
       this->stateTransitions.at(this->currentState);
      for(std::size_t i=0; i<transitions.size(); i++) {
        if(transitions.at(i).checkConditions(this->conditionValues)) {
          this->currentState = transitions.at(i).getDstState();
          break;
        }
      }
    }
  }
}
