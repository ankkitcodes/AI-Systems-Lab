# 01-inference — Notes

## Current architecture

```
Runtime              ← exists; main still calls ExecutionEngine directly
    │
    ▼
ExecutionEngine      ← resolves tensors by name, runs each node
    │
    ▼
Model
├── input  → "input"
├── graph
│    ├── "input"  → Multiply ×2 → "hidden"
│    └── "hidden" → ReLU        → "output"
└── output → "output"
    │
    ▼
Tensor               ← named float data flowing through the graph
```

## Named graph execution (done)

Tensors and nodes carry **names** so the model describes wiring like a real
computation graph (similar to ONNX):

- `Tensor("input", {1, -2, 3})` — data plus a tensor name
- `Node(MULTIPLY, "input", "hidden", 2.0f)` — op reads `"input"`, writes `"hidden"`
- `Node(RELU, "hidden", "output")` — op reads `"hidden"`, writes `"output"`
- `Model.set_input("input")` / `set_output("output")` — explicit model I/O boundary

Example flow:

```
[1, -2, 3]
     ↓  ×2
[2, -4, 6]
     ↓  ReLU
[2, 0, 6]
```

`ExecutionEngine::execute(model, input)` now:

- validates that `input.name()` matches `model.input_name()`
- stores tensors in a map keyed by name
- for each node, looks up `node.input_name()`, runs the op, stores under `node.output_name()`
- returns the tensor at `model.output_name()`

Uses `insert_or_assign` (not `map[key] =`) because `Tensor` has no default constructor.

## What main still does manually

`main.cpp` builds the full model in code — it acts as a stand-in for **ModelLoader**:

```cpp
model.set_input("input");
model.set_output("output");
model.add_node(...);
```

## Next step: ModelLoader

Load the graph from a file (e.g. JSON) instead of hardcoding in `main`:

```
model.json  →  ModelLoader  →  Model  →  Runtime.run(input)  →  output
```

After that, wire `main` through `Runtime` again and optionally add `Runtime::load(path)`.

## Runtime

`Runtime` exists and delegates to `ExecutionEngine`, but `main.cpp` currently calls
the engine directly. Will be wired back in once ModelLoader is in place.
