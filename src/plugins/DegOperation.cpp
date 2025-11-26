#include <cmath>
#include "PluginInterface.h"

class DegOperation : public IOperation {
public:
    std::string getName() const override {
        return "^";
    }
    
    size_t getArgumentCount() const override {
        return 2;
    }
    
    int getPriority() const override {
        return 3;
    }
    
    float execute(const std::vector<float>& args) override {
        return std::pow(args[0], args[1]);
    }
};

extern "C" {
    IOperation* createOperation() {
        return new DegOperation();
    }
    
    void destroyOperation(IOperation* operation) {
        delete operation;
    }
}