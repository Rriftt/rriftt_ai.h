* Reshapes the given tensor the given shape, without copying the underlying data.
* Usage: If t is a $$2$$x$$3$$ matrix, `RAI_TENSOR_RESHAPE(t, 6)` will **view** it as a vector of dimention $$6$$.
* Fails via `RAI_ASSERT()` if number of underlying scalars mismatch.

```c
#define RAI_TENSOR_RESHAPE(t, ...) rai__tensor_reshape(t, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
```
