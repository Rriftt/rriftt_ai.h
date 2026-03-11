#define RAI_IMPLEMENTATION
#include "../rriftt_ai.h"

#include <stdio.h>
#include <stdint.h>
#include <stdalign.h>

int main()
{
	size_t cap = 1024 * 1024; // 1MB
	RaiArena arena = rai_arena_create(cap);

	RAI_ASSERT(arena.memory_begin != nullptr);
	RAI_ASSERT(arena.capacity_bytes >= cap);
	RAI_ASSERT(arena.alloced_bytes == 0);

	void* p1 = rai_arena_alloc(&arena, 13);
	void* p2 = rai_arena_alloc(&arena, 7);
	void* p3 = rai_arena_alloc(&arena, 64);

	RAI_ASSERT((uintptr_t)p1 % alignof(max_align_t) == 0);
	RAI_ASSERT((uintptr_t)p2 % alignof(max_align_t) == 0);
	RAI_ASSERT((uintptr_t)p3 % alignof(max_align_t) == 0);

	RAI_ASSERT((uint8_t*)p2 - (uint8_t*)p1 >= 13);
	RAI_ASSERT((uint8_t*)p3 - (uint8_t*)p2 >= 7);

	rai_arena_clear(&arena);
	RAI_ASSERT(arena.alloced_bytes == 0);
	RAI_ASSERT(arena.capacity_bytes >= cap);

	rai_arena_destroy(&arena);
	RAI_ASSERT(arena.memory_begin == nullptr);
	RAI_ASSERT(arena.capacity_bytes == 0);
	RAI_ASSERT(arena.alloced_bytes == 0);

	printf("test_arena: OK\n");
	return 0;
}
