#define RAI_IMPLEMENTATION
#include "../rriftt_ai.h"

#include <stdio.h>

int main()
{
	RaiArena arena = rai_arena_create(1024 * 1024);

	// Allocation & Initialization
	RaiTensor t1 = RAI_TENSOR_ALLOC_FILL(&arena, 5.0f, 2, 3);
	RAI_ASSERT(t1.rank == 2);
	RAI_ASSERT(t1.count == 6);
	RAI_ASSERT(t1.dims[RAI__TENSOR_MAXRANK - 2] == 2);
	RAI_ASSERT(t1.dims[RAI__TENSOR_MAXRANK - 1] == 3);
	RAI_ASSERT(t1.strs[RAI__TENSOR_MAXRANK - 2] == 3);
	RAI_ASSERT(t1.strs[RAI__TENSOR_MAXRANK - 1] == 1);
	for (size_t i = 0; i < t1.count; ++i) {
		RAI_ASSERT(t1.data[i] == 5.0f);
	}

	// Reshape
	RaiTensor t2 = RAI_TENSOR_RESHAPE(t1, 3, 2);
	RAI_ASSERT(t2.rank == 2);
	RAI_ASSERT(t2.count == 6);
	RAI_ASSERT(t2.dims[RAI__TENSOR_MAXRANK - 2] == 3);
	RAI_ASSERT(t2.dims[RAI__TENSOR_MAXRANK - 1] == 2);
	RAI_ASSERT(t2.strs[RAI__TENSOR_MAXRANK - 2] == 2);
	RAI_ASSERT(t2.data == t1.data); // Must point to same memory

	// Subtensor (Slicing)
	t1.data[4] = 9.0f;			     // Modify row 1, col 1
	RaiTensor sub = RAI_TENSOR_SUBTENSOR(t1, 1); // Slice the second row
	RAI_ASSERT(sub.rank == 1);
	RAI_ASSERT(sub.count == 3);
	RAI_ASSERT(sub.dims[RAI__TENSOR_MAXRANK - 1] == 3);
	RAI_ASSERT(sub.data[1] == 9.0f); // Ensure pointer offset is correct

	// Broadcasting Add
	RaiTensor a = RAI_TENSOR_ALLOC_FILL(&arena, 2.0f, 2, 3);
	RaiTensor b = RAI_TENSOR_ALLOC_FILL(&arena, 3.0f, 3); // Rank 1
	RaiTensor c = rai_tensor_add(&arena, a, b);	      // b should broadcast to [2, 3]

	RAI_ASSERT(c.rank == 2);
	RAI_ASSERT(c.count == 6);
	RAI_ASSERT(c.dims[RAI__TENSOR_MAXRANK - 2] == 2);
	RAI_ASSERT(c.dims[RAI__TENSOR_MAXRANK - 1] == 3);
	for (size_t i = 0; i < c.count; ++i) {
		RAI_ASSERT(c.data[i] == 5.0f);
	}

	rai_arena_destroy(&arena);
	printf("test_tensor: OK\n");
	return 0;
}
