#include <iostream>

#include "Model.h"

int main()
{
    Model model;
    model.add_node(Node(OperationType::MULTIPLY, 2.0f));
    model.add_node(Node(OperationType::RELU));
    model.add_node(Node(OperationType::ADD, 5.0f));

    std::cout << "Number of nodes: " << model.nodes().size() << std::endl;
    return 0;
}