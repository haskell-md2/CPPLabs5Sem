#pragma once

#include "../Interfaces/IOperation.h"

class UnarMinus : public IOperation
{

public:
    std::string getName() const override {
        return "-_unar";
    }
    
    size_t getArgumentCount() const override {
        return 1;
    }

    int getPriority() const override {
        return 4;
    }
    
    float execute(const std::vector<float>& args) override {

        return -args[0];
    }
    
};


