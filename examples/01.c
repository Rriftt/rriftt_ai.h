#define RRIFTT_AI_IMPLEMENTATION
#include "../rriftt_ai.h"
#include <stdio.h>

int main()
{
	RaiString text = { 11, "HELLO WORLD" };

	RaiArena perm = rai_arena_create(1024 * 1024 * 10);
	RaiArena s_acts = rai_arena_create(1024 * 1024 * 10);
	RaiArena s_grads = rai_arena_create(1024 * 1024 * 10);

	RaiTokenizer tok = rai_tokenizer_train(&perm, &s_acts, text, 260);
	RaiTokenArray tokens = rai_tokenizer_encode(&perm, tok, text);

	size_t seq_len = tokens.count - 1;
	int* inputs = tokens.data;
	int* targets = tokens.data + 1;

	RaiModelConfig mcfg = {
		.vocab_size = 260,
		.embedding_length = 16,
		.attention_head_count = 2,
		.feed_forward_length = 32,
		.block_count = 1,
		.attention_layer_norm_rms_epsilon = 1e-5f,
		.rope_freq_base = 10000.0f
	};

	RaiAdamWConfig acfg = {
		.lr = 0.05f,
		.beta1 = 0.9f,
		.beta2 = 0.999f,
		.eps = 1e-8f,
		.weight_decay = 0.01f
	};

	RaiEngine engine = rai_engine_create(1024 * 1024 * 10, mcfg, acfg);

	for (int i = 0; i < 50; ++i) {
		rai_arena_clear(&s_acts);
		rai_arena_clear(&s_grads);

		RaiModelActs acts = rai_engine_forward(&s_acts, engine, 1, seq_len, inputs, 0);
		float loss = rai_cross_entropy(acts.output, 1, seq_len, targets);

		RaiTensor d_logits = rai_cross_entropy_grad(&s_acts, acts.output, 1, seq_len, targets);
		RaiModel grads = rai_engine_backward(&s_acts, &s_grads, engine, acts, d_logits, 1, seq_len, inputs, 0);

		rai_engine_step(&engine, grads);

		printf("step %d loss %f\n", i, loss);
	}

	rai_engine_destroy(&engine);
	rai_arena_destroy(&perm);
	rai_arena_destroy(&s_acts);
	rai_arena_destroy(&s_grads);

	return 0;
}
