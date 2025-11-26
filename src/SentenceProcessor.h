#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cmath>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <algorithm>


#include "StandartOperations/PlusOperation.h"
#include "StandartOperations/MinusOperation.h"
#include "StandartOperations/MultOperation.h"
#include "StandartOperations/DivOperation.h"
#include "StandartOperations/UnarMinusOperation.h"

#include "PluginManager.h"

#include "RawDataProcessor.h"

class SentenceProcessor {
private:

    PluginManager pluginManager;

    std::vector<std::string> _postfix;
    std::unordered_map<std::string, std::shared_ptr<IOperation>> operations;
    
    std::vector<std::string> _getPostfix(std::string input);

    RawDataProcessor _rdp;

    int priority(const std::string& s);
    
    void registerBuiltinOperations();
    void loadPlugins();

public:
    SentenceProcessor();
    ~SentenceProcessor() = default;
    
    void registerOperation(std::shared_ptr<IOperation> operation);
    float calculate(std::string input);
};