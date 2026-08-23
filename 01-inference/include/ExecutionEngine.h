#pragma once

#include "Node.h"
#include "Tensor.h"

class ExecutionEngine
{
public:
    Tensor execute(const Node& node, const Tensor& input) const;
};