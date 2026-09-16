#include <iostream>
#include <vector>
#include <stdexcept>

#include "Model.h"
#include "Node.h"
#include "Runtime.h"
#include "Tensor.h"


bool test_single_input()
{
    // input -> MULTIPLY(2) -> RELU -> output

    Model model;

    model.add_input("input");
    model.set_output("output");

    model.add_node(
        Node(
            OperationType::MULTIPLY,
            {"input"},
            {"hidden"},
            2.0f
        )
    );

    model.add_node(
        Node(
            OperationType::RELU,
            {"hidden"},
            {"output"}
        )
    );

    Tensor input(
        "input",
        {1.0f, -2.0f, 3.0f}
    );

    Runtime runtime;

    Tensor output = runtime.run(
        model,
        {input}
    );

    const std::vector<float> expected = {
        2.0f,
        0.0f,
        6.0f
    };

    if (output.size() != expected.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        if (output.values()[i] != expected[i])
        {
            return false;
        }
    }

    return true;
}


bool test_multi_input_add()
{
    // input_a ──┐
    //            ├── ADD ──> output
    // input_b ──┘

    Model model;

    model.add_input("input_a");
    model.add_input("input_b");

    model.set_output("output");

    model.add_node(
        Node(
            OperationType::ADD,
            {
                "input_a",
                "input_b"
            },
            {
                "output"
            }
        )
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

    Tensor output = runtime.run(
        model,
        {
            input_a,
            input_b
        }
    );

    const std::vector<float> expected = {
        11.0f,
        22.0f,
        33.0f
    };

    if (output.size() != expected.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        if (output.values()[i] != expected[i])
        {
            return false;
        }
    }

    return true;
}

bool test_out_of_order_execution()
{
    Model model;

    model.add_input("input");
    model.set_output("output");

    // Intentionally add nodes in the WRONG order.
    model.add_node(
        Node(
            OperationType::RELU,
            {"hidden"},
            {"output"}
        )
    );

    model.add_node(
        Node(
            OperationType::MULTIPLY,
            {"input"},
            {"hidden"},
            2.0f
        )
    );

    Tensor input(
        "input",
        {1.0f, -2.0f, 3.0f}
    );

    Runtime runtime;

    Tensor output = runtime.run(
        model,
        {input}
    );

    const std::vector<float> expected = {
        2.0f,
        0.0f,
        6.0f
    };

    if (output.size() != expected.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        if (output.values()[i] != expected[i])
        {
            return false;
        }
    }

    return true;
}

bool test_missing_dependency()
{
    Model model;

    model.add_input("input");
    model.set_output("output");

    // "missing" is never produced by any node.
    model.add_node(
        Node(
            OperationType::RELU,
            {"missing"},
            {"output"}
        )
    );

    Tensor input(
        "input",
        {1.0f, 2.0f, 3.0f}
    );

    Runtime runtime;

    try
    {
        runtime.run(
            model,
            {input}
        );

        // The runtime should NOT successfully execute.
        return false;
    }
    catch (const std::runtime_error&)
    {
        // An exception is the expected behavior.
        return true;
    }
}


bool test_cycle_detection()
{
    Model model;

    model.add_input("input");
    model.set_output("output");

    // Intentional cycle:
    //
    // input -> node1 -> tensor_a
    // tensor_a -> node2 -> tensor_b
    // tensor_b -> node1 would be required to complete it
    //
    // Simpler explicit cycle:
    //
    // tensor_a -> tensor_b
    // tensor_b -> tensor_a

    model.add_node(
        Node(
            OperationType::RELU,
            {"tensor_b"},
            {"tensor_a"}
        )
    );

    model.add_node(
        Node(
            OperationType::RELU,
            {"tensor_a"},
            {"tensor_b"}
        )
    );

    Tensor input(
        "input",
        {1.0f, 2.0f, 3.0f}
    );

    Runtime runtime;

    try
    {
        runtime.run(
            model,
            {input}
        );

        // The graph contains a cycle and should not execute.
        return false;
    }
    catch (const std::runtime_error&)
    {
        // An exception is the expected behavior.
        return true;
    }
}

bool test_wrong_input_count()
{
    Model model;

    model.add_input("input_a");
    model.add_input("input_b");

    model.set_output("output");

    model.add_node(
        Node(
            OperationType::ADD,
            {
                "input_a",
                "input_b"
            },
            {
                "output"
            }
        )
    );

    // Model expects TWO inputs,
    // but we provide only ONE.
    Tensor input_a(
        "input_a",
        {1.0f, 2.0f, 3.0f}
    );

    Runtime runtime;

    try
    {
        runtime.run(
            model,
            {input_a}
        );

        // Execution should not succeed.
        return false;
    }
    catch (const std::runtime_error&)
    {
        return true;
    }
}

bool test_add_size_mismatch()
{
    Model model;

    model.add_input("input_a");
    model.add_input("input_b");

    model.set_output("output");

    model.add_node(
        Node(
            OperationType::ADD,
            {
                "input_a",
                "input_b"
            },
            {
                "output"
            }
        )
    );

    Tensor input_a(
        "input_a",
        {1.0f, 2.0f, 3.0f}
    );

    Tensor input_b(
        "input_b",
        {10.0f, 20.0f}
    );

    Runtime runtime;

    try
    {
        runtime.run(
            model,
            {
                input_a,
                input_b
            }
        );

        // Execution should not succeed.
        return false;
    }
    catch (const std::runtime_error&)
    {
        return true;
    }
}



bool test_duplicate_input_tensor()
{
    Model model;

    model.add_input("input_a");
    model.add_input("input_b");

    model.set_output("output");

    model.add_node(
        Node(
            OperationType::ADD,
            {
                "input_a",
                "input_b"
            },
            {
                "output"
            }
        )
    );

    Tensor input_a(
        "input_a",
        {1.0f, 2.0f, 3.0f}
    );

    // Deliberately give the second tensor the same name.
    Tensor duplicate_input(
        "input_a",
        {10.0f, 20.0f, 30.0f}
    );

    Runtime runtime;

    try
    {
        runtime.run(
            model,
            {
                input_a,
                duplicate_input
            }
        );

        // Execution should not succeed.
        return false;
    }
    catch (const std::runtime_error&)
    {
        // Duplicate input should be rejected.
        return true;
    }
}


int main()
{
    bool all_tests_passed = true;


    if (test_single_input())
    {
        std::cout << "[PASS] Single-input runtime test\n";
    }
    else
    {
        std::cout << "[FAIL] Single-input runtime test\n";
        all_tests_passed = false;
    }


    if (test_multi_input_add())
    {
        std::cout << "[PASS] Multi-input ADD test\n";
    }
    else
    {
        std::cout << "[FAIL] Multi-input ADD test\n";
        all_tests_passed = false;
    }

    if (test_out_of_order_execution())
    {
        std::cout << "[PASS] Out-of-order execution test\n";

    }
    else
    {
        std::cout << "[FAIL] Out-of-order execution test\n";
        all_tests_passed = false;
    }

    if (test_missing_dependency())
    {
        std::cout << "[PASS] Missing dependency test\n";
    }
    else
    {
        std::cout << "[FAIL] Missing dependency test\n";
        all_tests_passed = false;
    }
    
    
    if (test_cycle_detection())
    {
        std::cout << "[PASS] Cycle detection test\n";
    }
    else
    {
        std::cout << "[FAIL] Cycle detection test\n";
        all_tests_passed = false;
    }

    if (test_wrong_input_count())
    {
        std::cout << "[PASS] Wrong input count test\n";
    }
    else
    {
        std::cout << "[FAIL] Wrong input count test\n";
        all_tests_passed = false;
    }
    
    
    if (test_add_size_mismatch())
    {
        std::cout << "[PASS] ADD tensor-size mismatch test\n";
    }
    else
    {
        std::cout << "[FAIL] ADD tensor-size mismatch test\n";
        all_tests_passed = false;
    }
    if (test_duplicate_input_tensor())
    {
        std::cout << "[PASS] Duplicate input tensor test\n";
    }
    else
    {
        std::cout << "[FAIL] Duplicate input tensor test\n";
        all_tests_passed = false;
    }
    return all_tests_passed ? 0 : 1;
}