/* config.h.  Manual config for MSVC.  */

#ifndef _MSC_VER
#warn "msvc/config.h shouldn't be included for your development environment."
#error "Please make sure the msvc/ directory is removed from your build path."
#endif

#if defined(_PREFAST_)
/* Disable "Banned API Usage:" errors when using WDK's OACR/Prefast */
#pragma warning(disable:28719)
#endif
#if defined(_MSC_VER)
// Disable some VS2012 Code Analysis warnings
#pragma warning(disable:6258)		// We'll use TerminateThread() regardless
#pragma warning(disable:6387)
#endif

/* CoInstaller subdirectory for WinUSB redist files ("winusb" or "wdf") */
#define COINSTALLER_DIR "wdf"

/* 64bit subdirectory for WinUSB redist files ("x64" or "amd64") */
#define X64_DIR "amd64"

/* 32 bit support */
#define OPT_M32

/* 64 bit support */
#define OPT_M64

/* Debug message logging */
//#define ENABLE_DEBUG_LOGGING

/* Debug message logging (toggable) */
#define INCLUDE_DEBUG_LOGGING

/* Message logging */
#define ENABLE_LOGGING 1
