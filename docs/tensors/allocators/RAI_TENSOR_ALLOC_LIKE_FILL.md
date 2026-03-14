Allocates a tensor of the same shape as `t` in the given `arena`. Then fills it with `fill`.

```c
#define RAI_TENSOR_ALLOC_LIKE_FILL(arena, t, fill, ...) rai__tensor_alloc_fill(arena, fill, (t).rank, (t).dims + RAI__TENSOR_MAXRANK - (t).rank)
```
