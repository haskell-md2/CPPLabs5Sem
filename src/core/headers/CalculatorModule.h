#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include <algorithm>


#include "../../operations/PlusOperation.h"
#include "../../operations/MinusOperation.h"
#include "../../operations/MultOperation.h"
#include "../../operations/DivOperation.h"
#include "../../operations/UnarMinusOperation.h"

#include "PluginManager.h"

#include "RawDataProcessor.h"
#include "Tokenezator.h"

class CalculatorModule {
private:

    PluginManager pluginManager;

    std::vector<std::string> _postfix;
    std::unordered_map<std::string, std::shared_ptr<IOperation>> operations;
    

    RawDataProcessor _rdp;
    Tokenezator _tr;

    int priority(const std::string& s);
    
    void registerBuiltinOperations();
    void loadPlugins();

public:
    CalculatorModule();
    ~CalculatorModule() = default;
    
    void registerOperation(std::shared_ptr<IOperation> operation);
    float calculate(std::string input);
};