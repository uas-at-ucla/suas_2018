#include "flight_loop.h"

namespace src {
namespace controls {
namespace loops {

FlightLoop::FlightLoop() :
    running_(false),
    phased_loop_(kFlightLoopFrequency),
    alarm_(kFlightLoopFrequency),
    last_loop_(0),
    did_alarm_(false),
    did_arm_(false),
    last_bomb_drop_(0),
    last_dslr_(0) {}

void FlightLoop::Run() {
  running_ = true;

  while (running_ && ::ros::ok()) {
    ::ros::spinOnce();

    // TODO: Fetch latest messages.
    ::src::controls::Sensors sensors = ros_to_proto_.GetSensors();

    // Set the current time in inputted sensors message.
    double current_time = ::lib::phased_loop::GetCurrentTime();
    sensors.set_time(current_time);

    ::src::controls::Goal goal; //= goal_uas_message.goal();

    // Run control loop iteration.
    ::src::controls::Output output =
        RunIteration(sensors, goal);

    ros_to_proto_.SendOutput(output);

    // TODO: Send out output message.
    ros_to_proto_.SendOutput(output);

    // Run loop at a set frequency.
    phased_loop_.sleep();
  }
}

::src::controls::Output
FlightLoop::RunIteration(::src::controls::Sensors sensors,
                         ::src::controls::Goal goal) {

  ::src::controls::Output output = GenerateDefaultOutput();

  state_machine_.Handle(sensors, goal, output);
  WriteActuators(sensors, goal, output);
  DumpProtobufMessages(sensors, goal, output);

  return output;
}

void FlightLoop::MonitorLoopFrequency(::src::controls::Sensors sensors) {
  // LOG_LINE("Flight Loop dt: " << ::std::setprecision(14)
  //                            << sensors.time() - last_loop_ - 0.01);

  if (sensors.time() - last_loop_ > 0.01 + 0.002) {
    //  LOG_LINE("Flight LOOP RUNNING SLOW: dt: "
    //           << std::setprecision(14) << sensors.time() - last_loop_ -
    //           0.01);
  }
  last_loop_ = sensors.time();
}

::src::controls::Output FlightLoop::GenerateDefaultOutput() {
  ::src::controls::Output output;

  // Set state to integer representation of the current state of the flight
  // loop.
  output.set_state(0);
  output.set_flight_time(0);
  output.set_current_command_index(0);

  output.set_velocity_x(0);
  output.set_velocity_y(0);
  output.set_velocity_z(0);
  output.set_yaw_setpoint(0);

  output.set_gimbal_angle(kDefaultGimbalAngle);
  output.set_bomb_drop(false);
  output.set_alarm(false);
  output.set_dslr(false);

  output.set_trigger_takeoff(0);
  output.set_trigger_hold(0);
  output.set_trigger_offboard(0);
  output.set_trigger_rtl(0);
  output.set_trigger_land(0);
  output.set_trigger_arm(0);
  output.set_trigger_disarm(0);

  return output;
}

void FlightLoop::WriteActuators(::src::controls::Sensors &sensors,
                                ::src::controls::Goal &goal,
                                ::src::controls::Output &output) {
  // Handle alarm.
  output.set_alarm(alarm_.ShouldAlarm());

  if (goal.trigger_alarm() + 0.05 > sensors.time()) {
    if (!did_alarm_) {
      did_alarm_ = true;
      alarm_.AddAlert({0.30, 0.30});
      //    LOG_LINE("Alarm was manually triggered");
    }
  } else {
    did_alarm_ = false;
  }

  if (sensors.armed() && !did_arm_) {
    // Send out a chirp if the Pixhawk just got armed.
    did_arm_ = true;
    alarm_.AddAlert({0.03, 0.25});
  }

  if (!sensors.armed()) {
    did_arm_ = false;
  }

  // Handle bomb drop.
  last_bomb_drop_ = ::std::max(last_bomb_drop_, goal.trigger_bomb_drop());

  output.set_bomb_drop(false);
  if (last_bomb_drop_ <= sensors.time() &&
      last_bomb_drop_ + 5.0 > sensors.time()) {
    output.set_bomb_drop(true);
  }

  // Handle dslr.
  output.set_dslr(false);
  last_dslr_ = ::std::max(last_dslr_, goal.trigger_dslr());
  if (last_dslr_ <= sensors.time() && last_dslr_ + 15.0 > sensors.time()) {
    output.set_dslr(true);
  }
}

void FlightLoop::DumpProtobufMessages(::src::controls::Sensors &sensors,
                                      ::src::controls::Goal &goal,
                                      ::src::controls::Output &output) {

  LogProtobufMessage("SENSORS", &sensors);
  LogProtobufMessage("GOAL", &goal);
  LogProtobufMessage("OUTPUT", &output);
}

void FlightLoop::LogProtobufMessage(::std::string name,
                                    ::google::protobuf::Message *message) {
  ::std::ostringstream output;

  ::std::string sensors_string;
  ::google::protobuf::TextFormat::PrintToString(*message, &sensors_string);

  ::std::vector<::std::string> sensors_split;
  ::boost::split(sensors_split, sensors_string,
                 [](char c) { return c == '\n'; });
  sensors_split.insert(sensors_split.begin(), "");
  sensors_split.pop_back();

  for (::std::string field : sensors_split) {
    output << name << "... " << field << "\n";
  }

  // LOG_LINE(output.str());
}

} // namespace loops
} // namespace controls
} // namespace src
