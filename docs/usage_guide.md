# Overview

`rriftt_ai.h` is a dependency-free, single-header C library for building, training,
and running neural networks from scratch. It requires absolutely nothing but
a standard C compiler to work. Everything else is in the source.

Do this:

```c
#define RRIFTT_AI_IMPLEMENTATION
```
before you include this file in *one* C file to create the implementation. i.e. it should look like this:

```c
#include ...
#include ...
#define RRIFTT_AI_IMPLEMENTATION
#include "rriftt_ai.h"
```

## Public And Private API

You can safely interface with any Type, Macro, or Function starting with 
`Rai`, `RAI_`, or `rai_`. These constitute the public API.

**DANGER**: Avoid using anything starting with `RAI__` or `rai__` (two underscores). 
These are strictly private internal functions and macros. Their behavior, 
memory assumptions, and signatures will change without notice.

## Naming Schemes

To prevent namespace collisions and maintain strict C readability, this 
library adheres to the following conventions:

- **Types (Structs/Enums)**: `PascalCase` with `Rai` prefix (e.g., `RaiTensor`, `RaiArena`)
- **Functions**: `snake_case` with `rai_` prefix (e.g., `rai_tensor_add`)
- **Macros**: `SCREAMING_SNAKE_CASE` with `RAI_` prefix(e.g., `RAI_ASSERT`) 
