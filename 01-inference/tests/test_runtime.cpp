#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cstdio>

#include "Model.h"
#include "ModelLoader.h"
#include "Node.h"
#include "Runtime.h"
#include "Tensor.h"
#include "ExecutionEngine.h"


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
        {1.0f, -2.0f, 3.0f}, {3}
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
        {1.0f, 2.0f, 3.0f},{3}
    );

    Tensor input_b(
        "input_b",
        {10.0f, 20.0f, 30.0f},{3}
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
        {1.0f, -2.0f, 3.0f}, {3}
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

bool test_valid_tensor_shape()
{
    Tensor tensor(
        "input",
        {
            1.0f,
            2.0f,
            3.0f,
            4.0f,
            5.0f,
            6.0f
        },
        {2,3}
    );

    if (tensor.size() != 6)
    {
        return false;
    } 

    const std::vector<std::size_t> expected_shape = {2,3};

    return tensor.shape() == expected_shape;
}

bool test_invalid_tensor_shape()
{
    try
    {
        Tensor tensor(
            "input",
            {
                1.0f,
                2.0f,
                3.0f,
                4.0f,
                5.0f
            },
            {2,3}
        );
        // 2 x 3 = 6, but only 5 values were supplied.
        return false;
    }
    catch (const std::runtime_error&)
    {
        return true;
    }
}

bool test_zero_tensor_dimension()
{
    try{
        Tensor tensor(
            "input",
            {
                1.0f,
                2.0f
            },
            {2,0}
        );
        return false;
    }
    catch (const std::runtime_error&){
        return true;
    }
}
bool test_add_matching_shapes()
{
    Node add_node(
        OperationType::ADD,
        {"a", "b"},
        {"output"}
    );

    Tensor a(
        "a",
        {1.0f, 2.0f, 3.0f, 4.0f},
        {2, 2}
    );

    Tensor b(
        "b",
        {10.0f, 20.0f, 30.0f, 40.0f},
        {2, 2}
    );

    ExecutionEngine engine;

    Tensor output = engine.execute(
        add_node,
        {a, b}
    );

    return output.values() ==
               std::vector<float>{
                   11.0f,
                   22.0f,
                   33.0f,
                   44.0f
               }
        &&
           output.shape() ==
               std::vector<std::size_t>{2, 2};
}
bool test_add_incompatible_shapes()
{
    Node add_node(
        OperationType::ADD,
        {"a","b"},
        {"output"}
    );

    Tensor a(
        "a",
        {1.0f, 2.0f, 3.0f, 4.0f},
        {2,2}
    );
    Tensor b(
        "b",
        {10.0f, 20.0f, 30.0f, 40.0f},
        {4,1}
    );
    ExecutionEngine engine;

    try{
        engine.execute(add_node, {a,b});
        return false;
    }
    catch (const std::runtime_error&)
    {
        return true;
    }
}

bool test_multiply_preserve_shape()
{
    Node multiply_node(
        OperationType::MULTIPLY,
        {"input"},
        {"output"},
        2.0f
    );
    Tensor input(
        "input",
        {1.0f,2.0f,3.0f,4.0f},
        {2,2}
    );
    ExecutionEngine engine;

    Tensor output = engine.execute(
        multiply_node,
        {input}
    );

    return output.values() == std::vector<float>{
        2.0f,
        4.0f,
        6.0f,
        8.0f
    }
    && output.shape() == std::vector<std::size_t>{2,2};
}

bool test_relu_preserves_shape()
{
    Node relu_node(
        OperationType::RELU,
        {"input"},
        {"output"}
    );

    Tensor input(
        "input",
        {-1.0f, 2.0f, -3.0f, 4.0f},
        {2, 2}
    );

    ExecutionEngine engine;

    Tensor output = engine.execute(
        relu_node,
        {input}
    );

    return output.values() ==
               std::vector<float>{
                   0.0f,
                   2.0f,
                   0.0f,
                   4.0f
               }
        &&
           output.shape() ==
               std::vector<std::size_t>{2, 2};
}
bool test_matmul_basic()
{
    Node matmul_node(
        OperationType::MATMUL,
        {"a", "b"},
        {"output"}
    );

    Tensor a(
        "a",
        {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f
        },
        {2, 3}
    );

    Tensor b(
        "b",
        {
             7.0f,  8.0f,
             9.0f, 10.0f,
            11.0f, 12.0f
        },
        {3, 2}
    );

    ExecutionEngine engine;

    Tensor output = engine.execute(
        matmul_node,
        {a, b}
    );

    return output.values() ==
               std::vector<float>{
                   58.0f,
                   64.0f,
                   139.0f,
                   154.0f
               }
        &&
           output.shape() ==
               std::vector<std::size_t>{2, 2};
}

bool test_matmul_incompatible_shapes()
{
    Node matmul_node(
        OperationType::MATMUL,
        {"a", "b"},
        {"output"}
    );

    Tensor a(
        "a",
        {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f
        },
        {2, 3}
    );

    Tensor b(
        "b",
        {
            1.0f, 2.0f,
            3.0f, 4.0f,
            5.0f, 6.0f,
            7.0f, 8.0f
        },
        {4, 2}
    );

    ExecutionEngine engine;

    try
    {
        engine.execute(
            matmul_node,
            {a, b}
        );

        return false;
    }
    catch (const std::runtime_error&)
    {
        return true;
    }
}
bool test_matmul_requires_2d()
{
    Node matmul_node(
        OperationType::MATMUL,
        {"a","b"},
        {"output"}
    );
    Tensor a(
        "a",
        {1.0f, 2.0f, 3.0f},
        {3}
    );
    Tensor b(
        "b",
        {
            1.0f, 2.0f,
            3.0f, 4.0f,
            5.0f, 6.0f
        },
        {3,2}
    );
    ExecutionEngine engine;

    try{
        engine.execute(
            matmul_node,
            {a,b}
        );
        return false;
    }
    catch (const std::runtime_error&)
    {
        return true;
    }
}

bool test_matmul_model_loader()
{
    const std::string file_path = "test_matmul_model.txt";

    std::ofstream file(file_path);

    if (!file.is_open())
    {
        return false;
    }
    file << "INPUT a\n";
    file << "INPUT b\n";
    file << "OUTPUT output\n";
    file << "\n";
    file << "NODE MATMUL a b output\n";

    file.close();

    try
    {
        ModelLoader loader;
        Model model = loader.load(file_path);

        Tensor a(
            "a",
            {
                1.0f, 2.0f, 3.0f,
                4.0f, 5.0f, 6.0f
            },
            {2,3}
        );

        Tensor b(
            "b",
            {
                7.0f, 8.0f,
                9.0f, 10.0f,
                11.0f, 12.0f
            },
            {3,2}
        );

        Runtime runtime;

        Tensor output = runtime.run(model, {a,b});

        bool passed =
            output.values() ==
                std::vector<float>{
                    58.0f,
                    64.0f,
                    139.0f,
                    154.0f
                }
            &&
            output.shape() ==
                std::vector<std::size_t>{2, 2};

        std::remove(file_path.c_str());

        return passed;
    }
    catch (const std::runtime_error&)
    {
        std::remove(file_path.c_str());
        return false;
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
    if (test_valid_tensor_shape())
    {
        std::cout << "[PASS] Valid tensor shape test\n";
    }
    else
    {
        std::cout << "[FAIL] Valid tensor shape test\n";
        all_tests_passed = false;
    }


    if (test_invalid_tensor_shape())
    {
        std::cout << "[PASS] Invalid tensor shape test\n";
    }
    else
    {
        std::cout << "[FAIL] Invalid tensor shape test\n";
        all_tests_passed = false;
    }


    if (test_zero_tensor_dimension())
    {
        std::cout << "[PASS] Zero tensor dimension test\n";
    }
    else
    {
        std::cout << "[FAIL] Zero tensor dimension test\n";
        all_tests_passed = false;
    }

    if (test_add_matching_shapes())
    {
        std::cout << "[PASS] ADD matching shapes test\n";
    }
    else
    {
        std::cout << "[FAIL] ADD matching shapes test\n";
        all_tests_passed = false;
    }

    if (test_add_incompatible_shapes())
    {
        std::cout << "[PASS] ADD incompatible shapes test\n";
    }
    else
    {
        std::cout << "[FAIL] ADD incompatible shapes test\n";
        all_tests_passed = false;
    }

    if (test_multiply_preserve_shape())
    {
        std::cout << "[PASS] MULTIPLY preserve shape test\n";
    }
    else
    {
        std::cout << "[FAIL] MULTIPLY preserve shape test\n";
        all_tests_passed = false;
    }

    if (test_relu_preserves_shape())
    {
        std::cout << "[PASS] RELU preserve shape test\n";
    }
    else
    {
        std::cout << "[FAIL] RELU preserve shape test\n";
        all_tests_passed = false;
    }

    if (test_matmul_basic())
    {
        std::cout << "[PASS] Basic MATMUL test\n";
    }
    else
    {
        std::cout << "[FAIL] Basic MATMUL test\n";
        all_tests_passed = false;
    }

    if (test_matmul_incompatible_shapes())
    {
        std::cout << "[PASS] MATMUL incompatible shapes test\n";
    }
    else
    {
        std::cout << "[FAIL] MATMUL incompatible shapes test\n";
        all_tests_passed = false;
    }
    if (test_matmul_requires_2d())
    {
        std::cout << "[PASS] MATMUL 2-D validation test\n";
    }
    else
    {
        std::cout << "[FAIL] MATMUL 2-D validation test\n";
        all_tests_passed = false;
    }
    if (test_matmul_model_loader())
    {
        std::cout << "[PASS] MATMUL model loader test\n";
    }
    else{
        std::cout << "[FAIL] MATMUL model loader test\n";
        all_tests_passed = false;
    }
    return all_tests_passed ? 0 : 1;

}