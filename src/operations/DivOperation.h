#pragma once

#include "IOperation.h"

#include <stdexcept>

class Div : public IOperation
{

public:
    std::string getName() const override {
        return "/";
    }
    
    size_t getArgumentCount() const override {
        return 2;
    }
    
    int getPriority() const override {
        return 2;
    }

    float execute(const std::vector<float>& args) override {

        if(args[1] == 0){
            throw std::invalid_argument("Деление на ноль! Предельные переходы мы не разрешали!");
        }
        return args[0] / args[1];
    }
    
};
