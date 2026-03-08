#include <time.h>
#define RRIFTT_AI_IMPLEMENTATION
#include "../rriftt_ai.h"

int main()
{
	RAI_SRAND((unsigned int)time(nullptr));

	// Request 1 KB permanent arena from kernel
	RaiArena perm_arena = rai_arena_create(1024);

	// Request 1 KB scratch arena from kernel
	RaiArena scratch_arena = rai_arena_create(1024);

	// Shapes
	size_t B = 4;
	size_t S = 1;
	size_t D = 2;

	// Inputs
	RaiTensor input = RAI_TENSOR_ALLOC(&perm_arena, B, S, D);
	input.data[0] = 0.0f, input.data[1] = 0.0f;
	input.data[2] = 0.0f, input.data[3] = 1.0f;
	input.data[4] = 1.0f, input.data[5] = 0.0f;
	input.data[6] = 1.0f, input.data[7] = 1.0f;

	// Targets
	RaiTensor target = RAI_TENSOR_ALLOC(&perm_arena, B, S, D);
	target.data[0] = 1.0f, target.data[1] = 0.0f;
	target.data[2] = 0.0f, target.data[3] = 1.0f;
	target.data[4] = 0.0f, target.data[5] = 1.0f;
	target.data[6] = 1.0f, target.data[7] = 0.0f;

	// Hyperparams
	float eta = 0.1;
	size_t max_steps = 10'000;

	// MLP
	RaiMlp mlp = RAI_MLP_ALLOC_RANDN(&perm_arena, 2, 2, 2);
	for (size_t step = 0; step < max_steps; ++step) {
		// Forward Prop
		RaiMlpActs acts = rai_mlp_forward(&scratch_arena, mlp, input);

		// Predictions
		RaiTensor logits = acts.layers[acts.num_layers - 1].out;
		RaiTensor preds = rai_tensor_softmax(&scratch_arena, logits);

		if (step == 0 || step == max_steps - 1) {
			RAI_TENSOR_PRINT(preds);
		}

		// Error Calc
		// float error = rai_tensor_mse(preds, target); // scalar
		// printf("step = %4zu, error = %f\n", step, error);

		// BackProp
		RaiTensor d_preds = rai_tensor_mse_grad(&scratch_arena, preds, target);
		RaiTensor d_logits = rai_tensor_softmax_grad(&scratch_arena, d_preds, preds);
		RaiMlpGrads grads = rai_mlp_backward(&scratch_arena, &scratch_arena, mlp, acts, d_logits, input);

		// Update
		rai_mlp_sgd(&scratch_arena, &mlp, grads, eta);
		rai_arena_clear(&scratch_arena);
	}

	return 0;
}
