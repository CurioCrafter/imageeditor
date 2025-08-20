#include "crash_handler.h"

#if defined(_WIN32)
#  define NOMINMAX
#  include <windows.h>
#  include <Dbghelp.h>
#  include <Shlwapi.h>
#  pragma comment(lib, "Dbghelp.lib")
#  pragma comment(lib, "Shlwapi.lib")
#endif

#include <string>

namespace platform {
namespace crash {

#if defined(_WIN32)
static LONG WINAPI writeDump(EXCEPTION_POINTERS* exceptionPointers) {
    char modulePath[MAX_PATH] = {0};
    if (GetModuleFileNameA(nullptr, modulePath, MAX_PATH) == 0) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // Build dump file path: <exe_dir>\\crash-<pid>-<tickcount>.dmp
    char directory[MAX_PATH] = {0};
    lstrcpynA(directory, modulePath, MAX_PATH);
    PathRemoveFileSpecA(directory);

    char dumpPath[MAX_PATH] = {0};
    DWORD pid = GetCurrentProcessId();
    DWORD ticks = GetTickCount();
    wsprintfA(dumpPath, "%s\\crash-%lu-%lu.dmp", directory, static_cast<unsigned long>(pid), static_cast<unsigned long>(ticks));

    HANDLE dumpFile = CreateFileA(dumpPath, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (dumpFile == INVALID_HANDLE_VALUE) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    MINIDUMP_EXCEPTION_INFORMATION dumpExceptionInfo;
    dumpExceptionInfo.ThreadId = GetCurrentThreadId();
    dumpExceptionInfo.ExceptionPointers = exceptionPointers;
    dumpExceptionInfo.ClientPointers = FALSE;

    MINIDUMP_TYPE dumpType = static_cast<MINIDUMP_TYPE>(
        MiniDumpWithIndirectlyReferencedMemory |
        MiniDumpScanMemory |
        MiniDumpWithThreadInfo |
        MiniDumpWithHandleData);

    BOOL ok = MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        dumpFile,
        dumpType,
        exceptionPointers ? &dumpExceptionInfo : nullptr,
        nullptr,
        nullptr);

    CloseHandle(dumpFile);

    // If dump writing succeeded, swallow the exception so the process exits.
    return ok ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}
#endif

void installCrashHandler(const char* /*applicationNameUtf8*/) {
#if defined(_WIN32)
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    SetUnhandledExceptionFilter(writeDump);
#else
    (void)0; // no-op
#endif
}

}  // namespace crash
}  // namespace platform


