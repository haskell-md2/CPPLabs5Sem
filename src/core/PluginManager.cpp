#include "headers/PluginManager.h"
#include <iostream>


PluginManager::PluginManager() = default;

PluginManager::PluginManager(const std::string& pluginsDirectory)
    : pluginsDir(pluginsDirectory) {}

PluginManager::PluginManager(PluginManager&& other) noexcept
    : loadedHandles(std::move(other.loadedHandles)),
      pluginsDir(std::move(other.pluginsDir)) {}

PluginManager& PluginManager::operator=(PluginManager&& other) noexcept {
    if (this != &other) {
        closeAllPlugins();
        loadedHandles = std::move(other.loadedHandles);
        pluginsDir = std::move(other.pluginsDir);
    }
    return *this;
}

PluginManager::~PluginManager() {
    closeAllPlugins();
}

void PluginManager::setPluginsDirectory(const std::string& directory) {
    pluginsDir = directory;
}

std::string PluginManager::getPluginsDirectory() const {
    return pluginsDir;
}

std::vector<std::shared_ptr<IOperation>> PluginManager::loadOperations() {
    std::vector<std::shared_ptr<IOperation>> operations;
    
    try {
        if (!std::filesystem::exists(pluginsDir)) {
            std::cout << "Плагины не найдены: " << pluginsDir << std::endl;
            return operations;
        }

        std::cout << "Загружаем плагины из " << pluginsDir << std::endl;
        
        for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
            std::string extension = entry.path().extension().string();
            
            if (extension == ".dll" || extension == ".so") {
                std::string pluginPath = entry.path().string();

                auto operation = loadPlugin(pluginPath);
                
                if (operation) {
                    operations.push_back(operation);
                    std::cout << "Успешно загружен плагин: " << pluginPath << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Загрузка плагина выдала исключение: " << e.what() << std::endl;
    }

    return operations;
}

std::shared_ptr<IOperation> PluginManager::loadPlugin(const std::string& pluginPath) {

    DLL_HandlePtr handle(DLL_LOAD(pluginPath));


    if (!handle) {
        std::cerr << "Не удалось загрузить плагин: " << pluginPath << std::endl;
        return nullptr; 
    }

    typedef IOperation* (*CreateFunc)();
    typedef void (*DestroyFunc)(IOperation*);

    auto createFunc = reinterpret_cast<CreateFunc>(DLL_GETSYM(handle.get(), "createOperation"));
    auto destroyFunc = reinterpret_cast<DestroyFunc>(DLL_GETSYM(handle.get(), "destroyOperation"));

    if (!createFunc || !destroyFunc) {
        std::cerr << "Не получилось найти функции в плагине: " << pluginPath << std::endl;
        return nullptr;
    }

    try {
        IOperation* operation = createFunc();
        
        if (!operation) {
            return nullptr;
        }

        auto operation_ptr = std::shared_ptr<IOperation>(
            operation,
            [destroyFunc](IOperation* op) { 
                if (op) destroyFunc(op);
            }
        );
        

        loadedHandles.push_back(std::move(handle));
        
        return operation_ptr;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка создания операции: " << e.what() << std::endl;
        return nullptr; 
    }
}

void PluginManager::closeAllPlugins() noexcept {
    loadedHandles.clear();
}