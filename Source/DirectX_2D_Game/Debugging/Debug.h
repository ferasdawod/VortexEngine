#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <memory>

#include "Logger.h"
#include "Performance/ScopedProfilerSample.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using std::string;

#if defined(DEBUG) || defined(_DEBUG)
#ifndef Assert
#define Assert(experssion, msg) assert((experssion) && msg)
#endif
#else
#ifndef Assert
#define Assert(experssion, msg) do { } while(0)
#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_LEAK_CHECK() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF)
#else
#define DEBUG_LEAK_CHECK() do { } while(0)
#endif

/// redefine the new keyword to catch memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#ifndef DBG_NEW
#define DBG_NEW_ new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define DBG_NEW DBG_NEW_
#endif // DBG_NEW
#else
#ifndef DBG_NEW
#define DBG_NEW new
#endif
#endif // DEBUG

/// helper macros for releasing com objects
#define SAFE_RELEASE(p) { if (p) { p->Release(); p = nullptr; } }
#define SAFE_DELETE(p)  { if (p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] p; p = nullptr; } }

/// helper macros for logging messages and errors
#if defined(DEBUG) || defined(_DEBUG)
#define LOG(msg, cat, file, function, line, hr, pause) \
do \
{ \
	std::string msgStr(msg); \
	std::stringstream stream; \
	stream << "[" << cat << "] " << msgStr << std::endl; \
	if (hr != 0) stream << "HRESULT : " << hr << std::endl; \
	stream << "Function : " << function << std::endl; \
	stream << "File : " << file << std::endl; \
	stream << "Line : " << line << std::endl; \
	msgStr = stream.str(); \
	Logger::GetPtr()->Log(msgStr); \
	if (pause) \
	{ \
		int result = MessageBoxA(nullptr, msgStr.c_str(), "Error", MB_ABORTRETRYIGNORE | MB_ICONERROR); \
		if (result == IDABORT) \
		{ \
			exit(0); \
		} \
		else if (result == IDRETRY) \
		{ \
			__asm int 3 \
		} \
	} \
	else \
	{ \
		OutputDebugString(msgStr.c_str()); \
	} \
} while (0)
#else
#define LOG(msg, cat, file, function, line, hr, pause) do { } while(0)
#endif

#define LOG_M(msg) LOG(msg, "Message", __FILE__, __FUNCTION__, __LINE__, 0, false)
#define LOG_E(msg, hr) LOG(msg, "Error", __FILE__, __FUNCTION__, __LINE__, hr, true)
