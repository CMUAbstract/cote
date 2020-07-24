// test-state-machine.cpp
// StateMachine class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

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
  double time = 0.0;
  double nodeVoltage = 5.0;
  computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
  computerStateMachine.setVariableValue("claimed-task-count",0.0);
  computerStateMachine.updateState();
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "computer-state",
   computerStateMachine.getCurrentState()
  );
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "power-w",
   std::to_string(computerStateMachine.getVariableValue("power-w"))
  );
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "dummy",
   std::to_string(computerStateMachine.getVariableValue("dummy"))
  );
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "node-voltage",
   std::to_string(computerStateMachine.getVariableValue("node-voltage"))
  );
  log.meas(
   cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
   std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
  );
  for(std::size_t i=0; i<2; i++) {
    for(std::size_t j=0; j<20; j++) {
      time += 0.1;
      nodeVoltage += 0.1;
      computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "power-w",
       std::to_string(computerStateMachine.getVariableValue("power-w"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "dummy",
       std::to_string(computerStateMachine.getVariableValue("dummy"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(computerStateMachine.getVariableValue("node-voltage"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(
        computerStateMachine.getVariableValue("claimed-task-count")
       )
      );
    }
    time += 0.1;
    computerStateMachine.setVariableValue("claimed-task-count",7.0);
    computerStateMachine.updateState();
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "computer-state",
     computerStateMachine.getCurrentState()
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "power-w",
     std::to_string(computerStateMachine.getVariableValue("power-w"))
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "dummy",
     std::to_string(computerStateMachine.getVariableValue("dummy"))
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "node-voltage",
     std::to_string(computerStateMachine.getVariableValue("node-voltage"))
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
     std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
    );
    for(std::size_t j=0; j<13; j++) {
      if(computerStateMachine.getCurrentState()=="WORK") {
        double workTimeS = computerStateMachine.getVariableValue("work-time-s");
        workTimeS += 0.1;
        computerStateMachine.setVariableValue("work-time-s",workTimeS);
        while(
         workTimeS>=computerStateMachine.getConstantValue("task-duration-s") &&
         computerStateMachine.getVariableValue("claimed-task-count")>0
        ) {
          double claimedTaskCount =
           computerStateMachine.getVariableValue("claimed-task-count");
          claimedTaskCount -= 1.0;
          computerStateMachine.setVariableValue(
           "claimed-task-count",claimedTaskCount
          );
          workTimeS -= computerStateMachine.getConstantValue("task-duration-s");
          computerStateMachine.setVariableValue("work-time-s",workTimeS);
        }
      }
      time += 0.1;
      nodeVoltage -= 0.1;
      computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "power-w",
       std::to_string(computerStateMachine.getVariableValue("power-w"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "dummy",
       std::to_string(computerStateMachine.getVariableValue("dummy"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(computerStateMachine.getVariableValue("node-voltage"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
      );
    }
    for(std::size_t j=0; j<11; j++) {
      time += 0.1;
      nodeVoltage += 0.1;
      computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "power-w",
       std::to_string(computerStateMachine.getVariableValue("power-w"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "dummy",
       std::to_string(computerStateMachine.getVariableValue("dummy"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(computerStateMachine.getVariableValue("node-voltage"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
      );
    }
    for(std::size_t j=0; j<2; j++) {
      if(computerStateMachine.getCurrentState()=="WORK") {
        double workTimeS = computerStateMachine.getVariableValue("work-time-s");
        workTimeS += 0.1;
        computerStateMachine.setVariableValue("work-time-s",workTimeS);
        while(
         workTimeS>=computerStateMachine.getConstantValue("task-duration-s") &&
         computerStateMachine.getVariableValue("claimed-task-count")>0
        ) {
          double claimedTaskCount =
           computerStateMachine.getVariableValue("claimed-task-count");
          claimedTaskCount -= 1.0;
          computerStateMachine.setVariableValue(
           "claimed-task-count",claimedTaskCount
          );
          workTimeS -= computerStateMachine.getConstantValue("task-duration-s");
          computerStateMachine.setVariableValue("work-time-s",workTimeS);
        }
      }
      time += 0.1;
      nodeVoltage -= 0.1;
      computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "power-w",
       std::to_string(computerStateMachine.getVariableValue("power-w"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "dummy",
       std::to_string(computerStateMachine.getVariableValue("dummy"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(computerStateMachine.getVariableValue("node-voltage"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
      );
    }
    for(std::size_t j=0; j<4; j++) {
      time += 0.1;
      nodeVoltage += 0.1;
      computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "power-w",
       std::to_string(computerStateMachine.getVariableValue("power-w"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "dummy",
       std::to_string(computerStateMachine.getVariableValue("dummy"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(computerStateMachine.getVariableValue("node-voltage"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
      );
    }
    time += 0.1;
    computerStateMachine.setVariableValue("claimed-task-count",1.0);
    computerStateMachine.updateState();
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "computer-state",
     computerStateMachine.getCurrentState()
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "power-w",
     std::to_string(computerStateMachine.getVariableValue("power-w"))
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "dummy",
     std::to_string(computerStateMachine.getVariableValue("dummy"))
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "node-voltage",
     std::to_string(computerStateMachine.getVariableValue("node-voltage"))
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
     std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
    );
    for(std::size_t j=0; j<2; j++) {
      if(computerStateMachine.getCurrentState()=="WORK") {
        double workTimeS = computerStateMachine.getVariableValue("work-time-s");
        workTimeS += 0.1;
        computerStateMachine.setVariableValue("work-time-s",workTimeS);
        while(
         workTimeS>=computerStateMachine.getConstantValue("task-duration-s") &&
         computerStateMachine.getVariableValue("claimed-task-count")>0
        ) {
          double claimedTaskCount =
           computerStateMachine.getVariableValue("claimed-task-count");
          claimedTaskCount -= 1.0;
          computerStateMachine.setVariableValue(
           "claimed-task-count",claimedTaskCount
          );
          workTimeS -= computerStateMachine.getConstantValue("task-duration-s");
          computerStateMachine.setVariableValue("work-time-s",workTimeS);
        }
      }
      time += 0.1;
      nodeVoltage -= 1.0;
      computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
      computerStateMachine.updateState();
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "computer-state",
       computerStateMachine.getCurrentState()
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "power-w",
       std::to_string(computerStateMachine.getVariableValue("power-w"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "dummy",
       std::to_string(computerStateMachine.getVariableValue("dummy"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "node-voltage",
       std::to_string(computerStateMachine.getVariableValue("node-voltage"))
      );
      log.meas(
       cote::LogLevel::INFO, std::to_string(time), "claimed-task-count",
       std::to_string(computerStateMachine.getVariableValue("claimed-task-count"))
      );
    }
  }
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
