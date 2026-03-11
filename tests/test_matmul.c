#define RAI_IMPLEMENTATION
#include "../rriftt_ai.h"

#include <stdio.h>

int main()
{
	RaiArena arena = rai_arena_create(1024 * 1024);

	// Standard 2D Matmul & Gradient
	// A: [1, 2]
	//    [3, 4]
	RaiTensor a = RAI_TENSOR_ALLOC(&arena, 2, 2);
	a.data[0] = 1.0f;
	a.data[1] = 2.0f;
	a.data[2] = 3.0f;
	a.data[3] = 4.0f;

	// B: [5, 6]
	//    [7, 8]
	RaiTensor b = RAI_TENSOR_ALLOC(&arena, 2, 2);
	b.data[0] = 5.0f;
	b.data[1] = 6.0f;
	b.data[2] = 7.0f;
	b.data[3] = 8.0f;

	// Forward: C = A @ B.T
	RaiTensor c = rai_tensor_matmul_t(&arena, a, b);
	RAI_ASSERT(c.rank == 2);
	RAI_ASSERT(c.data[0] == 17.0f);
	RAI_ASSERT(c.data[1] == 23.0f);
	RAI_ASSERT(c.data[2] == 39.0f);
	RAI_ASSERT(c.data[3] == 53.0f);

	// Backward: Assuming dC is all 1s
	RaiTensor dc = RAI_TENSOR_ALLOC_FILL(&arena, 1.0f, 2, 2);
	RaiTensorBinOpGrad grad = rai_tensor_matmul_t_grad(&arena, &arena, dc, a, b);

	// dA = dC @ B
	RAI_ASSERT(grad.d_a.data[0] == 12.0f);
	RAI_ASSERT(grad.d_a.data[1] == 14.0f);
	RAI_ASSERT(grad.d_a.data[2] == 12.0f);
	RAI_ASSERT(grad.d_a.data[3] == 14.0f);

	// dB = dC.T @ A
	RAI_ASSERT(grad.d_b.data[0] == 4.0f);
	RAI_ASSERT(grad.d_b.data[1] == 6.0f);
	RAI_ASSERT(grad.d_b.data[2] == 4.0f);
	RAI_ASSERT(grad.d_b.data[3] == 6.0f);

	// Broadcasted Accumulation Test
	// Simulating a Linear Layer: A[Batch=2, Seq=3, Dim=4], B[OutF=5, Dim=4]
	RaiTensor a3d = RAI_TENSOR_ALLOC_FILL(&arena, 1.0f, 2, 3, 4);
	RaiTensor b2d = RAI_TENSOR_ALLOC_FILL(&arena, 2.0f, 5, 4);

	RaiTensor c3d = rai_tensor_matmul_t(&arena, a3d, b2d);
	RAI_ASSERT(c3d.rank == 3);
	RAI_ASSERT(c3d.dims[RAI__TENSOR_MAXRANK - 3] == 2);
	RAI_ASSERT(c3d.dims[RAI__TENSOR_MAXRANK - 1] == 5);

	RaiTensor dc3d = RAI_TENSOR_ALLOC_FILL(&arena, 1.0f, 2, 3, 5);
	RaiTensorBinOpGrad grad3d = rai_tensor_matmul_t_grad(&arena, &arena, dc3d, a3d, b2d);

	RAI_ASSERT(grad3d.d_a.rank == 3);
	RAI_ASSERT(grad3d.d_b.rank == 2); // Natively preserved its 2D shape

	// Since dC=1 and A=1, the gradient for B is the accumulation of 1s over
	// Batch(2) * Seq(3) * Dim(4). That means each element in dB should equal 2 * 3 * 1 = 6.0f
	RAI_ASSERT(grad3d.d_b.data[0] == 6.0f);

	rai_arena_destroy(&arena);
	printf("test_matmul: OK\n");
	return 0;
}
