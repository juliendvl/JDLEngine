#pragma once


// --- PLATFORM DETECTION ---
#ifdef _WIN32
    #define JDL_WINDOWS
#else
    #error "Unsupported Platform!"
#endif // _WIN32

// --- DLL IMPORT/EXPORT ---
#ifdef JDL_WINDOWS
    #ifdef JDL_BUILD_DLL
        #define JDL_API __declspec(dllexport)
    #else
        #define JDL_API __declspec(dllimport)
    #endif // JDL_BUILD_DLL
#else
    #define JDL_API 
#endif // JDL_WINDOWS

// --- COMMON INCLUDES ---
#include <memory>
#include <string>
#include <vector>

// --- WARNINGS ---
#pragma warning(disable: 4251)
