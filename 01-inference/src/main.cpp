#include <iostream>

#include "ExecutionEngine.h"
#include "Node.h"

int main()
{
    Model model;

    model.add_node(Node(
        OperationType::MULTIPLY,
        "input",
        "hidden",
        2.0f
    ));

    model.add_node(Node(
        OperationType::RELU,
        "hidden",
        "output"
    ));

    Tensor input("input", {1.0f, -2.0f, 3.0f});

    ExecutionEngine engine;

    Tensor output = engine.execute(model, input);

    std::cout << "Input: ";
    for (float value : input.values())
    {
        std::cout << value << " ";
    }
    
    std::cout << "Output: ";
    for (float value : output.values())
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}