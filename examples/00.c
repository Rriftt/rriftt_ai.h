#define RRIFTT_AI_IMPLEMENTATION
#include "../rriftt_ai.h"

float mse(RaiTensor preds, RaiTensor targets)
{
	RAI_ASSERT(preds.count == targets.count && "Dimention Mismatch");
	float loss = 0.0f;
	for (size_t i = 0; i < preds.count; ++i) {
		float ei = preds.data[i] - targets.data[i];
		loss += ei * ei;
	}

	return loss / (float)preds.count;
}

RaiTensor mse_grad(RaiArena* arena, RaiTensor preds, RaiTensor targets)
{
	RAI_ASSERT(preds.count == targets.count && "Dimention Mismatch");
	RaiTensor d_preds = RAI_TENSOR_ALLOC_LIKE(arena, preds);
	float N = (float)preds.count;
	for (size_t i = 0; i < preds.count; ++i) {
		float ei = preds.data[i] - targets.data[i];
		d_preds.data[i] = 2 * ei / N;
	}

	return d_preds;
}

int main()
{
	// Request 1 KB permanent arena from kernel
	RaiArena perm_arena = rai_arena_create(1024);

	// Request 1 KB scratch arena from kernel
	RaiArena scratch_arena = rai_arena_create(1024);

	// Shapes
	size_t B = 4;
	size_t S = 1;
	size_t D = 2;

	// Inputs
	RaiTensor X = RAI_TENSOR_ALLOC(&perm_arena, B, S, D);
	X.data[0] = 0.0f, X.data[1] = 0.0f;
	X.data[2] = 0.0f, X.data[3] = 1.0f;
	X.data[4] = 1.0f, X.data[5] = 0.0f;
	X.data[6] = 1.0f, X.data[7] = 1.0f;

	// Targets
	RaiTensor Y = RAI_TENSOR_ALLOC(&perm_arena, B, S, D);
	Y.data[0] = 1.0f, Y.data[1] = 0.0f;
	Y.data[2] = 0.0f, Y.data[3] = 1.0f;
	Y.data[4] = 0.0f, Y.data[5] = 1.0f;
	Y.data[6] = 0.0f, Y.data[7] = 1.0f;

	// Normal Distribution
	float mean = 0.0f;
	float std_dev = 1.0f;

	// Hyperparams
	float eta = 0.1;
	size_t max_steps = 10000;

	// Weights & Biases
	RaiTensor W0 = RAI_TENSOR_ALLOC_RANDN(&perm_arena, mean, std_dev, D, D); // [D, D]
	RaiTensor B0 = RAI_TENSOR_ALLOC_RANDN(&perm_arena, mean, std_dev, D);	 // [D]

	for (size_t step = 0; step < max_steps; ++step) {
		// Forward Prop
		RaiTensor XWT = rai_tensor_matmul_t(&scratch_arena, X, W0); // [B, S, D]
		RaiTensor A = rai_tensor_add(&scratch_arena, XWT, B0);	    // [B, S, D]
		RaiTensor P = rai_tensor_softmax(&scratch_arena, A);	    // [B, S, D]

		if (step == 0 || step == max_steps - 1) {
			RAI_TENSOR_PRINT(P);
		}

		// Error Calc
		float error = mse(P, Y); // scalar
		// printf("step = %4zu, error = %f\n", step, error);

		// BackProp
		RaiTensor dP = mse_grad(&scratch_arena, P, Y); // [B, S, D]
		RaiTensor dA = rai_tensor_softmax_grad(&scratch_arena, dP, P);
		RaiTensorBinOpGrad A_grad = rai_tensor_add_grad(&scratch_arena, &scratch_arena, dA, XWT, B0);
		RaiTensor dXWT = A_grad.d_a;
		RaiTensor dB0 = A_grad.d_b;
		RaiTensorBinOpGrad XWT_grad = rai_tensor_matmul_t_grad(&scratch_arena, &scratch_arena, dXWT, X, W0);
		RaiTensor dW0 = XWT_grad.d_b;

		// Update
		RaiTensor eta_dW0 = rai_tensor_scale(&scratch_arena, dW0, -eta);
		RaiTensor eta_dB0 = rai_tensor_scale(&scratch_arena, dB0, -eta);
		rai__tensor_add(W0, W0, eta_dW0);
		rai__tensor_add(B0, B0, eta_dB0);

		rai_arena_clear(&scratch_arena);
	}

	return 0;
}
