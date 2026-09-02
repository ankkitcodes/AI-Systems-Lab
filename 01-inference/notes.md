# 01-inference — Notes

## Named tensors and nodes (current step)

Tensors and nodes now carry **names** so the model can describe wiring like a real
computation graph (similar to ONNX):

- `Tensor("input", {1, -2, 3})` — data plus a tensor name
- `Node(MULTIPLY, "input", "hidden", 2.0f)` — op reads `"input"`, writes `"hidden"`
- `Node(RELU, "hidden", "output")` — op reads `"hidden"`, writes `"output"`

Example flow:

```
"input" [1, -2, 3]  →  MULTIPLY  →  "hidden" [2, -4, 6]  →  RELU  →  "output" [2, 0, 6]
```

## What ExecutionEngine still does

`ExecutionEngine::execute(model, input)` still runs nodes **in list order** and pipes
the output of one step into the next. It does **not** yet:

- look up tensors by `node.input_name()`
- store intermediate results under `node.output_name()`
- support branching or multiple inputs

Names are part of the **model description** today; graph-aware execution comes next.

## Runtime

`Runtime` exists and delegates to `ExecutionEngine`, but `main.cpp` currently calls
the engine directly while graph naming is being added. Runtime will be wired back in
once loading and named execution are in place.
