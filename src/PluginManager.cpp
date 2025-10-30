#include "PluginManager.h"
#include <iostream>

PluginManager::PluginManager() = default;

PluginManager::PluginManager(const std::string& pluginsDirectory) 
    : pluginsDir(pluginsDirectory) {}

PluginManager::~PluginManager() {
    closeAllPlugins();
}

void PluginManager::setPluginsDirectory(const std::string& directory) {
    pluginsDir = directory;
}

std::string PluginManager::getPluginsDirectory() const {
    return pluginsDir;
}

std::vector<std::unique_ptr<IOperation>> PluginManager::loadOperations() {
    std::vector<std::unique_ptr<IOperation>> operations;
    
    try {
        if (!std::filesystem::exists(pluginsDir)) {
            std::cout << "Директория плагинов не найдена, исправляемся...: " << pluginsDir << std::endl;
            std::filesystem::create_directory(pluginsDir);
            return operations;
        }
        
        std::cout << "Загрузка плагинов из: " << pluginsDir << std::endl;
        
        for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
            std::string extension = entry.path().extension().string();
            
            if (extension == ".dll" || extension == ".so") {
                std::string pluginPath = entry.path().string();
                std::unique_ptr<IOperation> operation = loadPlugin(pluginPath);
                if (operation) {
                    operations.push_back(std::move(operation));
                }
            }
        }
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка при загрузке плагинов: " << e.what() << std::endl;
    }
    
    std::cout << "Загружено операций: " << operations.size() << std::endl;
    return operations;
}

std::unique_ptr<IOperation> PluginManager::loadPlugin(const std::string& pluginPath) {
    DLL_HANDLE handle = DLL_LOAD(pluginPath);
    
    if (!handle) {
#ifdef _WIN32
        std::cout << "Не удалось загрузить плагин: " << pluginPath 
                  << ". Ошибка: " << GetLastError() << std::endl;
#else
        std::cout << "Не удалось загрузить плагин: " << pluginPath 
                  << ". Ошибка: " << dlerror() << std::endl;
#endif
        return nullptr;
    }
    
    auto createFunc = (IOperation*(*)())DLL_GETSYM(handle, "createOperation");
    auto destroyFunc = (void(*)(IOperation*))DLL_GETSYM(handle, "destroyOperation");
    
    if (!createFunc || !destroyFunc) {
        std::cout << "Неверный формат плагина: " << pluginPath 
                  << " (отсутствуют необходимые функции)" << std::endl;
        DLL_CLOSE(handle);
        return nullptr;
    }
    
    try {
        IOperation* operation = createFunc();
        if (operation) {
            loadedHandles.push_back(handle);
            std::cout << "Успешно загружен плагин: " << pluginPath 
                      << " (операция: " << operation->getName() << ")" << std::endl;
            
            return std::unique_ptr<IOperation>(operation);
        } else {
            std::cout << "Плагин не создал операцию: " << pluginPath << std::endl;
            DLL_CLOSE(handle);
            return nullptr;
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка при создании операции из плагина " 
                  << pluginPath << ": " << e.what() << std::endl;
        DLL_CLOSE(handle);
        return nullptr;
    }
}

void PluginManager::closeAllPlugins() {
    for (DLL_HANDLE handle : loadedHandles) {
        DLL_CLOSE(handle);
    }
    loadedHandles.clear();
    std::cout << "Все плагины выгружены" << std::endl;
}