#include <iostream>

#include "ModelLoader.h"
#include "Runtime.h"

int main()
{
    ModelLoader loader;

    Model model = loader.load(
        "models/simple_model.txt"
    );

    Tensor input_a(
        "input_a",
        {1.0f, 2.0f, 3.0f}
    );

    Tensor input_b(
        "input_b",
        {10.0f, 20.0f, 30.0f}
    );

    Runtime runtime;

    std::vector<Tensor> inputs = {input_a, input_b};
    Tensor output = runtime.run(model, inputs);

    std::cout << "Input A: ";

    for (float value : input_a.values())
    {
        std::cout << value << " ";
    }

    std::cout << "\nInput B: ";

    for (float value : input_b.values())
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