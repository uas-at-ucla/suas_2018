#pragma once

namespace aos {
namespace testing {

// Registers an exit handler (using atexit(3)) which will call _exit(2).
// Intended to be called in a freshly fork(2)ed process where it will run before
// any other exit handlers that were already registered and prevent them from
// being run.
void PreventExit();

}  // namespace testing
}  // namespace aos

