Arena Allocator / Linear Allocator / Bump Allocator / Region Allocator
Used for requesting a chunk of memory using `RAI_MALLOC()` and allocating linearly inside it.
`rriftt_ai.h` uses `RaiArena` for every allocation. A function/macro that allocates memory can be readily identified from the fact that it will accept `RaiArena *` as its first parameter.

```c
typedef struct {
	size_t alloced_bytes;
	size_t capacity_bytes;
	void* memory_begin;
} RaiArena;
```
