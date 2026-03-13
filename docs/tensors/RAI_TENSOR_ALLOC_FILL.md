```c
#define RAI_TENSOR_ALLOC_FILL(arena, fill, ...) rai__tensor_alloc_fill(arena, fill, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
```
 * Same as `RAI_TENSOR_ALLOC()`, but fills the tensor with number specified by `fill`.
