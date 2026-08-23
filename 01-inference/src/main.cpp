#include <iostream>
#include "Node.h"

int main()
{
    Node multiply_node(OperationType::MULTIPLY, 2.0F);
    Node relu_node(OperationType::RELU);

    std::cout << "Multiply node parameter: "
              << multiply_node.parameter()
              << std::endl;
    
    std::cout << "ReLU node parameter: "
              << relu_node.parameter()
              << std::endl;

    return 0;
}