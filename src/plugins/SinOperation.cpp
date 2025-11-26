#include <cmath>
#include "PluginInterface.h"

class SinOeration : public IOperation {
public:
    std::string getName() const override {
        return "sin";
    }
    
    size_t getArgumentCount() const override {
        return 1;
    }
    
    int getPriority() const override {
        return 4;
    }
    
    float execute(const std::vector<float>& args) override {
        return std::sin(args[0]);
    }
};

extern "C" {
    IOperation* createOperation() {
        return new SinOeration();
    }
    
    void destroyOperation(IOperation* operation) {
        delete operation;
    }
}