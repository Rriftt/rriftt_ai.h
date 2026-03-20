* Reshapes the given tensor `t` to the shape of the tensor `like_t`.
* This is a view operation. No memory is copied.
* Fails via `RAI_ASSERT()` if number of underlying scalars mismatch.

```c
#define RAI_TENSOR_RESHAPE_LIKE(t, like_t) rai__tensor_reshape(t, (like_t).rank, (like_t).dims + RAI__TENSOR_MAXRANK - (like_t).rank)
```
