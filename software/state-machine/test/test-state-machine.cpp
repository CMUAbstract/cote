// test-state-machine.cpp
// StateMachine class test file
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
#include <cstddef>          // size_t
#include <cstdlib>          // exit, EXIT_SUCCESS
#include <string>           // to_string

// satsim
#include <Log.hpp>          // Log
#include <LogLevel.hpp>     // LogLevel
#include <StateMachine.hpp> // StateMachine

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,"../logs");
  cote::StateMachine computerStateMachine(
   "../configuration/computer-state-machine.dat",1,&log
  );
  uint32_t time = 0;
  double nodeVoltage = 5.0;
  computerStateMachine.setCondition("node-voltage",nodeVoltage);
  double claimedTaskCount = 0.0;
  computerStateMachine.setCondition("claimed-task-count",claimedTaskCount);
  computerStateMachine.updateState();
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "node-voltage",
   std::to_string(nodeVoltage)
  );
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
   std::to_string(claimedTaskCount)
  );
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "computer-state",
   computerStateMachine.getCurrentState()
  );
  for(std::size_t i=0; i<2; i++) {
    for(std::size_t j=0; j<20; j++) {
      time += 1;
      nodeVoltage += 0.1;
      computerStateMachine.setCondition("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(nodeVoltage)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(claimedTaskCount)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
    }
    time += 1;
    claimedTaskCount += 1.0;
    computerStateMachine.setCondition("claimed-task-count",claimedTaskCount);
    computerStateMachine.updateState();
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "node-voltage",
     std::to_string(nodeVoltage)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
     std::to_string(claimedTaskCount)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "computer-state",
     computerStateMachine.getCurrentState()
    );
    for(std::size_t j=0; j<13; j++) {
      time += 1;
      nodeVoltage -= 0.1;
      computerStateMachine.setCondition("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(nodeVoltage)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(claimedTaskCount)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
    }
    for(std::size_t j=0; j<11; j++) {
      time += 1;
      nodeVoltage += 0.1;
      computerStateMachine.setCondition("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(nodeVoltage)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(claimedTaskCount)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
    }
    for(std::size_t j=0; j<3; j++) {
      time += 1;
      nodeVoltage -= 0.1;
      computerStateMachine.setCondition("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(nodeVoltage)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(claimedTaskCount)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
    }
    time += 1;
    claimedTaskCount -= 1.0;
    computerStateMachine.setCondition("claimed-task-count",claimedTaskCount);
    computerStateMachine.updateState();
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "node-voltage",
     std::to_string(nodeVoltage)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
     std::to_string(claimedTaskCount)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "computer-state",
     computerStateMachine.getCurrentState()
    );
    for(std::size_t j=0; j<5; j++) {
      time += 1;
      nodeVoltage += 0.1;
      computerStateMachine.setCondition("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(nodeVoltage)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(claimedTaskCount)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
    }
    time += 1;
    claimedTaskCount += 2.0;
    computerStateMachine.setCondition("claimed-task-count",claimedTaskCount);
    computerStateMachine.updateState();
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "node-voltage",
     std::to_string(nodeVoltage)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
     std::to_string(claimedTaskCount)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "computer-state",
     computerStateMachine.getCurrentState()
    );
    for(std::size_t j=0; j<2; j++) {
      time += 1;
      nodeVoltage -= 1.0;
      computerStateMachine.setCondition("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(nodeVoltage)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(claimedTaskCount)
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
    }
    time += 1;
    claimedTaskCount -= 2.0;
    computerStateMachine.setCondition("claimed-task-count",claimedTaskCount);
    computerStateMachine.updateState();
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "node-voltage",
     std::to_string(nodeVoltage)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
     std::to_string(claimedTaskCount)
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "computer-state",
     computerStateMachine.getCurrentState()
    );
  }
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
