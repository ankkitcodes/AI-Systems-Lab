#pragma once

#include "ExecutionEngine.h"
#include "Model.h"
#include "Tensor.h"

class Runtime
{
public:
    Tensor run(const Model& model, const Tensor& input) const;

private:
    ExecutionEngine execution_engine_;
};
