#pragma once

#include "../Interfaces/IOperation.h"

class Minus : public IOperation
{

public:
    std::string getName() const override {
        return "-";
    }
    
    size_t getArgumentCount() const override {
        return 2;
    }

    int getPriority() const override {
        return 1;
    }
    
    float execute(const std::vector<float>& args) override {

        return args[0] - args[1];
    }
    
};


