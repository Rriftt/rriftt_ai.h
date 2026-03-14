* Allocates a tensor of the same shape as `t` in the given `arena`.

```c
#define RAI_TENSOR_ALLOC_LIKE(arena, t) rai__tensor_alloc(arena, (t).rank, (t).dims + RAI__TENSOR_MAXRANK - (t).rank)
```
