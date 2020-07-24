// StateTransition.cpp
// StateTransition class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>             // size_t
#include <map>                 // map
#include <string>              // string, find, npos, substr, stod
#include <utility>             // move, pair, make_pair
#include <vector>              // vector

// comsim
#include <StateTransition.hpp> // StateTransition

namespace cote {
  StateTransition::StateTransition(const std::string& description) {
    // skip past the "transition" label
    std::size_t delimiterIndex = description.find(":",0);
    std::size_t i = (
     delimiterIndex==std::string::npos ?
     delimiterIndex : delimiterIndex+std::string(":").size()
    );
    // parse and set the source state name
    delimiterIndex = description.find("->",i);
    if(delimiterIndex==std::string::npos) {
      this->srcState = "";
      i = delimiterIndex;
    } else {
      this->srcState = description.substr(i,delimiterIndex-i);
      i = delimiterIndex+std::string("->").size();
    }
    // parse and set the destination state name
    delimiterIndex = description.find(":",i);
    if(delimiterIndex==std::string::npos) {
      this->dstState = "";
      i = delimiterIndex;
    } else {
      this->dstState = description.substr(i,delimiterIndex-i);
      i = delimiterIndex+std::string(":").size();
    }
    // parse and set the transition conditions
    while(i<description.size()) {
      delimiterIndex = description.find(",",i);
      std::string transitionCondition = description.substr(i,delimiterIndex-i);
      std::size_t j = 0;
      std::string condition = "";
      std::string comparisonOperator = "";
      double value = 0.0;
      if(transitionCondition.find("==",0)!=std::string::npos) {
        j = transitionCondition.find("==",0);
        comparisonOperator = "==";
      } else if(transitionCondition.find("!=",0)!=std::string::npos) {
        j = transitionCondition.find("!=",0);
        comparisonOperator = "!=";
      } else if(transitionCondition.find("<=",0)!=std::string::npos) {
        j = transitionCondition.find("<=",0);
        comparisonOperator = "<=";
      } else if(transitionCondition.find(">=",0)!=std::string::npos) {
        j = transitionCondition.find(">=",0);
        comparisonOperator = ">=";
      } else if(transitionCondition.find("<",0)!=std::string::npos) {
        j = transitionCondition.find("<",0);
        comparisonOperator = "<";
      } else if(transitionCondition.find(">",0)!=std::string::npos) {
        j = transitionCondition.find(">",0);
        comparisonOperator = ">";
      }
      condition = transitionCondition.substr(0,j);
      value = std::stod(transitionCondition.substr(
       j+comparisonOperator.size(),std::string::npos
      ));
      if(this->conditions.count(condition)==0) {
        this->conditions[condition] =
         std::vector<std::pair<std::string,double>>();
      }
      this->conditions.at(condition).push_back(
       std::make_pair(comparisonOperator,value)
      );
      i = (
       delimiterIndex==std::string::npos ?
       delimiterIndex : delimiterIndex+std::string(",").size()
      );
    }
  }

  StateTransition::StateTransition(const StateTransition& stateTransition) :
   srcState(stateTransition.getSrcState()),
   dstState(stateTransition.getDstState()),
   conditions(stateTransition.getConditions()) {}

  StateTransition::StateTransition(StateTransition&& stateTransition) :
   srcState(stateTransition.srcState), dstState(stateTransition.dstState),
   conditions(stateTransition.conditions) {}

  StateTransition::~StateTransition() {}

  StateTransition& StateTransition::operator=(
   const StateTransition& stateTransition
  ) {
    StateTransition temp(stateTransition);
    *this = std::move(temp);
    return *this;
  }

  StateTransition& StateTransition::operator=(
   StateTransition&& stateTransition
  ) {
    this->srcState   = stateTransition.srcState;
    this->dstState   = stateTransition.dstState;
    this->conditions = stateTransition.conditions;
    return *this;
  }

  StateTransition* StateTransition::clone() const {
    return new StateTransition(*this);
  }

  std::string StateTransition::getSrcState() const {
    return this->srcState;
  }

  std::string StateTransition::getDstState() const {
    return this->dstState;
  }

  std::map<std::string,std::vector<std::pair<std::string,double>>>
   StateTransition::getConditions() const {
    return this->conditions;
  }

  bool StateTransition::checkConditions(
   const std::map<std::string,double>& conditionValues
  ) const {
    bool conditionsMet = true;
    for(
     std::map<
      std::string,std::vector<std::pair<std::string,double>>
     >::const_iterator it=this->conditions.begin();
     it!=this->conditions.end(); it++
    ) {
      std::string condition = it->first;
      if(conditionValues.count(condition)==0) {
        conditionsMet = false;
      } else {
        std::vector<std::pair<std::string,double>> tests = it->second;
        for(std::size_t i=0; i<tests.size(); i++) {
          if(tests.at(i).first=="==") {
            conditionsMet = conditionsMet &&
             (conditionValues.at(condition)==tests.at(i).second);
          } else if(tests.at(i).first=="!=") {
            conditionsMet = conditionsMet &&
             (conditionValues.at(condition)!=tests.at(i).second);
          } else if(tests.at(i).first=="<=") {
            conditionsMet = conditionsMet &&
             (conditionValues.at(condition)<=tests.at(i).second);
          } else if(tests.at(i).first==">=") {
            conditionsMet = conditionsMet &&
             (conditionValues.at(condition)>=tests.at(i).second);
          } else if(tests.at(i).first=="<") {
            conditionsMet = conditionsMet &&
             (conditionValues.at(condition)<tests.at(i).second);
          } else if(tests.at(i).first==">") {
            conditionsMet = conditionsMet &&
             (conditionValues.at(condition)>tests.at(i).second);
          }
          if(!conditionsMet) {
            break;
          }
        }
      }
      if(!conditionsMet) {
        break;
      }
    }
    return conditionsMet;
  }
}
