#include "Runtime.h"

Tensor Runtime::run(const Model& model, const Tensor& input) const
{
    return execution_engine_.execute(model, input);
}