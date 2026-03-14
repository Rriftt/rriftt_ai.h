* Allocates an empty arena with `capacity_bytes` of capacity (aligned to `max_align_t`).
* Only function in this library that uses `RAI_MALLOC()`.
* Fails with `RAI_ASSERT()` if `RAI_MALLOC()` fails.
* Allocated arena can be freed with `rai_arena_destroy()`.

```c
RaiArena rai_arena_create(size_t capacity_bytes);
```
