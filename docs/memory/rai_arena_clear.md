* Deallocates all the memory allocated by `rai_arena_alloc()`.
* Does not free the arena. Further calls to `rai_arena_alloc()` will overwrite previous allocations.

```c
void rai_arena_clear(RaiArena* arena);
```
