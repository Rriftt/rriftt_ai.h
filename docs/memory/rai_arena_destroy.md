Frees arena `arena` rendering all allocations within it unusable.
Only function in this library that uses `RAI_FREE()`.
Zeroes out the `arena` struct to indicate it has been destroyed.

```c
void rai_arena_destroy(RaiArena* arena);
```
