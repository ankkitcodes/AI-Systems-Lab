#include <iostream>

#include "ExecutionEngine.h"

int main()
{
    Tensor input({1.0f, -2.0f, 3.0f});

    Node multiply_node(OperationType::MULTIPLY, 2.0f);

    ExecutionEngine engine;

    Tensor output = engine.execute(multiply_node, input);

    std::cout << "Input: ";

    for (float value : input.values())
    {
        std::cout << value << " ";
    }

    std::cout << "\nOutput: ";

    for (float value : output.values())
    {
        std::cout << value << " ";
    }

    std::cout << std::endl;

    return 0;
}