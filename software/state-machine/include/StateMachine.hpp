// StateMachine.hpp
// StateMachine class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_STATE_MACHINE_HPP
#define COTE_STATE_MACHINE_HPP

// Standard library
#include <cstdint>             // uint32_t
#include <map>                 // map
#include <string>              // string
#include <tuple>               // tuple
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
    std::map<std::string,double> getConstantValues() const;
    std::map<std::string,double> getVariableValues() const;
    std::map<
     std::string,std::vector<std::tuple<std::string,std::string,double>>
    > getEnterStateValues() const;
    std::map<
     std::string,std::vector<std::tuple<std::string,std::string,double>>
    > getExitStateValues() const;
    std::map<
     std::string,std::map<
      std::string,std::vector<std::tuple<std::string,std::string,double>>
     >
    > getTransitionValues() const;
    std::map<std::string,std::vector<StateTransition>> getStateTransitions()
     const;
    std::string getCurrentState() const;
    uint32_t getID() const;
    Log* getLog() const;
    double getConstantValue(const std::string& constant) const;
    double getVariableValue(const std::string& variable) const;
    void setVariableValue(const std::string& variable, const double& value);
    void setCurrentState(const std::string& state);
    void updateState();
  private:
    // constantValues map
    //   Key: the string name of the constant
    //   Value: the value of the constant represented as a double
    std::map<std::string,double> constantValues;
    // variableValues map
    //   Key: the string name of the variable
    //   Value: the value of the variable represented as a double
    std::map<std::string,double> variableValues;
    // enterStateValues map
    //   Key: the string name of the state that is being entered
    //   Value: a vector of (string,string,double) tuples, where the first entry
    //          is the string variable name, the second entry is the string
    //          operator, and the third entry is the double value used with the
    //          operator
    std::map<
     std::string,std::vector<std::tuple<std::string,std::string,double>>
    > enterStateValues;
    // exitStateValues map
    //   Key: the string name of the state that is being exited
    //   Value: a vector of (string,string,double) tuples, where the first entry
    //          is the string variable name, the second entry is the string
    //          operator, and the third entry is the double value used with the
    //          operator
    std::map<
     std::string,std::vector<std::tuple<std::string,std::string,double>>
    > exitStateValues;
    // transitionValues map
    //   Key: the string name of the state that is being exited (srcState)
    //   Value: a map with the following structure:
    //     Key: the string name of the state that is being entered (dstState)
    //     Value: a vector of (string,string,double) tuples, where the first
    //            entry is the string variable name, the second entry is the
    //            string operator, and the third entry is the double value used
    //            with the operator
    std::map<
     std::string,
     std::map<
      std::string,std::vector<std::tuple<std::string,std::string,double>>
     >
    > transitionValues;
    // stateTransitions map
    //   Key: the string name of the state
    //   Value: a vector of StateTransition objects representing the possible
    //          transitions to new states from the key state
    std::map<std::string,std::vector<StateTransition>> stateTransitions;
    // currentState, id, and log
    std::string currentState; // a string that represents the current state
    uint32_t id;              // identification number
    Log* log;                 // singleton, should not be deleted
  };
}

#endif
