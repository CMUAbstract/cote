// StateMachine.hpp
// StateMachine class header file
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

#ifndef COTE_STATE_MACHINE_HPP
#define COTE_STATE_MACHINE_HPP

// Standard library
#include <cstdint>             // uint32_t
#include <map>                 // map
#include <string>              // string
#include <vector>              // vector

// cote library
#include <Log.hpp>             // Log
#include <StateTransition.hpp> // StateTransition

namespace cote {
  class StateMachine {
  public:
    StateMachine(
     const std::string& configFile, const uint32_t& id=0, Log* const log=NULL
    );
    StateMachine(const StateMachine& stateMachine);
    StateMachine(StateMachine&& stateMachine);
    virtual ~StateMachine();
    virtual StateMachine& operator=(const StateMachine& stateMachine);
    virtual StateMachine& operator=(StateMachine&& stateMachine);
    virtual StateMachine* clone() const;
    std::string getCurrentState() const;
    std::map<std::string,std::vector<StateTransition>> getStateTransitions()
     const;
    std::map<std::string,double> getConditionValues() const;
    uint32_t getID() const;
    Log* getLog() const;
    void setCondition(const std::string& condition, const double& value);
    void updateState();
  private:
    std::string currentState;
    // stateTransitions map
    //   Key: the string name of the state
    //   Value: a vector of StateTransition objects representing the possible
    //          transitions to new states from the key state
    std::map<std::string,std::vector<StateTransition>> stateTransitions;
    // conditionValues map
    //   Key: the string name of the condition
    //   Value: the current value of the condition represented as a double
    std::map<std::string,double> conditionValues;
    uint32_t id;             // identification number
    Log* log;                // singleton, should not be deleted
  };
}

#endif
