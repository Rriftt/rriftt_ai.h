* Returns a transposed handle to the tensor `t`. The axes referred to by `axis_a` and `axis_b` are transposed.
* Fails via `RAI_ASSERT()` if the axes are out of bounds.

```c
RaiTensor rai_tensor_transpose(RaiTensor t, size_t axis_a, size_t axis_b);
```
