#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "Interfaces/IOperation.h"

class RawDataProcessor
{
private:
    std::unordered_map<std::string, std::shared_ptr<IOperation>> _operations;

public:
    RawDataProcessor(){};
    RawDataProcessor(std::unordered_map<std::string, std::shared_ptr<IOperation>> & operations): _operations(operations){}

    std::vector<std::string> split(std::string input);
    bool isDig(char c);
    bool isFunction(const std::string& token);
};

