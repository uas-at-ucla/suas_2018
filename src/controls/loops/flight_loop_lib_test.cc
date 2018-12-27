#include "src/controls/loops/flight_loop.h"

#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "gtest/gtest.h"

namespace src {
namespace controls {
namespace loops {
namespace testing {

class FlightLoopTest : public ::testing::Test {
 public:
  FlightLoopTest() {
    flight_loop_.SetVerbose(true);

    sensors_.set_time(0);
    sensors_.set_latitude(0);
    sensors_.set_longitude(0);
    sensors_.set_altitude(0);
    sensors_.set_relative_altitude(0);
    sensors_.set_heading(0);
    sensors_.set_velocity_x(0);
    sensors_.set_velocity_y(0);
    sensors_.set_velocity_z(0);
    sensors_.set_gps_ground_speed(0);
    sensors_.set_gps_satellite_count(0);
    sensors_.set_gps_eph(0);
    sensors_.set_gps_epv(0);
    sensors_.set_accelerometer_x(0);
    sensors_.set_accelerometer_y(0);
    sensors_.set_accelerometer_z(0);
    sensors_.set_gyro_x(0);
    sensors_.set_gyro_y(0);
    sensors_.set_gyro_z(0);
    sensors_.set_absolute_pressure(0);
    sensors_.set_relative_pressure(0);
    sensors_.set_pressure_altitude(0);
    sensors_.set_temperature(0);
    sensors_.set_battery_voltage(0);
    sensors_.set_battery_current(0);
    sensors_.set_armed(0);
    sensors_.set_autopilot_state(0);
    sensors_.set_last_gps(0);

    goal_.set_run_mission(false);
    goal_.set_trigger_failsafe(false);
    goal_.set_trigger_throttle_cut(false);

    goal_.set_trigger_takeoff(false);
    goal_.set_trigger_hold(false);
    goal_.set_trigger_offboard(false);
    goal_.set_trigger_rtl(false);
    goal_.set_trigger_land(false);
    goal_.set_trigger_arm(false);
    goal_.set_trigger_disarm(false);

    goal_.set_trigger_takeoff(false);
    goal_.set_trigger_hold(false);
    goal_.set_trigger_offboard(false);
    goal_.set_trigger_rtl(false);
    goal_.set_trigger_land(false);
    goal_.set_trigger_arm(false);
    goal_.set_trigger_disarm(false);
  }

  void StepLoop() {
    output_ = flight_loop_.RunIteration(sensors_, goal_);
    sensors_.set_time(sensors_.time() +
                      1.0 / ::src::controls::loops::kFlightLoopFrequency);
  }

  ::src::controls::Sensors &sensors() { return sensors_; }

  ::src::controls::Goal &goal() { return goal_; }

  ::src::controls::Output &output() { return output_; }

 private:
  FlightLoop flight_loop_;

  ::src::controls::Sensors sensors_;
  ::src::controls::Goal goal_;
  ::src::controls::Output output_;
};

TEST_F(FlightLoopTest, Initialization) {
  for (double start = sensors().time(); sensors().time() < start + 5;) {
    StepLoop();
    ASSERT_EQ(output().state(), ::src::controls::loops::State::STANDBY);
    ASSERT_EQ(output().velocity_x(), 0);
    ASSERT_EQ(output().velocity_y(), 0);
    ASSERT_EQ(output().velocity_z(), 0);

    ASSERT_FALSE(output().trigger_takeoff());
    ASSERT_FALSE(output().trigger_hold());
    ASSERT_FALSE(output().trigger_offboard());
    ASSERT_FALSE(output().trigger_rtl());
    ASSERT_FALSE(output().trigger_land());
    ASSERT_FALSE(output().trigger_arm());
    ASSERT_FALSE(output().trigger_disarm());
  }

  sensors().set_armed(true);

  for (double start = sensors().time(); sensors().time() < start + 5;) {
    StepLoop();

    ASSERT_EQ(output().state(), ::src::controls::loops::State::ARMED);
    ASSERT_FALSE(output().trigger_arm());
    ASSERT_FALSE(output().trigger_takeoff());
    ASSERT_FALSE(output().trigger_hold());
    ASSERT_FALSE(output().trigger_offboard());
    ASSERT_FALSE(output().trigger_rtl());
    ASSERT_FALSE(output().trigger_land());
    ASSERT_FALSE(output().trigger_disarm());
  }
}

} // namespace testing
} // namespace loops
} // namespace controls
} // namespace src

int main(int argc, char **argv) {
  // static struct option getopt_options[] = {{"verbose", no_argument, 0, 'v'},
  //                                          {0, 0, 0, 0}};

  // while (1) {
  //   int opt = getopt_long(argc, argv, "i:o:sc", getopt_options, NULL);
  //   if (opt == -1)
  //     break;

  //   switch (opt) {
  //     case 'v':
  //       ::src::controls::loops::testing::verbose = true;
  //       break;
  //     default:
  //       exit(1);
  //       break;
  //   }
  // }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
