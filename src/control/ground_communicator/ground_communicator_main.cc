#include "ground_communicator.h"

int main() {
  ::src::control::ground_communicator::MissionReceiver ground_communicator;
  ground_communicator.Run();
}
