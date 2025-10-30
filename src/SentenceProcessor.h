#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "StandartOperations/PlusOperation.h"
#include "StandartOperations/MinusOperation.h"
#include "StandartOperations/MultOperation.h"
#include "StandartOperations/DivOperation.h"
#include "StandartOperations/UnarMinusOperation.h"

#include "PluginManager.h"


class SentenceProcessor {
private:

    PluginManager pluginManager;

    std::vector<std::string> _postfix;
    std::unordered_map<std::string, std::unique_ptr<IOperation>> operations;
    
    std::vector<std::string> _getPostfix(std::string input);
    std::vector<std::string> _split(std::string input);
    
    bool isDig(char c);
    bool isFunction(const std::string& token);
    int priority(const std::string& s);
    
    void registerBuiltinOperations();
    void loadPlugins();

public:
    SentenceProcessor();
    ~SentenceProcessor() = default;
    
    void registerOperation(std::unique_ptr<IOperation> operation);
    float calculate(std::string input);
};