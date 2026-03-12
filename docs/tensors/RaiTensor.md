```c
#define RAI__TENSOR_MAXRANK 8
typedef struct {
	size_t rank;
	size_t dims[RAI__TENSOR_MAXRANK];
	size_t strs[RAI__TENSOR_MAXRANK];
	size_t count;
	float* data;
} RaiTensor;
```
 * `RaiTensor` is a row-major **view** into a contiguous block of memory used to store the `float` scalars.
 * `rank` : rank of the tensor in question. 0 for scalar, 1 for vector, 2 for matrix and so on.
 * `dims` : the right-aligned dimensions of the tensor.
 * `strs` : the right-aligned strides across corresponding dimension.
 * `count`: number of scalars in the tensor
 * `data` : points to the beginning of the contiguous chunk of memory.
