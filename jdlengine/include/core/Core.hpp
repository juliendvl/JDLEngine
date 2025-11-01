#pragma once


// --- DLL Import/Export ---
#ifdef _WIN32
    #ifdef JDL_BUILD_DLL
        #define JDL_API __declspec(dllexport)
    #else
        #define JDL_API __declspec(dllimport)
    #endif // JDL_BUILD_DLL
#else
    #define JDL_API 
#endif // _WIN32


// --- Common Includes ---
#include <iostream>
#include <memory>
#include <string>
#include <vector>


// --- Pragma Warnings ---
#pragma warning(disable: 4251)
