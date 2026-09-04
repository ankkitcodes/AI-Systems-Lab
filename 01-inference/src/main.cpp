#include <iostream>

#include "ModelLoader.h"
#include "Runtime.h"

int main()
{
    ModelLoader loader;

    Model model = loader.load(
        "models/simple_model.txt"
    );

    Tensor input(
        "input",
        {1.0f, -2.0f, 3.0f}
    );

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