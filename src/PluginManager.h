#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <filesystem>

#include "Interfaces/IOperation.h"

#ifdef _WIN32
    #include <windows.h>
    #define DLL_HANDLE HMODULE
    #define DLL_LOAD(path) LoadLibraryA(path.c_str())
    #define DLL_GETSYM(handle, sym) GetProcAddress(handle, sym)
    #define DLL_CLOSE(handle) FreeLibrary(handle)
#else
    #include <dlfcn.h>
    #define DLL_HANDLE void*
    #define DLL_LOAD(path) dlopen(path.c_str(), RTLD_LAZY)
    #define DLL_GETSYM(handle, sym) dlsym(handle, sym)
    #define DLL_CLOSE(handle) dlclose(handle)
#endif

class PluginManager {
private:
    std::vector<DLL_HANDLE> loadedHandles;
    std::string pluginsDir = "./plugins";
    
    std::unique_ptr<IOperation> loadPlugin(const std::string& pluginPath);
    void closeAllPlugins();

public:
    PluginManager();
    explicit PluginManager(const std::string& pluginsDirectory);
    ~PluginManager();
    
    std::vector<std::unique_ptr<IOperation>> loadOperations();
    
    void setPluginsDirectory(const std::string& directory);
    std::string getPluginsDirectory() const;
};