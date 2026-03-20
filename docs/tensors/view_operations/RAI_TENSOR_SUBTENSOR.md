* Returns a handle to the subtensor of the given tensor at the specified index (or indeces).
* This is a view operation. No memory is copied.
* Usage: `RAI_TENSOR_SUBTENSOR(t, 2, 1)` applied on a $$3$$x$$2$$x$$2$$ tensor returns a vector of shape $$2$$ at index (2, 1) of the tensor `t`.
* Fails via `RAI_ASSERT()` in case of index being out of bounds.

```c
#define RAI_TENSOR_SUBTENSOR(t, ...) rai__tensor_subtensor(t, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
```
