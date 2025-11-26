#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <stack>
#include <stdexcept>

#include "RawDataProcessor.h"

#include "../../operations/IOperation.h"

class Tokenezator
{
private:
    std::unordered_map<std::string, std::shared_ptr<IOperation>> _operations;
    int priority(const std::string& s);
    RawDataProcessor _rdp;

public:
    Tokenezator(std::unordered_map<std::string, std::shared_ptr<IOperation>> operations, RawDataProcessor rdp): _operations(operations), _rdp(rdp){}
    Tokenezator(){}

    std::vector<std::string> getPostfix(std::string input);
};


