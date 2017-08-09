#pragma once

#include <string>

using std::string;

// Assert Macro
#if defined(DEBUG) || defined(_DEBUG)
#ifndef Assert
#define Assert(experssion, msg) assert((experssion) && msg)
#endif
#else
#ifndef Assert
#define Assert(experssion, msg) do { } while(0)
#endif
#endif

// Debug Leak Check
#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_LEAK_CHECK() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF)
#else
#define DEBUG_LEAK_CHECK() do { } while(0)
#endif

// Redefine new key word
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

// Helper Macros For COM Objects
#define SAFE_RELEASE(p)			do { if (p) { p->Release(); p = nullptr; } } while(0)
#define SAFE_DELETE(p)			do { if (p) { delete p; p = nullptr; } } while (0)
#define SAFE_DELETE_ARRAY(p)	do { if (p) { delete[] p; p = nullptr; } } while (0)

// Logging Macros
#if defined(DEBUG) || defined(_DEBUG)
#define LOG(msg, cat) \
do \
{ \
	Logger::GetPtr()->Log(cat, msg); \
} while (0)
#else
#define LOG(msg, cat) do { } while(0)
#endif

#define LOG_M(msg) LOG(msg, MessageType::Info)
#define LOG_W(msg) LOG(msg, MessageType::Warning)

#if defined(DEBUG) || defined(_DEBUG)
#define LOG_E(msg, hr) \
do \
{ \
	LOG(msg, MessageType::Error); \
	int result = MessageBoxA(nullptr, msg, "Error", MB_ABORTRETRYIGNORE | MB_ICONERROR); \
	if (result == IDABORT) \
	{ \
		exit(0); \
	} \
	else if (result == IDRETRY) \
	{ \
		/* trigger a break point interrupt */  \
		__asm int 3 \
	} \
} while (0)
#else
#define LOG_E(msg, hr) do { } while(0)
#endif