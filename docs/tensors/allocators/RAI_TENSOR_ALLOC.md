* Allocates a tesor of given shape in the given `arena`.
* Usage: `RAI_TENSOR_ALLOC(arena, 2, 3)` to allocate a $$2$$x$$3$$ matrix.
* Not providing any shape is well defined. In that case it allocates a scalar.

```c
#define RAI_TENSOR_ALLOC(arena, ...) rai__tensor_alloc(arena, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
```
