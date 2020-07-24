// StateMachine.cpp
// StateMachine class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>           // min
#include <cstddef>             // size_t
#include <cstdint>             // uint32_t
#include <fstream>             // ifstream
#include <map>                 // map
#include <string>              // string, getline, find, substr, npos, stod
#include <utility>             // move
#include <vector>              // vector

// cote library
#include <Log.hpp>             // Log
#include <StateMachine.hpp>    // StateMachine
#include <StateTransition.hpp> // StateTransition

namespace cote {
  StateMachine::StateMachine(
   const std::string& configFile, const uint32_t& id, Log* const log
  ) : currentState(""), id(id), log(log) {
    // Parse configuration file
    std::ifstream configHandle(configFile);
    std::string line = "";
    while(std::getline(configHandle,line)) {
      std::size_t labelEndIndex = line.find(":",0);
      std::string label = line.substr(0,labelEndIndex);
      line =
       line.substr(labelEndIndex+std::string(":").size(),std::string::npos);
      if(label=="constant") {
        std::size_t delimiterIndex = line.find(":",0);
        std::string constantName = line.substr(0,delimiterIndex);
        double constantValue = std::stod(
         line.substr(delimiterIndex+std::string(":").size(),std::string::npos)
        );
        this->constantValues[constantName] = constantValue;
      } else if(label=="variable") {
        std::size_t delimiterIndex = line.find(":",0);
        std::string variableName = line.substr(0,delimiterIndex);
        double variableValue = std::stod(
         line.substr(delimiterIndex+std::string(":").size(),std::string::npos)
        );
        this->variableValues[variableName] = variableValue;
      } else if(label=="enter-state") {
        std::size_t delimiterIndex = line.find(":",0);
        std::string stateName = line.substr(0,delimiterIndex);
        line =
         line.substr(delimiterIndex+std::string(":").size(),std::string::npos);
        std::size_t lineIndex = 0;
        while(lineIndex<line.size()) {
          delimiterIndex = line.find(",",lineIndex);
          std::string enterChange =
           line.substr(lineIndex,delimiterIndex-lineIndex);
          std::size_t operatorIndex = 0;
          std::string variableName = "";
          std::string assignmentOperator = "";
          double variableValue = 0.0;
          if(enterChange.find("+=",0)!=std::string::npos) {
            operatorIndex = enterChange.find("+=",0);
            assignmentOperator = "+=";
          } else if(enterChange.find("-=",0)!=std::string::npos) {
            operatorIndex = enterChange.find("-=",0);
            assignmentOperator = "-=";
          } else if(enterChange.find("*=",0)!=std::string::npos) {
            operatorIndex = enterChange.find("*=",0);
            assignmentOperator = "*=";
          } else if(enterChange.find("=",0)!=std::string::npos) {
            operatorIndex = enterChange.find("=",0);
            assignmentOperator = "=";
          }
          variableName = enterChange.substr(0,operatorIndex);
          variableValue = std::stod(enterChange.substr(
           operatorIndex+assignmentOperator.size(),std::string::npos
          ));
          if(this->enterStateValues.count(stateName)==0) {
            this->enterStateValues[stateName] =
             std::vector<std::tuple<std::string,std::string,double>>();
          }
          this->enterStateValues.at(stateName).push_back(
           std::make_tuple(variableName,assignmentOperator,variableValue)
          );
          lineIndex = (
           delimiterIndex==std::string::npos ?
           delimiterIndex : delimiterIndex+std::string(",").size()
          );
        }
      } else if(label=="exit-state") {
        std::size_t delimiterIndex = line.find(":",0);
        std::string stateName = line.substr(0,delimiterIndex);
        line =
         line.substr(delimiterIndex+std::string(":").size(),std::string::npos);
        std::size_t lineIndex = 0;
        while(lineIndex<line.size()) {
          delimiterIndex = line.find(",",lineIndex);
          std::string exitChange =
           line.substr(lineIndex,delimiterIndex-lineIndex);
          std::size_t operatorIndex = 0;
          std::string variableName = "";
          std::string assignmentOperator = "";
          double variableValue = 0.0;
          if(exitChange.find("+=",0)!=std::string::npos) {
            operatorIndex = exitChange.find("+=",0);
            assignmentOperator = "+=";
          } else if(exitChange.find("-=",0)!=std::string::npos) {
            operatorIndex = exitChange.find("-=",0);
            assignmentOperator = "-=";
          } else if(exitChange.find("*=",0)!=std::string::npos) {
            operatorIndex = exitChange.find("*=",0);
            assignmentOperator = "*=";
          } else if(exitChange.find("=",0)!=std::string::npos) {
            operatorIndex = exitChange.find("=",0);
            assignmentOperator = "=";
          }
          variableName = exitChange.substr(0,operatorIndex);
          variableValue = std::stod(exitChange.substr(
           operatorIndex+assignmentOperator.size(),std::string::npos
          ));
          if(this->exitStateValues.count(stateName)==0) {
            this->exitStateValues[stateName] =
             std::vector<std::tuple<std::string,std::string,double>>();
          }
          this->exitStateValues.at(stateName).push_back(
           std::make_tuple(variableName,assignmentOperator,variableValue)
          );
          lineIndex = (
           delimiterIndex==std::string::npos ?
           delimiterIndex : delimiterIndex+std::string(",").size()
          );
        }
      } else if(label=="transition") {
        // Find the delimiter between the transition conditions and the
        // transition changes
        std::size_t transitionChangeIndex = line.find(":",0);
        transitionChangeIndex = (
         transitionChangeIndex==std::string::npos ?
         transitionChangeIndex : transitionChangeIndex+std::string(":").size()
        );
        transitionChangeIndex = line.find(":",transitionChangeIndex);
        // Record the transition conditions, source state, and destination state
        std::string transitionConditions =
         "transition:"+line.substr(0,transitionChangeIndex);
        std::size_t srcStartIndex =
         transitionConditions.find(":",0)+std::string(":").size();
        std::size_t srcEndIndex = transitionConditions.find("->",0);
        std::size_t dstStartIndex = srcEndIndex+std::string("->").size();
        std::size_t dstEndIndex = transitionConditions.find(":",dstStartIndex);
        std::string srcStateString =
         transitionConditions.substr(srcStartIndex,srcEndIndex-srcStartIndex);
        std::string dstStateString =
         transitionConditions.substr(dstStartIndex,dstEndIndex-dstStartIndex);
        if(this->stateTransitions.count(srcStateString)==0) {
          this->stateTransitions[srcStateString] =
           std::vector<StateTransition>();
        }
        this->stateTransitions.at(srcStateString).push_back(
         cote::StateTransition(transitionConditions)
        );
        // Record the transition changes
        transitionChangeIndex = (
         transitionChangeIndex==std::string::npos ?
         transitionChangeIndex : transitionChangeIndex+std::string(":").size()
        );
        std::string transitionChanges = line.substr(
         std::min(transitionChangeIndex,line.size()),std::string::npos
        );
        if(this->transitionValues.count(srcStateString)==0) {
          this->transitionValues[srcStateString] = std::map<
           std::string,std::vector<std::tuple<std::string,std::string,double>>
          >();
        }
        if(this->transitionValues.at(srcStateString).count(dstStateString)==0) {
          this->transitionValues.at(srcStateString)[dstStateString] =
           std::vector<std::tuple<std::string,std::string,double>>();
        }
        transitionChangeIndex = 0;
        while(transitionChangeIndex<transitionChanges.size()) {
          std::size_t delimterIndex =
           transitionChanges.find(",",transitionChangeIndex);
          std::string transitionChange = transitionChanges.substr(
           transitionChangeIndex, delimterIndex
          );
          std::size_t operatorIndex = 0;
          std::string variableName = "";
          std::string assignmentOperator = "";
          double variableValue = 0.0;
          if(transitionChange.find("+=",0)!=std::string::npos) {
            operatorIndex = transitionChange.find("+=",0);
            assignmentOperator = "+=";
          } else if(transitionChange.find("-=",0)!=std::string::npos) {
            operatorIndex = transitionChange.find("-=",0);
            assignmentOperator = "-=";
          } else if(transitionChange.find("*=",0)!=std::string::npos) {
            operatorIndex = transitionChange.find("*=",0);
            assignmentOperator = "*=";
          } else if(transitionChange.find("=",0)!=std::string::npos) {
            operatorIndex = transitionChange.find("=",0);
            assignmentOperator = "=";
          }
          variableName = transitionChange.substr(0,operatorIndex);
          variableValue = std::stod(transitionChange.substr(
           operatorIndex+assignmentOperator.size(),std::string::npos
          ));
          this->transitionValues.at(srcStateString).at(dstStateString).push_back
          (
           std::make_tuple(variableName,assignmentOperator,variableValue)
          );
          transitionChangeIndex = (
           delimterIndex==std::string::npos ?
           delimterIndex : delimterIndex+std::string(",").size()
          );
        }
      } else if(label=="initial-state") {
        this->currentState = line;
      }
    }
    configHandle.close();
  }

  StateMachine::StateMachine(const StateMachine& stateMachine) :
   constantValues(stateMachine.getConstantValues()),
   variableValues(stateMachine.getVariableValues()),
   enterStateValues(stateMachine.getEnterStateValues()),
   exitStateValues(stateMachine.getExitStateValues()),
   transitionValues(stateMachine.getTransitionValues()),
   stateTransitions(stateMachine.getStateTransitions()),
   currentState(stateMachine.getCurrentState()), id(stateMachine.getID()),
   log(stateMachine.getLog()) {}

  StateMachine::StateMachine(StateMachine&& stateMachine) :
   constantValues(stateMachine.constantValues),
   variableValues(stateMachine.variableValues),
   enterStateValues(stateMachine.enterStateValues),
   exitStateValues(stateMachine.exitStateValues),
   transitionValues(stateMachine.transitionValues),
   stateTransitions(stateMachine.stateTransitions),
   currentState(stateMachine.currentState), id(stateMachine.id),
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
    this->constantValues   = stateMachine.constantValues;
    this->variableValues   = stateMachine.variableValues;
    this->enterStateValues = stateMachine.enterStateValues;
    this->exitStateValues  = stateMachine.exitStateValues;
    this->transitionValues = stateMachine.transitionValues;
    this->stateTransitions = stateMachine.stateTransitions;
    this->currentState     = stateMachine.currentState;
    this->id               = stateMachine.id;
    this->log              = stateMachine.log;
    stateMachine.log       = NULL;
    return *this;
  }

  StateMachine* StateMachine::clone() const {
    return new StateMachine(*this);
  }

  std::map<std::string,double> StateMachine::getConstantValues() const {
    return this->constantValues;
  }

  std::map<std::string,double> StateMachine::getVariableValues() const {
    return this->variableValues;
  }

  std::map<
   std::string,std::vector<std::tuple<std::string,std::string,double>>
  > StateMachine::getEnterStateValues() const {
    return this->enterStateValues;
  }

  std::map<
   std::string,std::vector<std::tuple<std::string,std::string,double>>
  > StateMachine::getExitStateValues() const {
    return this->exitStateValues;
  }

  std::map<
   std::string,std::map<
    std::string,std::vector<std::tuple<std::string,std::string,double>>
   >
  > StateMachine::getTransitionValues() const {
    return this->transitionValues;
  }

  std::map<std::string,std::vector<StateTransition>>
   StateMachine::getStateTransitions() const {
    return this->stateTransitions;
  }

  std::string StateMachine::getCurrentState() const {
    return this->currentState;
  }

  uint32_t StateMachine::getID() const {
    return this->id;
  }

  Log* StateMachine::getLog() const {
    return this->log;
  }

  double StateMachine::getConstantValue(const std::string& constant) const {
    return this->constantValues.at(constant);
  }

  double StateMachine::getVariableValue(const std::string& variable) const {
    return this->variableValues.at(variable);
  }

  void StateMachine::setVariableValue(
   const std::string& variable, const double& value
  ) {
    this->variableValues.at(variable) = value;
  }

  void StateMachine::updateState() {
    if(this->stateTransitions.count(this->currentState)!=0) {
      std::vector<StateTransition> possibleTransitions =
       this->stateTransitions.at(this->currentState);
      for(std::size_t i=0; i<possibleTransitions.size(); i++) {
        if(possibleTransitions.at(i).checkConditions(this->variableValues)) {
          std::string previousState = this->currentState;
          this->currentState = possibleTransitions.at(i).getDstState();
          // Apply exit changes
          if(this->exitStateValues.count(previousState)!=0) {
            for(
             std::size_t j=0; j<this->exitStateValues.at(previousState).size();
             j++
            ) {
              std::tuple<std::string,std::string,double> exitChange =
               this->exitStateValues.at(previousState).at(j);
              std::string variableName = std::get<0>(exitChange);
              std::string assignmentOperator = std::get<1>(exitChange);
              double changeValue = std::get<2>(exitChange);
              double variableValue = this->getVariableValue(variableName);
              if(assignmentOperator=="+=") {
                this->setVariableValue(variableName, variableValue+changeValue);
              } else if(assignmentOperator=="-=") {
                this->setVariableValue(variableName, variableValue-changeValue);
              } else if(assignmentOperator=="*=") {
                this->setVariableValue(variableName, variableValue*changeValue);
              } else if(assignmentOperator=="=") {
                this->setVariableValue(variableName, changeValue);
              }
            }
          }
          // Apply transition changes
          if(this->transitionValues.count(previousState)!=0) {
            if(this->transitionValues.at(previousState).count(
             this->currentState
            )!=0) {
              for(
               std::size_t j=0;
               j<this->transitionValues.at(previousState).at(
                this->currentState
               ).size();
               j++
              ) {
                std::tuple<std::string,std::string,double> transitionChange =
                 this->transitionValues.at(previousState).at(
                  this->currentState
                 ).at(j);
                std::string variableName = std::get<0>(transitionChange);
                std::string assignmentOperator = std::get<1>(transitionChange);
                double changeValue = std::get<2>(transitionChange);
                double variableValue = this->getVariableValue(variableName);
                if(assignmentOperator=="+=") {
                  this->setVariableValue(
                   variableName, variableValue+changeValue
                  );
                } else if(assignmentOperator=="-=") {
                  this->setVariableValue(
                   variableName, variableValue-changeValue
                  );
                } else if(assignmentOperator=="*=") {
                  this->setVariableValue(
                   variableName, variableValue*changeValue
                  );
                } else if(assignmentOperator=="=") {
                  this->setVariableValue(variableName, changeValue);
                }
              }
            }
          }
          // Apply enter changes
          if(this->enterStateValues.count(this->currentState)!=0) {
            for(
             std::size_t j=0;
             j<this->enterStateValues.at(this->currentState).size();
             j++
            ) {
              std::tuple<std::string,std::string,double> enterChange =
               this->enterStateValues.at(this->currentState).at(j);
              std::string variableName = std::get<0>(enterChange);
              std::string assignmentOperator = std::get<1>(enterChange);
              double changeValue = std::get<2>(enterChange);
              double variableValue = this->getVariableValue(variableName);
              if(assignmentOperator=="+=") {
                this->setVariableValue(variableName, variableValue+changeValue);
              } else if(assignmentOperator=="-=") {
                this->setVariableValue(variableName, variableValue-changeValue);
              } else if(assignmentOperator=="*=") {
                this->setVariableValue(variableName, variableValue*changeValue);
              } else if(assignmentOperator=="=") {
                this->setVariableValue(variableName, changeValue);
              }
            }
          }
          break;
        }
      }
    }
  }

  void StateMachine::setCurrentState(const std::string& state) {
    std::string previousState = this->currentState;
    this->currentState = state;
    // Apply exit changes
    if(this->exitStateValues.count(previousState)!=0) {
      for(
       std::size_t j=0; j<this->exitStateValues.at(previousState).size();
       j++
      ) {
        std::tuple<std::string,std::string,double> exitChange =
         this->exitStateValues.at(previousState).at(j);
        std::string variableName = std::get<0>(exitChange);
        std::string assignmentOperator = std::get<1>(exitChange);
        double changeValue = std::get<2>(exitChange);
        double variableValue = this->getVariableValue(variableName);
        if(assignmentOperator=="+=") {
          this->setVariableValue(variableName, variableValue+changeValue);
        } else if(assignmentOperator=="-=") {
          this->setVariableValue(variableName, variableValue-changeValue);
        } else if(assignmentOperator=="*=") {
          this->setVariableValue(variableName, variableValue*changeValue);
        } else if(assignmentOperator=="=") {
          this->setVariableValue(variableName, changeValue);
        }
      }
    }
    // Apply enter changes
    if(this->enterStateValues.count(this->currentState)!=0) {
      for(
       std::size_t j=0;
       j<this->enterStateValues.at(this->currentState).size();
       j++
      ) {
        std::tuple<std::string,std::string,double> enterChange =
         this->enterStateValues.at(this->currentState).at(j);
        std::string variableName = std::get<0>(enterChange);
        std::string assignmentOperator = std::get<1>(enterChange);
        double changeValue = std::get<2>(enterChange);
        double variableValue = this->getVariableValue(variableName);
        if(assignmentOperator=="+=") {
          this->setVariableValue(variableName, variableValue+changeValue);
        } else if(assignmentOperator=="-=") {
          this->setVariableValue(variableName, variableValue-changeValue);
        } else if(assignmentOperator=="*=") {
          this->setVariableValue(variableName, variableValue*changeValue);
        } else if(assignmentOperator=="=") {
          this->setVariableValue(variableName, changeValue);
        }
      }
    }
  }
}
