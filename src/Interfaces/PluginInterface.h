#pragma once

#include "IOperation.h"

extern "C" {
    IOperation* createOperation();
    void destroyOperation(IOperation* operation);
}