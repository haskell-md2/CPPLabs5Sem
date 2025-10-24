#pragma once

#include "../Interfaces/IOperation.h"

class Div : public IOperation
{

public:
    std::string getName() const override {
        return "/";
    }
    
    size_t getArgumentCount() const override {
        return 2;
    }
    
    float execute(const std::vector<float>& args) override {
        if (args.size() != getArgumentCount()) {
            // throw std::invalid_argument("Plus requires exactly 2 argument");
        }
        return args[0] / args[1];
    }
    
};
