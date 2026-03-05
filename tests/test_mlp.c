#define RRIFTT_AI_IMPLEMENTATION
#include "../rriftt_ai.h"

int main()
{
	RaiArena arena = rai_arena_create(1024);
	RaiArena scratch = rai_arena_create(1024);

	RaiMlpLayer layer = rai_mlp_layer_alloc_randn(&arena, 4, 2);

	RaiTensor in = RAI_TENSOR_ALLOC_RANDN(&arena, 0.0f, 1.0f, 1, 4);
	RaiMlpLayerActs acts = rai_mlp_layer_forward(&scratch, layer, in);

	RaiTensor d_out = RAI_TENSOR_ALLOC_FILL(&scratch, 1.0f, 1, 2);
	RaiMlpLayerGrad grad = rai_mlp_layer_backward(&arena, &scratch, layer, acts, d_out, in);

	RAI_TENSOR_PRINT(grad.d_weight);

	rai_arena_destroy(&arena);
	return 0;
}
