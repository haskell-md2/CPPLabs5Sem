#pragma once

#include "../operations/IOperation.h"

extern "C" {
    IOperation* createOperation();
    void destroyOperation(IOperation* operation);
}