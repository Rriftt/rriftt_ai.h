#define RRIFTT_AI_IMPLEMENTATION
#include "../rriftt_ai.h"

#include <time.h>

int main()
{
	RAI_SRAND((unsigned int)time(nullptr));

	RaiArena arena = rai_arena_create(1024 * 1024 * 1);
	RaiArena scratch = rai_arena_create(1024 * 1024);
#if 0
	RaiMlpLayer layer = rai_mlp_layer_alloc_randn(&arena, 1024, 2048);

	RaiTensor in = RAI_TENSOR_ALLOC_RANDN(&arena, 0.0f, 1.0f, 1, 1024);
	RaiMlpLayerActs acts = rai_mlp_layer_forward(&scratch, layer, in);

	float mean = rai_tensor_mean(acts.prod);
	float std_dev = rai_tensor_std_dev(acts.prod);
	printf("prod : mean = %f, std_dev = %f\n", mean, std_dev);

	mean = rai_tensor_mean(acts.sum);
	std_dev = rai_tensor_std_dev(acts.sum);
	printf("sum  : mean = %f, std_dev = %f\n", mean, std_dev);

	mean = rai_tensor_mean(acts.out);
	std_dev = rai_tensor_std_dev(acts.out);
	printf("out  : mean = %f, std_dev = %f\n", mean, std_dev);
#else
	RaiMlp mlp = RAI_MLP_ALLOC_RANDN(&arena, 2, 2, 2);
	RaiTensor in = RAI_TENSOR_ALLOC_RANDN(&arena, 0.0f, 1.0f, 4, 2);
	RaiTensor target = RAI_TENSOR_ALLOC_RANDN(&scratch, 0.0f, 1.0f, 4, 2);

	float lr = 0.0001f;
	size_t max_steps = 10;
	for (size_t step = 0; step < max_steps; ++step) {
		// Forward Prop
		RaiMlpActs acts = rai_mlp_forward(&scratch, mlp, in);
#if 0
	for (size_t i = 0; i < acts.num_layers; ++i) {
		float mean = rai_tensor_mean(acts.layers[i].prod);
		float std_dev = rai_tensor_std_dev(acts.layers[i].prod);
		printf("prod : mean = %f, std_dev = %f\n", mean, std_dev);

		mean = rai_tensor_mean(acts.layers[i].sum);
		std_dev = rai_tensor_std_dev(acts.layers[i].sum);
		printf("sum  : mean = %f, std_dev = %f\n", mean, std_dev);

		mean = rai_tensor_mean(acts.layers[i].out);
		std_dev = rai_tensor_std_dev(acts.layers[i].out);
		printf("out  : mean = %f, std_dev = %f\n", mean, std_dev);
	}
#endif
		// Output and Error
		RaiTensor output = acts.layers[mlp.num_layers - 1].out;
		float error = rai_tensor_mse(output, target);
		printf("step = %4zu, error = %f\n", step, error);

		// Backward Prop
		RaiTensor d_out = rai_tensor_mse_grad(&scratch, output, target);
		RaiMlpGrads grads = rai_mlp_backward(&scratch, &scratch, mlp, acts, d_out, in);
#if 0
	for (size_t i = 0; i < grads.num_layers; ++i) {
		float mean = rai_tensor_mean(grads.layers[i].d_bias);
		float std_dev = rai_tensor_std_dev(grads.layers[i].d_bias);
		printf("d_bias   : mean = %f, std_dev = %f\n", mean, std_dev);

		mean = rai_tensor_mean(grads.layers[i].d_weight);
		std_dev = rai_tensor_std_dev(grads.layers[i].d_weight);
		printf("d_weight : mean = %f, std_dev = %f\n", mean, std_dev);

		mean = rai_tensor_mean(grads.layers[i].d_in);
		std_dev = rai_tensor_std_dev(grads.layers[i].d_in);
		printf("d_in     : mean = %f, std_dev = %f\n", mean, std_dev);
	}
#endif
		// Update
		rai_mlp_sgd(&scratch, &mlp, grads, lr);
		rai_arena_clear(&scratch);
	}
#endif
	rai_arena_destroy(&arena);
	rai_arena_destroy(&scratch);
	return 0;
}
