#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <thread>

#include "zmq.hpp"
#include <boost/algorithm/string.hpp>
#include <google/protobuf/text_format.h>
#include <ros/console.h>

#include "lib/alarm/alarm.h"
#include "lib/mission_manager/mission_commands.pb.h"
#include "lib/phased_loop/phased_loop.h"
#include "lib/physics_structs/physics_structs.h"
#include "lib/proto_comms/proto_comms.h"
#include "src/controls/ground_controls/timeline/executor/executor.h"
#include "src/controls/ground_controls/timeline/timeline_grammar.pb.h"
#include "src/controls/messages.pb.h"

namespace src {
namespace controls {
namespace flight_loop {
namespace mission_state_machine {
namespace {
static constexpr double kAcceptanceRadius = 10.0; // meters
} // namespace

enum MissionState {
  TRANSLATE,
  UGV_RELEASE,
  GET_NEXT_CMD,
};

::std::string StateToString(MissionState state);

// States //////////////////////////////////////////////////////////////////////
class State {
 public:
  State() = default;
  virtual ~State() = default;

  virtual void Handle(::src::controls::Sensors &sensors,
                      ::src::controls::Goal &goal,
                      ::src::controls::Output &output) = 0;

  virtual void Reset() = 0;
};

class TranslateState : public State {
 public:
  TranslateState();
  ~TranslateState() = default;

  void Handle(::src::controls::Sensors &sensors, ::src::controls::Goal &goal,
              ::src::controls::Output &output) override;
  void Reset() override;
};

class UGVReleaseState : public State {
 public:
  UGVReleaseState();
  ~UGVReleaseState() = default;

  void Handle(::src::controls::Sensors &sensors, ::src::controls::Goal &goal,
              ::src::controls::Output &output) override;
  void Reset() override;
};

class GetNextCmdState : public State {
 public:
  GetNextCmdState();
  ~GetNextCmdState() = default;

  void Handle(::src::controls::Sensors &sensors, ::src::controls::Goal &goal,
              ::src::controls::Output &output) override;
  void Reset() override;
};

class UnknownState : public State {
 public:
  UnknownState();
  ~UnknownState() = default;

  void Handle(::src::controls::Sensors &sensors, ::src::controls::Goal &goal,
              ::src::controls::Output &output) override;
  void Reset() override;
};

// State machine router ////////////////////////////////////////////////////////
class MissionStateMachine {
 public:
  MissionStateMachine();
  ~MissionStateMachine();

  void StateTransition(::src::controls::Output &output);
  bool SafetyStateOverride(::src::controls::Goal &goal,
                           ::src::controls::Output &output);

  void Handle(::src::controls::Sensors &sensors, ::src::controls::Goal &goal,
              ::src::controls::Output &output);

  void LoadMission(
      ::src::controls::ground_controls::timeline::DroneProgram drone_program);

 private:
  State *GetStateHandler(MissionState state);

  MissionState state_;
  ::std::map<MissionState, State *> state_handlers_;
  UnknownState *unknown_state_;
};

} // namespace mission_state_machine
} // namespace flight_loop
} // namespace controls
} // namespace src
