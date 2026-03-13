```c
#define RAI_TENSOR_PRINT(t) rai__tensor_print(t, 0)
```
 * Pretty-prints the tensor to the console.
 * Depends on `<stdio.h>`.
 * Use for debugging purposes. Do not use for large tensors because that will explode in your console.
