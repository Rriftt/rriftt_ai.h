# rriftt_ai.h

**A public-domain single-header, zero-dependency C23 AI engine.**

Modern deep learning is suffocating under layers of Python wrappers, 10GB toolchains, and CMake nightmares. `rriftt_ai.h` is a brutalist correction to the ecosystem. It is a single-header C library for building, training, and running AI models from scratch.

## Single-Header

`rriftt_ai.h` takes the concept of the legendary `nothings/stb` and `miniaudio.h` single-header libraries and pushes it to its absolute limit. In one header file it includes:
* this README
* LICENSE
* Usage Guides
* Documentation
* Public API
* Examples
* Private Implementation
* Tests

The best documentation is the header itself. However, for convenience, standard documentation files are provided — these are automatically extracted directly from the header using the included `build.sh`.

## Integration

No build systems. No external BLAS libraries. No hidden `malloc` calls during execution. Just drop `rriftt_ai.h` into your directory. In **exactly one** C file, define the implementation macro before including:

```c
#define RAI_IMPLEMENTATION
#include "rriftt_ai.h"
```

## Quick Start

You do not need a supercomputer to do tensor math. Here is a fully functional AI memory arena and tensor operation in under 20 lines:

```c
#define RAI_IMPLEMENTATION
#include "rriftt_ai.h"

int main(void) {
    // 1. Boot the memory arena (1MB)
    RaiArena arena = rai_arena_create(1024 * 1024);

    // 2. Allocate and fill two 2x2 tensors
    RaiTensor A = RAI_TENSOR_ALLOC_FILL(&arena, 2.0f, 2, 2);
    RaiTensor B = RAI_TENSOR_ALLOC_FILL(&arena, 3.0f, 2, 2);

    // 3. Execute math
    RaiTensor C = rai_tensor_add(&arena, A, B);

    // 4. Verify
    RAI_TENSOR_PRINT(C);

    // 5. Vaporize all memory instantly
    rai_arena_destroy(&arena);
    return 0;
}
```

Compile it with a standard compiler. No flags required other than linking the math library (which, if desired, can be stripped off using the built-in math function overrides):

```bash
gcc main.c -o engine -lm
./engine
```

## Architecture

* **C23 Strict:** Mathematically tight, modern C architecture.
* **Fully Configurable**: Every standard library dependency can be stripped off using the built-in configuration macros, making it fully compatible with a platform without a host operating system.
* **Arena Allocation (`RaiArena`):** Total control over memory. `rriftt_ai.h` never calls `malloc` or `free` during forward or backward passes. You pass it an arena, and it operates strictly within that perimeter.
* **The Full Transformer Stack:** Natively implements RoPE, RMSNorm, SwiGLU, and Scaled Dot-Product Attention without external dependencies.
* **Modular Structure**: `rriftt_ai.h` is written with modularity in mind. Each built-in AI module is written using simpler sub-modules.
* **The Full MLP stack**: Natively supports a configurable MLP for quick classification problems/teaching purposes.
* **Training Engine Natively Included:** Full backpropagation routines, Cross-Entropy loss, and AdamW optimizer states are built directly into the C structs.
* **Native BPE Tokenizer:** Train and execute Byte-Pair Encoding directly in C.

## Rolling Release Model

The project is based on a **bleeding-edge rolling-release** model. Please use `rriftt_ai.h` from the latest commit.

This engine is a living architecture. It is under active development, and the foundation is built to scale. Pull requests, hardware-specific optimizations, and new feature implementations are heavily encouraged and actively reviewed. 

While the API is subject to change, the core engine is heavily tested before every push.

## License

Public Domain (Unlicense) or MIT. Choose whichever fits your system.

