#pragma once

// Simple Windows crash handler that writes a minidump next to the executable.

namespace platform {
namespace crash {

// Installs a process-wide unhandled exception filter on Windows that writes
// a .dmp file into the same directory as the executable. On non-Windows
// platforms, this is a no-op.
void installCrashHandler(const char* applicationNameUtf8);

}  // namespace crash
}  // namespace platform


