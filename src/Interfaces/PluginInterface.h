#pragma once

#include "IOperation.h"

#ifdef _WIN32
    #ifdef PLUGIN_EXPORTS
        #define PLUGIN_API __declspec(dllexport)
    #else
        #define PLUGIN_API __declspec(dllimport)
    #endif
#else
    #define PLUGIN_API
#endif

extern "C" {
    PLUGIN_API IOperation* createOperation();
    PLUGIN_API void destroyOperation(IOperation* operation);
}