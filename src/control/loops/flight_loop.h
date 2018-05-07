#ifndef SPINNY_CONTROL_LOOPS_FLIGHT_LOOP_H_
#define SPINNY_CONTROL_LOOPS_FLIGHT_LOOP_H_

#include <atomic>

#include "aos/common/time.h"
#include "aos/common/util/phased_loop.h"
#include "aos/linux_code/init.h"

#include "src/control/loops/flight_loop.q.h"
#include "src/control/loops/pilot/pilot.h"
#include "lib/physics_structs/physics_structs.h"
#include "lib/logger/log_sender.h"
#include "lib/alarm/alarm.h"

/*      ________  ________  ___  ________   ________       ___    ___
       |\   ____\|\   __  \|\  \|\   ___  \|\   ___  \    |\  \  /  /|
       \ \  \___|\ \  \|\  \ \  \ \  \\ \  \ \  \\ \  \   \ \  \/  / /
        \ \_____  \ \   ____\ \  \ \  \\ \  \ \  \\ \  \   \ \    / /
         \|____|\  \ \  \___|\ \  \ \  \\ \  \ \  \\ \  \   \/  /  /
           ____\_\  \ \__\    \ \__\ \__\\ \__\ \__\\ \__\__/  / /
          |\_________\|__|     \|__|\|__| \|__|\|__| \|__|\___/ /
          \|_________|           UAS @ UCLA 2018         \|___|/              */

namespace src {
namespace control {
namespace loops {

class FlightLoop {
 public:
  FlightLoop();

  void Run();
  void Iterate();

  // Flight loop state machine states.
  enum State {
    STANDBY,
    ARMING,
    ARMED,
    TAKING_OFF,
    IN_AIR,
    LANDING,
    FAILSAFE,
    FLIGHT_TERMINATION
  };

  State state() const { return state_; }

  // Method to dump all the current sensors at the head of the message queue.
  void DumpSensors();
  void DumpSensorsPeriodic();

  void SetVerbose(bool verbose);

 private:
  void RunIteration();

  State state_;

  pilot::Pilot pilot_;

  ::std::atomic<bool> running_;

  ::aos::time::PhasedLoop phased_loop_;
  ::std::chrono::time_point<std::chrono::system_clock> start_;

  int takeoff_ticker_;
  bool verbose_;
  int count_;

  void EndFlightTimer();
  int previous_flights_time_;
  unsigned long current_flight_start_time_;

  ::lib::alarm::Alarm alarm_;

  bool got_sensors_;
  double last_loop_;
  bool did_alarm_;
};

const std::map<FlightLoop::State, std::string> state_string = {
  {FlightLoop::STANDBY, "STANDBY"},
  {FlightLoop::ARMING, "ARMING"},
  {FlightLoop::ARMED, "ARMED"},
  {FlightLoop::TAKING_OFF, "TAKING_OFF"},
  {FlightLoop::IN_AIR, "IN_AIR"},
  {FlightLoop::LANDING, "LANDING"},
  {FlightLoop::FAILSAFE, "FAILSAFE"},
  {FlightLoop::FLIGHT_TERMINATION, "FLIGHT_TERMINATION"}
};

}  // namespace loops
}  // namespace control
}  // namespace src

#endif  // SPINNY_CONTROL_LOOPS_FLIGHT_LOOP_H_
