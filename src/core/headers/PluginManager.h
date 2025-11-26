#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <filesystem>

#include "../../operations/IOperation.h"

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


struct DLLDeleter {
    void operator()(DLL_HANDLE handle) const noexcept {
        if (handle != nullptr) {
            DLL_CLOSE(handle);
        }
    }
};


#ifdef _WIN32
using DLL_HandlePtr = std::unique_ptr<std::remove_pointer_t<HMODULE>, DLLDeleter>;
#else
using DLL_HandlePtr = std::unique_ptr<void, DLLDeleter>;
#endif

class PluginManager {
private:
    std::vector<DLL_HandlePtr> loadedHandles;
    std::string pluginsDir = "./plugins";
    
    std::shared_ptr<IOperation> loadPlugin(const std::string& pluginPath);
    void closeAllPlugins() noexcept;

public:

    PluginManager();
    explicit PluginManager(const std::string& pluginsDirectory);
    
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;
    
    PluginManager(PluginManager&& other) noexcept;
    PluginManager& operator=(PluginManager&& other) noexcept;
    
    ~PluginManager() noexcept; 
    
    void setPluginsDirectory(const std::string& directory);
    std::string getPluginsDirectory() const;
    std::vector<std::shared_ptr<IOperation>> loadOperations();
};