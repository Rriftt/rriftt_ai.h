* Same as `RAI_TENSOR_ALLOC()`, but fills the tensor with numbers drawn from a normal distribution of mean `mean` and standard deviation `std_dev`.

```c
#define RAI_TENSOR_ALLOC_RANDN(arena, mean, std_dev, ...) rai__tensor_alloc_randn(arena, mean, std_dev, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
```
