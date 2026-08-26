#include <iostream>

#include "Runtime.h"

int main()
{
    Model model;

    model.add_node(Node(OperationType::MULTIPLY, 2.0f));
    model.add_node(Node(OperationType::RELU));
    model.add_node(Node(OperationType::ADD, 5.0f));

    Tensor input({1.0f, -2.0f, 3.0f});
    Runtime runtime;
    Tensor output = runtime.run(model, input);

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
