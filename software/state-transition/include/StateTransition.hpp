// StateTransition.hpp
// StateTransition class header file
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

#ifndef COTE_STATE_TRANSITION_HPP
#define COTE_STATE_TRANSITION_HPP

// Standard library
#include <map>     // map
#include <string>  // string
#include <utility> // pair
#include <vector>  // vector

// cote
// None

namespace cote {
  class StateTransition {
  public:
    StateTransition(const std::string& description);
    StateTransition(const StateTransition& stateTransition);
    StateTransition(StateTransition&& stateTransition);
    virtual ~StateTransition();
    virtual StateTransition& operator=(const StateTransition& stateTransition);
    virtual StateTransition& operator=(StateTransition&& stateTransition);
    virtual StateTransition* clone() const;
    std::string getSrcState() const;
    std::string getDstState() const;
    std::map<std::string,std::vector<std::pair<std::string,double>>>
     getConditions() const;
    bool checkConditions(const std::map<std::string,double>& conditionValues)
     const;
  private:
    std::string srcState;
    std::string dstState;
    // conditions map
    //   Key: the string name of the condition
    //   Value: a vector of pairs; each pair starts with a string, which
    //          represents the comparison operator, and ends with a double,
    //          which is the value used with the comparison operator in order to
    //          determine whether the transition condition has been met
    std::map<std::string,std::vector<std::pair<std::string,double>>> conditions;
  };
}

#endif
