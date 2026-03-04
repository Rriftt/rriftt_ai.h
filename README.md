# rriftt_ai.h

**A bare-metal, zero-dependency C23 neural network engine.**

Modern deep learning is suffocating under layers of Python wrappers, 10GB toolchains, and CMake nightmares. `rriftt_ai.h` is a brutalist correction to the ecosystem. It is a single-header C library for building, training, and running Transformer models from scratch.

No build systems. No external BLAS libraries. No hidden `malloc` calls during execution. Just drop the header into your project and compile.

## The Visual Proof

You do not need a supercomputer to allocate a tensor. Here is a fully functional AI memory arena and tensor operation in under 20 lines:

```c
#define RRIFTT_AI_IMPLEMENTATION
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

Compile it with a standard compiler. No flags required other than linking the math library:
```bash
gcc main.c -o engine -lm
./engine
```

## Architecture

* **Arena Allocation (`RaiArena`):** Total control over memory. `rriftt_ai.h` never calls `malloc` or `free` during forward or backward passes. You pass it an arena, and it operates strictly within that perimeter.
* **C23 Strict:** Mathematically tight, modern C architecture.
* **The Full Transformer Stack:** Natively implements RoPE, RMSNorm, SwiGLU, and Scaled Dot-Product Attention without external dependencies.
* **Training Engine Natively Included:** Full backpropagation routines, Cross-Entropy loss, and AdamW optimizer states are built directly into the C structs.
* **Native BPE Tokenizer:** Train and execute Byte-Pair Encoding directly in C.

## Integration

Drop `rriftt_ai.h` into your directory. In **exactly one** C file, define the implementation macro before including:

```c
#define RRIFTT_AI_IMPLEMENTATION
#include "rriftt_ai.h"
```

## Active Development

This engine is a living architecture. It is under active development, and the foundation is built to scale. Pull requests, hardware-specific optimizations, and new feature implementations are heavily encouraged and actively reviewed. 

## License

Public Domain (Unlicense) or MIT. Choose whichever fits your system.
