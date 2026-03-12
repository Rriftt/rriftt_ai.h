```c
void* rai_arena_alloc(RaiArena* arena, size_t size_bytes);
```
 * Allocates `size_bytes` within `arena` (aligned to `max_align_t`).
 * Fails with `RAI_ASSERT()` if `arena` is out of memory.
 * Allocated memory is **not** zeroed out.
