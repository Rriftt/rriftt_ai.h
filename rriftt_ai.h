/*
rriftt_ai.h - v1.0 - public domain bare-metal AI library
requires C23 or higher. no warranty implied; use at your own risk.

============================================================================
OVERVIEW
============================================================================
rriftt_ai is a dependency-free, single-header C library for building, training,
and running neural networks from scratch. It requires absolutely nothing but
a standard C compiler to work. Everything else is in the source.

Do this:
#define RRIFTT_AI_IMPLEMENTATION
before you include this file in *one* C file to create the implementation.

// i.e. it should look like this:
#include ...
#include ...
#define RRIFTT_AI_IMPLEMENTATION
#include "rriftt_ai.h"

============================================================================
GENERAL USAGE GUIDE
============================================================================
You can safely interface with any Type, Macro, or Function starting with 
Rai, RAI_, or rai_. These constitute the public API.

DANGER: Avoid using anything starting with RAI__ or rai__ (two underscores). 
These are strictly private internal functions and macros. Their behavior, 
memory assumptions, and signatures will change without notice.

============================================================================
NAMING SCHEMES
============================================================================
To prevent namespace collisions and maintain strict C readability, this 
library adheres to the following conventions:

- Types (Structs/Enums): PascalCase with 'Rai' prefix (e.g., RaiTensor, RaiArena)
- Functions: snake_case with 'rai_' prefix (e.g., rai_tensor_add)
- Macros: SCREAMING_SNAKE_CASE with 'RAI_' prefix (e.g., RAI_ASSERT)

============================================================================
LICENSE
============================================================================
This software is available under 2 licenses -- choose whichever you prefer.

----------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2026 Rriftt

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
----------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------

=============================================================================
QUICK START / EXAMPLE USAGE
=============================================================================

#define RRIFTT_AI_IMPLEMENTATION
#include "rriftt_ai.h"
#include <stdio.h>

int main() {
	// 1. Create a 1MB memory arena
	RaiArena arena = rai_arena_create(1024 * 1024);

	// 2. Allocate two 2x2 tensors and fill them
	RaiTensor A = RAI_TENSOR_ALLOC_FILL(&arena, 2.0f, 2, 2);
	RaiTensor B = RAI_TENSOR_ALLOC_FILL(&arena, 3.0f, 2, 2);

	// 3. Perform a mathematical operation
	RaiTensor C = rai_tensor_add(&arena, A, B);

	// 4. Print the result
	RAI_TENSOR_PRINT(C);

	// 5. Free all memory instantly
	rai_arena_destroy(&arena);
	return 0;
}

=============================================================================
*/

#ifndef RRIFTT_AI_H
#define RRIFTT_AI_H

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#error rriftt_ai.h requires at least C23. Please compile with -std=c2x or -std=c23.
#endif

#ifdef __cplusplus
#error rriftt_ai.h does NOT lower its standards.
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>

#ifndef RAI_PRINTF
#include <stdio.h>
#define RAI_PRINTF(fmt, ...) printf(fmt, __VA_ARGS__)
#endif // RAI_PRINTF

#ifndef RAI_MALLOC
#include <stdlib.h>
#define RAI_MALLOC(size) malloc(size)
#endif // RAI_MALLOC

#ifndef RAI_FREE
#include <stdlib.h>
#define RAI_FREE(ptr) free(ptr)
#endif // RAI_FREE

#ifndef RAI_MEMCPY
#include <string.h>
#define RAI_MEMCPY(dest, src, size) memcpy(dest, src, size)
#endif // RAI_MEMCPY

#ifndef RAI_MEMSET
#include <string.h>
#define RAI_MEMSET(dest, set, size) memset(dest, set, size)
#endif // RAI_MEMSET

#ifndef RAI_ASSERT
#include <assert.h>
#define RAI_ASSERT(cond) assert(cond)
#endif // RAI_ASSERT

#ifndef RAI_SQRT1_2F
#define RAI_SQRT1_2F 0.70710678118654752440f
#endif // RAI_SQRT1_2F

#ifndef RAI_PIF
#define RAI_PIF 3.14159265358979323846f
#endif // RAI_PIF

#ifndef RAI_ERFCF
#include <math.h>
#define RAI_ERFCF(x) erfcf(x)
#endif // RAI_ERFCF

#ifndef RAI_SQRTF
#include <math.h>
#define RAI_SQRTF(x) sqrtf(x)
#endif // RAI_SQRTF

#ifndef RAI_EXPF
#include <math.h>
#define RAI_EXPF(x) expf(x)
#endif // RAI_EXPF

#ifndef RAI_COSF
#include <math.h>
#define RAI_COSF(x) cosf(x)
#endif // RAI_COSF

#ifndef RAI_SINF
#include <math.h>
#define RAI_SINF(x) sinf(x)
#endif // RAI_SINF

#ifndef RAI_LOGF
#include <math.h>
#define RAI_LOGF(x) logf(x)
#endif // RAI_LOGF

#ifndef RAI_POWF
#include <math.h>
#define RAI_POWF(base, exp) powf(base, exp)
#endif // RAI_POWF

#ifndef RAI_RAND
#include <stdlib.h>
#define RAI_RAND() rand()
#endif // RAI_RAND

#ifndef RAI_RAND_MAX
#include <stdlib.h>
#define RAI_RAND_MAX RAND_MAX
#endif // RAI_RAND_MAX

#ifndef RAI_SRAND
#include <stdlib.h>
#define RAI_SRAND(seed) srand(seed)
#endif // RAI_SRAND

#ifndef RAI_INFINITY
#include <math.h>
#define RAI_INFINITY INFINITY
#endif // RAI_INFINITY

// ------------------------- Math API ---------------------------------
// PRNG - Gaussian Distribution
float rai_randn(float mean, float std_dev);

// ------------------------- Arena API ---------------------------------
// Arena Allocator
typedef struct {
	size_t alloced_bytes;
	size_t capacity_bytes;
	void *memory_begin;
} RaiArena;

RaiArena rai_arena_create(size_t capacity_bytes);
void rai_arena_destroy(RaiArena *arena);
void *rai_arena_alloc(RaiArena *arena, size_t size_bytes);
void rai_arena_clear(RaiArena *arena);

// ------------------------- Tensor API ---------------------------------
// Tensors - The Atoms of Deep Learning
#define RAI__TENSOR_MAXRANK 8
typedef struct {
	size_t rank;
	size_t dims[RAI__TENSOR_MAXRANK];
	size_t strs[RAI__TENSOR_MAXRANK];
	size_t count;
	float *data;
} RaiTensor;

// Tensor functions
// Loggers
void rai__tensor_print(RaiTensor t, int indent);

// Allocators
RaiTensor rai__tensor_alloc(RaiArena *arena, size_t rank, size_t dims[rank + 1]);
RaiTensor rai__tensor_alloc_randn(RaiArena *arena, float mean, float std_dev, size_t rank, size_t dims[rank + 1]);
RaiTensor rai__tensor_alloc_fill(RaiArena *arena, float fill, size_t rank, size_t dims[rank + 1]);

// View Operations
RaiTensor rai__tensor_reshape(RaiTensor t, size_t new_rank, size_t new_dims[new_rank + 1]);
RaiTensor rai__tensor_subtensor(RaiTensor t, size_t idxs_len, size_t idxs[idxs_len + 1]);

#define RAI__ARRAY_LEN(arr) ((sizeof(arr)) / (sizeof((arr)[0])))
#define RAI__NULL_TERMINATED_ARRAY(...) ((size_t[]){ __VA_OPT__(__VA_ARGS__, ) 0 })
#define RAI__NULL_TERMINATED_ARRAY_LEN(...) (RAI__ARRAY_LEN(RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__)) - 1)

// Loggers
void rai_tensor_info(RaiTensor t);
#define RAI_TENSOR_PRINT(t) rai__tensor_print(t, 0)

// Allocators
#define RAI_TENSOR_ALLOC(arena, ...) rai__tensor_alloc(arena, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
#define RAI_TENSOR_ALLOC_LIKE(arena, t) rai__tensor_alloc(arena, (t).rank, (t).dims + RAI__TENSOR_MAXRANK - (t).rank)
#define RAI_TENSOR_ALLOC_RANDN(arena, mean, std_dev, ...) rai__tensor_alloc_randn(arena, mean, std_dev, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
#define RAI_TENSOR_ALLOC_FILL(arena, fill, ...) rai__tensor_alloc_fill(arena, fill, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
#define RAI_TENSOR_ALLOC_LIKE_FILL(arena, t, fill, ...) rai__tensor_alloc_fill(arena, fill, (t).rank, (t).dims + RAI__TENSOR_MAXRANK - (t).rank)

// View operations
#define RAI_TENSOR_RESHAPE(t, ...) rai__tensor_reshape(t, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
#define RAI_TENSOR_RESHAPE_LIKE(t, like_t) rai__tensor_reshape(t, (like_t).rank, (like_t).dims + RAI__TENSOR_MAXRANK - (like_t).rank)
#define RAI_TENSOR_SUBTENSOR(t, ...) rai__tensor_subtensor(t, RAI__NULL_TERMINATED_ARRAY_LEN(__VA_ARGS__), RAI__NULL_TERMINATED_ARRAY(__VA_ARGS__))
RaiTensor rai_tensor_transpose(RaiTensor t, size_t axis_a, size_t axis_b);

// Binary Operations
RaiTensor rai_tensor_add(RaiArena *arena, RaiTensor a, RaiTensor b);
RaiTensor rai_tensor_sub(RaiArena *arena, RaiTensor a, RaiTensor b);
RaiTensor rai_tensor_mul(RaiArena *arena, RaiTensor a, RaiTensor b);
RaiTensor rai_tensor_matmul_t(RaiArena *arena, RaiTensor a, RaiTensor b);
RaiTensor rai_tensor_rmsnorm(RaiArena *arena, RaiTensor t, RaiTensor weight, float eps);

// Binary Operations Gradients
typedef struct {
	RaiTensor d_a;
	RaiTensor d_b;
} RaiTensorBinOpGrad;

RaiTensorBinOpGrad rai_tensor_add_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b);
RaiTensorBinOpGrad rai_tensor_sub_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b);
RaiTensorBinOpGrad rai_tensor_mul_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b);
RaiTensorBinOpGrad rai_tensor_matmul_t_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b);
RaiTensorBinOpGrad rai_tensor_rmsnorm_grad(RaiArena *arena_in, RaiArena *arena_weight, RaiTensor d_out, RaiTensor in, RaiTensor weight, float eps);

// Unary Operations
RaiTensor rai_tensor_copy(RaiArena *arena, RaiTensor t);
RaiTensor rai_tensor_scale(RaiArena *arena, RaiTensor t, float scale);
RaiTensor rai_tensor_relu(RaiArena *arena, RaiTensor t);
RaiTensor rai_tensor_leaky_relu(RaiArena *arena, RaiTensor t, float leak);
RaiTensor rai_tensor_silu(RaiArena *arena, RaiTensor t);
RaiTensor rai_tensor_gelu(RaiArena *arena, RaiTensor t);
RaiTensor rai_tensor_softmax(RaiArena *arena, RaiTensor t);
RaiTensor rai_tensor_rope(RaiArena *arena, RaiTensor t, int start_idx, float theta_scale);

// Unary Operations Gradients
RaiTensor rai_tensor_scale_grad(RaiArena *arena, RaiTensor d_out, float scale);
RaiTensor rai_tensor_relu_grad(RaiArena *arena, RaiTensor d_out, RaiTensor in);
RaiTensor rai_tensor_leaky_relu_grad(RaiArena *arena, RaiTensor d_out, RaiTensor in, float leak);
RaiTensor rai_tensor_silu_grad(RaiArena *arena, RaiTensor d_out, RaiTensor in);
RaiTensor rai_tensor_softmax_grad(RaiArena *arena, RaiTensor d_out, RaiTensor probs);
RaiTensor rai_tensor_rope_grad(RaiArena *arena, RaiTensor d_out, unsigned long start_idx, float theta_scale);

// ------------------------- MLP API ---------------------------------
typedef struct {
	RaiTensor weight;
	RaiTensor bias;
} RaiMlpLayer;

RaiMlpLayer rai_mlp_layer_alloc_randn(RaiArena* arena, size_t num_inputs, size_t num_outputs);

typedef struct {
	RaiTensor prod;
	RaiTensor sum;
	RaiTensor out;
} RaiMlpLayerActs;

RaiMlpLayerActs rai_mlp_layer_forward(RaiArena* arena, RaiMlpLayer layer, RaiTensor in);

typedef struct {
	RaiTensor d_weight;
	RaiTensor d_bias;
} RaiMlpLayerGrad;

RaiMlpLayerGrad rai_mlp_layer_backward(
	RaiArena* arena,
	RaiArena* scratch,
	RaiMlpLayer layer,
	RaiMlpLayerActs acts,
	RaiTensor d_out,
	RaiTensor in
);

// ------------------------- LLM API ---------------------------------
// Model Configuration
typedef struct {
	size_t vocab_size;	     // V
	size_t embedding_length;     // D
	size_t attention_head_count; // H
	size_t feed_forward_length;  // F

	size_t block_count;	     // L
	float attention_layer_norm_rms_epsilon;
	float rope_freq_base;
} RaiModelConfig;

// Optimizer configuration
typedef struct {
	float lr;
	float beta1;
	float beta2;
	float eps;
	float weight_decay;
} RaiAdamWConfig;

// Model structure
#define RAI_MODEL_MAXBLOCKCOUNT 8
#define RAI_MODEL_TENSOR_COUNT (3 + 9 * RAI_MODEL_MAXBLOCKCOUNT)
typedef union {
	struct {
		// Embedding
		RaiTensor token_embd; // [V, D]

		// Transformer
		struct {
			RaiTensor attn_norm;   // [D]
			RaiTensor attn_q;      // [D, D]
			RaiTensor attn_k;      // [D, D]
			RaiTensor attn_v;      // [D, D]
			RaiTensor attn_output; // [D, D]

			RaiTensor ffn_norm;    // [D]
			RaiTensor ffn_gate;    // [F, D]
			RaiTensor ffn_up;      // [F, D]
			RaiTensor ffn_down;    // [D, F]
		} blk[RAI_MODEL_MAXBLOCKCOUNT];

		// LM Head
		RaiTensor output_norm; // [D]
		RaiTensor output;      // [V, D]
	};
	RaiTensor array[RAI_MODEL_TENSOR_COUNT];
} RaiModel;

// Optimizer states
typedef struct {
	RaiModel means; // First moments
	RaiModel uvars; // Second moments
	size_t time;	// Timestep
} RaiAdamW;

// High level training engine
typedef struct {
	RaiModelConfig model_config;
	RaiAdamWConfig adamw_config;

	RaiArena arena; // Persistent memory

	RaiModel weights;
	RaiAdamW adamw;
} RaiEngine;

// Temporary Activations
typedef struct {
	// Embedding
	RaiTensor token_embd; // [B, S, D]

	// Transformer
	struct {
		RaiTensor attn_norm;	  // [B, S, D]

		RaiTensor attn_q;	  // [B, S, D]
		RaiTensor attn_k;	  // [B, S, D]

		RaiTensor attn_q_rope;	  // [B, S, D]
		RaiTensor attn_k_rope;	  // [B, S, D]

		RaiTensor attn_q_split;	  // [B, S, H, D_head]
		RaiTensor attn_k_split;	  // [B, S, H, D_head]
		RaiTensor attn_q_heads;	  // [B, H, S, D_head]
		RaiTensor attn_k_heads;	  // [B, H, S, D_head]

		RaiTensor attn_qkt;	  // [B, H, S, S]
		RaiTensor attn_scores;	  // [B, H, S, S]
		RaiTensor attn_probs;	  // [B, H, S, S]

		RaiTensor attn_v;	  // [B, S, D]
		RaiTensor attn_v_split;	  // [B, S, H, D_head]
		RaiTensor attn_v_heads;	  // [B, H, S, D_head]
		RaiTensor attn_v_heads_t; // [B, H, D_head, S]
		RaiTensor attn_ctx_heads; // [B, H, S, D_head]
		RaiTensor attn_ctx_split; // [B, S, H, D_head]
		RaiTensor attn_ctx;	  // [B, S, D]

		RaiTensor attn_output;	  // [B, S, D]

		RaiTensor x_mid;	  // [B, S, D]

		RaiTensor ffn_norm;	  // [B, S, D]

		RaiTensor ffn_gate;	  // [B, S, F]
		RaiTensor ffn_up;	  // [B, S, F]
		RaiTensor ffn_silu;	  // [B, S, F]
		RaiTensor ffn_swiglu;	  // [B, S, F]

		RaiTensor ffn_down;	  // [B, S, D]

		RaiTensor x_out;	  // [B, S, D]
	} blk[RAI_MODEL_MAXBLOCKCOUNT];

	// LM Head
	RaiTensor output_norm; // [B, S, D]
	RaiTensor output;      // [B, S, V]
} RaiModelActs;

// Loss and Backprop Seeding
float rai_cross_entropy(RaiTensor logits, size_t batch_size, size_t seq_len, int targets[batch_size * seq_len]);
RaiTensor rai_cross_entropy_grad(RaiArena *arena, RaiTensor logits, size_t batch_size, size_t seq_len, int targets[batch_size * seq_len]);

// ------------------------- Engine API ---------------------------------
RaiEngine rai_engine_create(
	// Configurations
	size_t memory_capacity,
	RaiModelConfig model_config,
	RaiAdamWConfig adamw_config
);

void rai_engine_destroy(
	// Mutating Object
	RaiEngine *engine
);

RaiModelActs rai_engine_forward(
	// Workspace
	RaiArena *scratch,

	// Engine Context
	RaiEngine engine,

	// Input Tokens
	size_t batch_size,
	size_t seq_len,
	int tokens[batch_size * seq_len],

	// Execution State
	size_t pos_offset
);

RaiModel rai_engine_backward(
	// Workspaces
	RaiArena *scratch_acts,
	RaiArena *scratch_grads,

	// Engine Context
	RaiEngine engine,

	// Forward Pass Activations
	RaiModelActs acts,

	// Loss Gradient
	RaiTensor d_logits, // [B, S, V]

	// Input Tokens
	size_t batch_size,
	size_t seq_len,
	int tokens[batch_size * seq_len],

	// Execution State
	size_t pos_offset
);

void rai_engine_step(
	// Mutating Object
	RaiEngine *engine,

	// Gradients
	RaiModel grads
);

float rai_lr_cosine(float step, float warmup_steps, float max_steps, float max_lr, float min_lr);

// ------------------------- Tokenizer API -------------------------
typedef struct {
	int left;
	int right;
} RaiMerge;

typedef struct {
	size_t length;
	char *data;
} RaiString;

typedef struct {
	size_t count;
	int *data;
} RaiTokenArray;

typedef struct {
	size_t vocab_size;
	RaiMerge *merges;
	RaiString *vocab;
} RaiTokenizer;

RaiTokenizer rai_tokenizer_train(
	RaiArena *arena,
	RaiArena *scratch,
	RaiString text,
	size_t target_vocab_size
);

RaiTokenArray rai_tokenizer_encode(
	RaiArena *arena,
	RaiTokenizer tok,
	RaiString text
);

RaiString rai_tokenizer_decode(
	RaiArena *arena,
	RaiTokenizer tok,
	RaiTokenArray tokens
);

#endif // RRIFTT_AI_H

#ifdef RRIFTT_AI_IMPLEMENTATION

void rai_tensor_info(RaiTensor t)
{
	RAI_PRINTF("rank = %zu\n", t.rank);
	for (size_t i = 0; i < RAI__TENSOR_MAXRANK; ++i) {
		RAI_PRINTF("dims[%zu] = %zu, strs[%zu] = %zu\n", i, t.dims[i], i, t.strs[i]);
	}
	RAI_PRINTF("data = %p\n", (void *)t.data);
	RAI_PRINTF("count = %zu\n", t.count);
}

void rai__tensor_print(RaiTensor t, int indent)
{
	bool scalar = t.rank == 0;
	bool vector = t.rank == 1;

	// Scalar
	if (scalar) {
		RAI_PRINTF("%5.2f  %s", t.data[0], indent == 0 ? "\n" : "");
		return;
	}

	RAI_PRINTF("%*s[%s", indent, "", vector ? "  " : "\n");
	for (size_t i = 0; i < t.dims[RAI__TENSOR_MAXRANK - t.rank]; ++i) {
		rai__tensor_print(RAI_TENSOR_SUBTENSOR(t, i), indent + 4);
	}
	RAI_PRINTF("%*s]\n", vector ? 0 : indent, "");
}

float rai_randn(float mean, float std_dev)
{
	static float x2 = 0.0f;
	static bool call = false;

	if (call) {
		call = !call;
		return mean + std_dev * x2;
	}

	float u1 = 0.0f, u2 = 0.0f, w = 0.0f;
	do {
		u1 = (float)RAI_RAND() / (float)RAI_RAND_MAX * 2.0f - 1.0f;
		u2 = (float)RAI_RAND() / (float)RAI_RAND_MAX * 2.0f - 1.0f;
		w = u1 * u1 + u2 * u2;
	} while (w >= 1.0f || w == 0.0f);

	float scale = RAI_SQRTF((-2.0f * RAI_LOGF(w)) / w);
	x2 = u2 * scale;
	call = !call;
	return mean + std_dev * u1 * scale;
}

#define RAI__ALIGN(x) (x + alignof(max_align_t)) & ~(alignof(max_align_t) - 1)

RaiArena rai_arena_create(size_t capacity_bytes)
{
	RaiArena arena = {
		.alloced_bytes = 0,
		.capacity_bytes = RAI__ALIGN(capacity_bytes),
	};

	arena.memory_begin = RAI_MALLOC(arena.capacity_bytes);
	RAI_ASSERT(arena.memory_begin && "Failed to allocate RaiArena: OOM");

	return arena;
}

void *rai_arena_alloc(RaiArena *arena, size_t size_bytes)
{
	size_t aligned_size_bytes = RAI__ALIGN(size_bytes);
	RAI_ASSERT(arena->alloced_bytes + aligned_size_bytes <= arena->capacity_bytes && "Arena OOM");

	uint8_t *target_pointer = (uint8_t *)arena->memory_begin + arena->alloced_bytes;
	arena->alloced_bytes += aligned_size_bytes;

	return (void *)target_pointer;
}

#undef RAI__ALIGN

void rai_arena_clear(RaiArena *arena)
{
	arena->alloced_bytes = 0;
}

void rai_arena_destroy(RaiArena *arena)
{
	arena->alloced_bytes = 0;
	arena->capacity_bytes = 0;
	RAI_FREE(arena->memory_begin);
	arena->memory_begin = nullptr;
}

RaiTensor rai__tensor_alloc(RaiArena *arena, size_t rank, size_t dims[rank + 1])
{
	RAI_ASSERT(rank <= RAI__TENSOR_MAXRANK && "Tensor rank exceeds limit");
	RaiTensor t = { .rank = rank };

	// promotion
	for (size_t i = 0; i < RAI__TENSOR_MAXRANK - rank; ++i) {
		t.dims[i] = 1;
	}

	// actual ranks
	t.count = 1;
	for (size_t i = RAI__TENSOR_MAXRANK - rank; i < RAI__TENSOR_MAXRANK; ++i) {
		t.dims[i] = dims[i - (RAI__TENSOR_MAXRANK - rank)];
		t.count *= t.dims[i];
	}

	// strides
	size_t stride = 1;
	for (size_t i = 0; i < RAI__TENSOR_MAXRANK; ++i) {
		t.strs[RAI__TENSOR_MAXRANK - 1 - i] = stride;
		stride *= t.dims[RAI__TENSOR_MAXRANK - 1 - i];
	}

	t.data = rai_arena_alloc(arena, t.count * sizeof(t.data[0]));

	return t;
}

RaiTensor rai__tensor_alloc_randn(RaiArena *arena, float mean, float std_dev, size_t rank, size_t dims[rank + 1])
{
	RaiTensor t = rai__tensor_alloc(arena, rank, dims);
	for (size_t i = 0; i < t.count; ++i) {
		t.data[i] = rai_randn(mean, std_dev);
	}
	return t;
}

RaiTensor rai__tensor_alloc_fill(RaiArena *arena, float fill, size_t rank, size_t dims[rank + 1])
{
	RaiTensor t = rai__tensor_alloc(arena, rank, dims);
	for (size_t i = 0; i < t.count; ++i) {
		t.data[i] = fill;
	}
	return t;
}

RaiTensor rai__tensor_reshape(RaiTensor t, size_t new_rank, size_t new_dims[new_rank + 1])
{
	RAI_ASSERT(new_rank <= RAI__TENSOR_MAXRANK && "reshape rank exceeds limit");
	RaiTensor rt = { .rank = new_rank, .data = t.data };

	// promotion
	for (size_t i = 0; i < RAI__TENSOR_MAXRANK - new_rank; ++i) {
		rt.dims[i] = 1;
	}

	// actual ranks
	rt.count = 1;
	for (size_t i = RAI__TENSOR_MAXRANK - new_rank; i < RAI__TENSOR_MAXRANK; ++i) {
		rt.dims[i] = new_dims[i - (RAI__TENSOR_MAXRANK - new_rank)];
		rt.count *= rt.dims[i];
	}
	RAI_ASSERT(rt.count == t.count && "reshape element count mismatch");

	// strides
	size_t stride = 1;
	for (size_t i = 0; i < RAI__TENSOR_MAXRANK; ++i) {
		rt.strs[RAI__TENSOR_MAXRANK - 1 - i] = stride;
		stride *= rt.dims[RAI__TENSOR_MAXRANK - 1 - i];
	}

	return rt;
}

RaiTensor rai__tensor_subtensor(RaiTensor t, size_t idxs_len, size_t idxs[idxs_len + 1])
{
	RAI_ASSERT(t.rank >= 1 && "scalars cannot have subtensors");
	RAI_ASSERT(idxs_len <= t.rank && "number of indices exceeds rank");
	RaiTensor st = { .rank = t.rank - idxs_len, .data = t.data };

	// promotion
	for (size_t i = 0; i < RAI__TENSOR_MAXRANK - t.rank; ++i) {
		st.dims[i] = 1;
	}

	// indexing
	size_t start_idx = RAI__TENSOR_MAXRANK - t.rank;
	for (size_t i = start_idx; i < start_idx + idxs_len; ++i) {
		size_t idx = idxs[i - start_idx];
		RAI_ASSERT(idx < t.dims[i] && "tensor index OOB");
		st.dims[i] = 1;
		st.data += idx * t.strs[i];
	}

	// inheritance
	st.count = 1;
	for (size_t i = start_idx + idxs_len; i < RAI__TENSOR_MAXRANK; ++i) {
		st.dims[i] = t.dims[i];
		st.count *= st.dims[i];
	}

	// strides
	size_t stride = 1;
	for (size_t i = 0; i < RAI__TENSOR_MAXRANK; ++i) {
		st.strs[RAI__TENSOR_MAXRANK - 1 - i] = stride;
		stride *= st.dims[RAI__TENSOR_MAXRANK - 1 - i];
	}

	return st;
}

RaiTensor rai_tensor_transpose(RaiTensor t, size_t axis_a, size_t axis_b)
{
	RAI_ASSERT(axis_a < t.rank && "Transpose axis_a out of bounds");
	RAI_ASSERT(axis_b < t.rank && "Transpose axis_b out of bounds");
	RaiTensor tt = t;

	size_t offset = RAI__TENSOR_MAXRANK - tt.rank;
	size_t idx_a = offset + axis_a;
	size_t idx_b = offset + axis_b;

	size_t tmp_d = tt.dims[idx_a];
	tt.dims[idx_a] = tt.dims[idx_b];
	tt.dims[idx_b] = tmp_d;

	size_t tmp_s = tt.strs[idx_a];
	tt.strs[idx_a] = tt.strs[idx_b];
	tt.strs[idx_b] = tmp_s;

	return tt;
}

#define RAI__TENSOR_BROADCAST_AND_PROMOTE(NAME, A, B, LIMIT)                  \
	do {                                                                  \
		/* Rank Promotion */                                          \
		size_t max_r = (A).rank > (B).rank ? (A).rank : (B).rank;     \
		(A).rank = max_r;                                             \
		(B).rank = max_r;                                             \
                                                                              \
		/* Dimension Alignment */                                     \
		for (size_t i = 0; i < (LIMIT); ++i) {                        \
			size_t d_a = (A).dims[i];                             \
			size_t d_b = (B).dims[i];                             \
                                                                              \
			if (d_a == 1 && d_b > 1) {                            \
				(A).dims[i] = d_b;                            \
				(A).strs[i] = 0;                              \
				(A).count *= d_b;                             \
			} else if (d_b == 1 && d_a > 1) {                     \
				(B).dims[i] = d_a;                            \
				(B).strs[i] = 0;                              \
				(B).count *= d_a;                             \
			} else if (d_a != d_b) {                              \
				RAI_ASSERT(0 && #NAME " Dimension mismatch"); \
			}                                                     \
		}                                                             \
	} while (false)

static void rai__tensor_add(RaiTensor out, RaiTensor a, RaiTensor b)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_a = a.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_b = b.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			out.data[i * s_out] = a.data[i * s_a] + b.data[i * s_b];
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_add(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(a, i), RAI_TENSOR_SUBTENSOR(b, i));
	}
}

RaiTensor rai_tensor_add(RaiArena *arena, RaiTensor a, RaiTensor b)
{
	RAI__TENSOR_BROADCAST_AND_PROMOTE(add, a, b, RAI__TENSOR_MAXRANK);
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, a);
	rai__tensor_add(out, a, b);
	return out;
}

static void rai__tensor_sub(RaiTensor out, RaiTensor a, RaiTensor b)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_a = a.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_b = b.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			out.data[i * s_out] = a.data[i * s_a] - b.data[i * s_b];
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_sub(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(a, i), RAI_TENSOR_SUBTENSOR(b, i));
	}
}

RaiTensor rai_tensor_sub(RaiArena *arena, RaiTensor a, RaiTensor b)
{
	RAI__TENSOR_BROADCAST_AND_PROMOTE(add, a, b, RAI__TENSOR_MAXRANK);
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, a);
	rai__tensor_sub(out, a, b);
	return out;
}

static void rai__tensor_mul(RaiTensor out, RaiTensor a, RaiTensor b)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_a = a.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_b = b.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			out.data[i * s_out] = a.data[i * s_a] * b.data[i * s_b];
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_mul(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(a, i), RAI_TENSOR_SUBTENSOR(b, i));
	}
}

RaiTensor rai_tensor_mul(RaiArena *arena, RaiTensor a, RaiTensor b)
{
	RAI__TENSOR_BROADCAST_AND_PROMOTE(mul, a, b, RAI__TENSOR_MAXRANK);
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, a);
	rai__tensor_mul(out, a, b);
	return out;
}

static void rai__tensor_gelu(RaiTensor out, RaiTensor in)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_in = in.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			float val = in.data[i * s_in];
			float phi = 0.5 * RAI_ERFCF(-val * RAI_SQRT1_2F);
			out.data[i * s_out] = val * phi;
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_gelu(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i));
	}
}

RaiTensor rai_tensor_gelu(RaiArena *arena, RaiTensor t)
{
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_gelu(out, t);
	return out;
}

static void rai__tensor_silu(RaiTensor out, RaiTensor in)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_in = in.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			float val = in.data[i * s_in];
			float sigmoid = 1.0f / (1.0f + RAI_EXPF(-val));
			out.data[i * s_out] = val * sigmoid;
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_silu(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i));
	}
}

RaiTensor rai_tensor_silu(RaiArena *arena, RaiTensor t)
{
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_silu(out, t);
	return out;
}

static void rai__tensor_leaky_relu(RaiTensor out, RaiTensor in, float leak)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_in = in.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			float val = in.data[i * s_in];
			out.data[i * s_out] = val > 0.0f ? val : leak * val;
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_leaky_relu(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i), leak);
	}
}

RaiTensor rai_tensor_leaky_relu(RaiArena *arena, RaiTensor t, float leak)
{
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_leaky_relu(out, t, leak);
	return out;
}

RaiTensor rai_tensor_relu(RaiArena *arena, RaiTensor t)
{
	return rai_tensor_leaky_relu(arena, t, 0.0f);
}

static void rai__tensor_copy(RaiTensor out, RaiTensor in)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_in = in.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			out.data[i * s_out] = in.data[i * s_in];
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_copy(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i));
	}
}

RaiTensor rai_tensor_copy(RaiArena *arena, RaiTensor t)
{
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_copy(out, t);
	return out;
}

static void rai__tensor_scale(RaiTensor out, RaiTensor in, float scale)
{
	// base case
	if (out.rank <= 1) {
		size_t len = out.dims[RAI__TENSOR_MAXRANK - 1];
		size_t s_out = out.strs[RAI__TENSOR_MAXRANK - 1];
		size_t s_in = in.strs[RAI__TENSOR_MAXRANK - 1];

		for (size_t i = 0; i < len; ++i) {
			out.data[i * s_out] = in.data[i * s_in] * scale;
		}
		return;
	}

	// recursive step
	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	for (size_t i = 0; i < loop_count; ++i) {
		rai__tensor_scale(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i), scale);
	}
}

RaiTensor rai_tensor_scale(RaiArena *arena, RaiTensor t, float scale)
{
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_scale(out, t, scale);
	return out;
}

static void rai__tensor_matmul_t(RaiTensor out, RaiTensor a, RaiTensor b)
{
	size_t col_index = RAI__TENSOR_MAXRANK - 1;
	size_t row_index = RAI__TENSOR_MAXRANK - 2;

	// base case
	if (out.rank <= 2) {
		size_t M = out.dims[row_index];
		size_t N = out.dims[col_index];
		size_t P = a.dims[col_index];

		size_t sr_out = out.strs[row_index];
		size_t sc_out = out.strs[col_index];
		size_t sr_a = a.strs[row_index];
		size_t sc_a = a.strs[col_index];
		size_t sr_b = b.strs[row_index];
		size_t sc_b = b.strs[col_index];

		float *restrict ptr_out = out.data;
		float *restrict ptr_a = a.data;
		float *restrict ptr_b = b.data;
		for (size_t i = 0; i < M; ++i) {
			for (size_t j = 0; j < N; ++j) {
				float acc = 0.0f;
				for (size_t k = 0; k < P; ++k) {
					acc += ptr_a[i * sr_a + k * sc_a] * ptr_b[j * sr_b + k * sc_b];
				}
				ptr_out[i * sr_out + j * sc_out] = acc;
			}
		}

		return;
	}

	size_t current_dim = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_count = out.dims[current_dim];

	// recursive step
	for (size_t i = 0; i < loop_count; ++i) {
		RaiTensor st_a = RAI_TENSOR_SUBTENSOR(a, i);
		RaiTensor st_b = RAI_TENSOR_SUBTENSOR(b, i);
		RaiTensor st_out = RAI_TENSOR_SUBTENSOR(out, i);

		rai__tensor_matmul_t(st_out, st_a, st_b);
	}
}

RaiTensor rai_tensor_matmul_t(RaiArena *arena, RaiTensor a, RaiTensor b)
{
	// dimensions check
	size_t col_index = RAI__TENSOR_MAXRANK - 1;
	size_t row_index = RAI__TENSOR_MAXRANK - 2;
	RAI_ASSERT(a.dims[col_index] == b.dims[col_index] && "Dimension mismatch for matmul");
	RAI__TENSOR_BROADCAST_AND_PROMOTE(MatMul, a, b, RAI__TENSOR_MAXRANK - 2);

	// New dims is neither like a nor like b. So we recalculate
	size_t dims[RAI__TENSOR_MAXRANK];
	RAI_MEMCPY(dims, a.dims, RAI__TENSOR_MAXRANK * sizeof(dims[0]));
	dims[col_index] = b.dims[row_index];
	RaiTensor out = rai__tensor_alloc(arena, a.rank, dims + RAI__TENSOR_MAXRANK - a.rank);

	rai__tensor_matmul_t(out, a, b);
	return out;
}

static void rai__tensor_rope(RaiTensor out, RaiTensor in, int start_idx, float theta_scale)
{
	// base case
	if (out.rank <= 2) {
		size_t r = RAI__TENSOR_MAXRANK;

		size_t d0 = out.dims[r - 1];
		size_t d1 = out.dims[r - 2];

		size_t s0 = in.strs[r - 1], s1 = in.strs[r - 2];
		size_t os0 = out.strs[r - 1], os1 = out.strs[r - 2];

		RAI_ASSERT(d0 % 2 == 0 && "Rotation dimension (d0) must be even");
		size_t half_d0 = d0 / 2;

		for (size_t i1 = 0; i1 < d1; ++i1) {
			unsigned long current_idx = start_idx + i1;

			// Stack-allocated trig cache
			// TODO: cache elsewhere
			float cos_v[half_d0], sin_v[half_d0];

			for (size_t k = 0; k < half_d0; ++k) {
				float freq = 1.0f / RAI_POWF(theta_scale, (float)(2 * k) / (float)d0);
				float alpha = (float)current_idx * freq;
				cos_v[k] = RAI_COSF(alpha);
				sin_v[k] = RAI_SINF(alpha);
			}

			float *src = in.data + (i1 * s1);
			float *dst = out.data + (i1 * os1);

			for (size_t k = 0; k < half_d0; ++k) {
				// Temporary variables make this in-place safe
				float v_l = src[k * s0];
				float v_r = src[(k + half_d0) * s0];

				float cv = cos_v[k];
				float sv = sin_v[k];

				dst[k * os0] = v_l * cv - v_r * sv;
				dst[(k + half_d0) * os0] = v_l * sv + v_r * cv;
			}
		}
		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - out.rank;
	size_t loop_len = out.dims[outer_idx];

	for (size_t i = 0; i < loop_len; ++i) {
		rai__tensor_rope(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i), start_idx, theta_scale);
	}
}

RaiTensor rai_tensor_rope(RaiArena *arena, RaiTensor t, int start_idx, float theta_scale)
{
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_rope(out, t, start_idx, theta_scale);
	return out;
}

static void rai__tensor_rmsnorm(RaiTensor out, RaiTensor in, RaiTensor weight, float eps)
{
	// base case
	if (out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;

		size_t N = out.dims[idx];

		size_t s_in = in.strs[idx];
		size_t s_out = out.strs[idx];
		size_t s_w = weight.strs[idx];

		float sum_sq = 0.0f;
		for (size_t i = 0; i < N; ++i) {
			// TODO: Vectorise this loop for different platforms
			float x = in.data[i * s_in];
			sum_sq += x * x;
		}

		float mean = sum_sq / N;

		// TODO: Quake3 Inverse Squareroot perhaps?
		float scale = 1.0f / RAI_SQRTF(mean + eps);

		for (size_t i = 0; i < N; ++i) {
			// TODO: Vectorise this loop for different platforms
			float x = in.data[i * s_in];
			float w = weight.data[i * s_w];
			out.data[i * s_out] = x * scale * w;
		}

		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - out.rank;
	size_t outer_dim = out.dims[outer_idx];
	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_rmsnorm(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i), RAI_TENSOR_SUBTENSOR(weight, i), eps);
	}
}

RaiTensor rai_tensor_rmsnorm(RaiArena *arena, RaiTensor t, RaiTensor weight, float eps)
{
	RAI__TENSOR_BROADCAST_AND_PROMOTE(RMSNorm, t, weight, RAI__TENSOR_MAXRANK);
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_rmsnorm(out, t, weight, eps);
	return out;
}

static void rai__tensor_softmax(RaiTensor out, RaiTensor in)
{
	// base case
	if (out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = out.dims[idx];
		size_t s_in = in.strs[idx];
		size_t s_out = out.strs[idx];

		if (N == 0) {
			return;
		}

		// Find Max (for numerical stability)
		float max_val = -RAI_INFINITY;
		for (size_t i = 0; i < N; ++i) {
			float val = in.data[i * s_in];
			if (val > max_val) {
				max_val = val;
			}
		}

		// Exponentiate and Sum
		// We write the exponentials directly into 'out' to avoid re-computing expf
		float sum = 0.0f;
		for (size_t i = 0; i < N; ++i) {
			float val = RAI_EXPF(in.data[i * s_in] - max_val);
			out.data[i * s_out] = val; // Store e^(x-max)
			sum += val;
		}

		// Normalize
		float inv_sum = 1.0f / sum;
		for (size_t i = 0; i < N; ++i) {
			out.data[i * s_out] *= inv_sum;
		}

		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - out.rank;
	size_t outer_dim = out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_softmax(RAI_TENSOR_SUBTENSOR(out, i), RAI_TENSOR_SUBTENSOR(in, i));
	}
}

RaiTensor rai_tensor_softmax(RaiArena *arena, RaiTensor t)
{
	RaiTensor out = RAI_TENSOR_ALLOC_LIKE(arena, t);
	rai__tensor_softmax(out, t);
	return out;
}

RaiModel rai_model_alloc_weights(RaiArena *arena, RaiModelConfig config)
{
	size_t V = config.vocab_size;
	size_t D = config.embedding_length;
	size_t H = config.attention_head_count;
	size_t F = config.feed_forward_length;
	size_t L = config.block_count;

	RAI_ASSERT(L <= RAI_MODEL_MAXBLOCKCOUNT && "Block Count exceeds RAI_MODEL_MAXBLOCKCOUNT");
	RAI_ASSERT(D % H == 0 && "embedding_length (D) must be divisible by attention_head_count (H)");
	RAI_ASSERT(D > 0 && H > 0 && "Model dimensions must be non-zero");

	RaiModel w = { 0 };

	w.token_embd = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, 1.0f, V, D); // [V, D]

	float inv_sqrtD = 1.0f / RAI_SQRTF((float)D);
	float inv_sqrt2LD = 1.0f / RAI_SQRTF(2.0f * (float)L * (float)D);
	float inv_sqrt2LF = 1.0f / RAI_SQRTF(2.0f * (float)L * (float)F);

	for (size_t l = 0; l < L; ++l) {
		w.blk[l].attn_norm = RAI_TENSOR_ALLOC_FILL(arena, 1.0f, D);		       // [D]
		w.blk[l].attn_q = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrtD, D, D);	       // [D, D]
		w.blk[l].attn_k = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrtD, D, D);	       // [D, D]
		w.blk[l].attn_v = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrtD, D, D);	       // [D, D]
		w.blk[l].attn_output = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrt2LD, D, D); // [D, D]

		w.blk[l].ffn_norm = RAI_TENSOR_ALLOC_FILL(arena, 1.0f, D);		       // [D]
		w.blk[l].ffn_gate = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrtD, F, D);      // [F, D]
		w.blk[l].ffn_up = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrtD, F, D);	       // [F, D]
		w.blk[l].ffn_down = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrt2LF, D, F);    // [D, F]
	}

	w.output_norm = RAI_TENSOR_ALLOC_FILL(arena, 1.0f, D);		 // [D]
	w.output = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, inv_sqrtD, V, D); // [V, D]

	return w;
}

RaiModel rai_model_alloc_zeroed(RaiArena *arena, RaiModelConfig config)
{
	size_t V = config.vocab_size;
	size_t D = config.embedding_length;
	size_t H = config.attention_head_count;
	size_t F = config.feed_forward_length;
	size_t L = config.block_count;

	RAI_ASSERT(L <= RAI_MODEL_MAXBLOCKCOUNT && "Block Count exceeds RAI_MODEL_MAXBLOCKCOUNT");
	RAI_ASSERT(D % H == 0 && "embedding_length (D) must be divisible by attention_head_count (H)");
	RAI_ASSERT(D > 0 && H > 0 && "Model dimensions must be non-zero");

	RaiModel m = { 0 };

	// We cannot to the zero-filling by RAI_MODEL_TENSOR_COUNT because
	// not every tensor is the same shape. We are literally defining the shapes here.
	// Makes you wonder, maybe lazy != stupid afterall
	m.token_embd = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, V, D); // [V, D]

	for (size_t l = 0; l < L; ++l) {
		m.blk[l].attn_norm = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D);	 // [D]
		m.blk[l].attn_q = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D, D);	 // [D, D]
		m.blk[l].attn_k = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D, D);	 // [D, D]
		m.blk[l].attn_v = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D, D);	 // [D, D]
		m.blk[l].attn_output = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D, D); // [D, D]

		m.blk[l].ffn_norm = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D);	 // [D]
		m.blk[l].ffn_gate = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, F, D);	 // [F, D]
		m.blk[l].ffn_up = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, F, D);	 // [F, D]
		m.blk[l].ffn_down = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D, F);	 // [D, F]
	}

	m.output_norm = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, D); // [D]
	m.output = RAI_TENSOR_ALLOC_FILL(arena, 0.0f, V, D);   // [V, D]

	return m;
}

RaiModelActs rai_engine_forward(
	// Workspace
	RaiArena *scratch,

	// Engine Context
	RaiEngine engine,

	// Input Tokens
	size_t batch_size,
	size_t seq_len,
	int tokens[batch_size * seq_len],

	// Execution State
	size_t pos_offset
)
{
	RaiModelActs acts;

	size_t B = batch_size;
	size_t S = seq_len;
	size_t D = engine.model_config.embedding_length;
	size_t H = engine.model_config.attention_head_count;
	size_t D_head = D / H;
	float scale = 1.0f / RAI_SQRTF((float)D_head);
	size_t L = engine.model_config.block_count;

	acts.token_embd = RAI_TENSOR_ALLOC(scratch, B, S, D);

	// Embedding
	for (size_t b = 0; b < batch_size; ++b) {
		for (size_t s = 0; s < seq_len; ++s) {
			int token = tokens[b * seq_len + s];
			RAI_ASSERT((size_t)token < engine.model_config.vocab_size && "Unknown token");
			rai__tensor_copy(RAI_TENSOR_SUBTENSOR(acts.token_embd, b, s), RAI_TENSOR_SUBTENSOR(engine.weights.token_embd, token));
		}
	}

	// Transformer
	for (size_t l = 0; l < L; ++l) {
		// Attention RMSNorm
		RaiTensor x_in = l == 0 ? acts.token_embd : acts.blk[l - 1].x_out;
		acts.blk[l].attn_norm = rai_tensor_rmsnorm(scratch, x_in, engine.weights.blk[l].attn_norm, engine.model_config.attention_layer_norm_rms_epsilon);

		// QK projections
		acts.blk[l].attn_q = rai_tensor_matmul_t(scratch, acts.blk[l].attn_norm, engine.weights.blk[l].attn_q);
		acts.blk[l].attn_k = rai_tensor_matmul_t(scratch, acts.blk[l].attn_norm, engine.weights.blk[l].attn_k);

		// RoPE
		acts.blk[l].attn_q_rope = rai_tensor_rope(scratch, acts.blk[l].attn_q, pos_offset, engine.model_config.rope_freq_base);
		acts.blk[l].attn_k_rope = rai_tensor_rope(scratch, acts.blk[l].attn_k, pos_offset, engine.model_config.rope_freq_base);

		// Head splitting
		acts.blk[l].attn_q_split = RAI_TENSOR_RESHAPE(acts.blk[l].attn_q_rope, B, S, H, D_head);
		acts.blk[l].attn_k_split = RAI_TENSOR_RESHAPE(acts.blk[l].attn_k_rope, B, S, H, D_head);
		acts.blk[l].attn_q_heads = rai_tensor_transpose(acts.blk[l].attn_q_split, 1, 2);
		acts.blk[l].attn_k_heads = rai_tensor_transpose(acts.blk[l].attn_k_split, 1, 2);

		// Scaled Dot Product Attention
		acts.blk[l].attn_qkt = rai_tensor_matmul_t(scratch, acts.blk[l].attn_q_heads, acts.blk[l].attn_k_heads);
		acts.blk[l].attn_scores = rai_tensor_scale(scratch, acts.blk[l].attn_qkt, scale);
		acts.blk[l].attn_probs = rai_tensor_softmax(scratch, acts.blk[l].attn_scores);

		// Attention context with Heads merging
		acts.blk[l].attn_v = rai_tensor_matmul_t(scratch, acts.blk[l].attn_norm, engine.weights.blk[l].attn_v);
		acts.blk[l].attn_v_split = RAI_TENSOR_RESHAPE(acts.blk[l].attn_v, B, S, H, D_head);
		acts.blk[l].attn_v_heads = rai_tensor_transpose(acts.blk[l].attn_v_split, 1, 2);
		acts.blk[l].attn_v_heads_t = rai_tensor_transpose(acts.blk[l].attn_v_heads, 2, 3);
		acts.blk[l].attn_ctx_heads = rai_tensor_matmul_t(scratch, acts.blk[l].attn_probs, acts.blk[l].attn_v_heads_t);
		acts.blk[l].attn_ctx_split = rai_tensor_transpose(acts.blk[l].attn_ctx_heads, 1, 2);
		acts.blk[l].attn_ctx = RAI_TENSOR_RESHAPE(acts.blk[l].attn_ctx_split, B, S, D);

		// Attention output
		acts.blk[l].attn_output = rai_tensor_matmul_t(scratch, acts.blk[l].attn_ctx, engine.weights.blk[l].attn_output);

		// Residual connection
		acts.blk[l].x_mid = rai_tensor_add(scratch, x_in, acts.blk[l].attn_output);

		// FFN RMSnorm
		acts.blk[l].ffn_norm = rai_tensor_rmsnorm(scratch, acts.blk[l].x_mid, engine.weights.blk[l].ffn_norm, engine.model_config.attention_layer_norm_rms_epsilon);

		// SwiGLU
		acts.blk[l].ffn_gate = rai_tensor_matmul_t(scratch, acts.blk[l].ffn_norm, engine.weights.blk[l].ffn_gate);
		acts.blk[l].ffn_up = rai_tensor_matmul_t(scratch, acts.blk[l].ffn_norm, engine.weights.blk[l].ffn_up);
		acts.blk[l].ffn_silu = rai_tensor_silu(scratch, acts.blk[l].ffn_gate);
		acts.blk[l].ffn_swiglu = rai_tensor_mul(scratch, acts.blk[l].ffn_silu, acts.blk[l].ffn_up);

		// Down projection
		acts.blk[l].ffn_down = rai_tensor_matmul_t(scratch, acts.blk[l].ffn_swiglu, engine.weights.blk[l].ffn_down);

		// Residual connection
		acts.blk[l].x_out = rai_tensor_add(scratch, x_in, acts.blk[l].ffn_down);
	}

	// Output RMSNorm
	acts.output_norm = rai_tensor_rmsnorm(scratch, acts.blk[L - 1].x_out, engine.weights.output_norm, engine.model_config.attention_layer_norm_rms_epsilon);

	// Logits
	acts.output = rai_tensor_matmul_t(scratch, acts.output_norm, engine.weights.output);

	return acts;
}

float rai_cross_entropy(RaiTensor logits, size_t batch_size, size_t seq_len, int targets[batch_size * seq_len])
{
	RAI_ASSERT(logits.rank == 3 && "logits is supposed to be a rank 3 tensor of shape [B, S, V]");

	float loss = 0.0f;
	for (size_t b = 0; b < batch_size; ++b) {
		for (size_t s = 0; s < seq_len; ++s) {
			RaiTensor slogits = RAI_TENSOR_SUBTENSOR(logits, b, s);
			size_t vocab_size = slogits.dims[RAI__TENSOR_MAXRANK - 1];
			size_t stride = slogits.strs[RAI__TENSOR_MAXRANK - 1];

			float max = -RAI_INFINITY;
			for (size_t i = 0; i < vocab_size; ++i) {
				if (slogits.data[i * stride] > max)
					max = slogits.data[i * stride];
			}

			float sum = 0.0f;
			for (size_t i = 0; i < vocab_size; ++i) {
				float exp = RAI_EXPF(slogits.data[i * stride] - max);
				sum += exp;
			}

			int target = targets[b * seq_len + s];
			RAI_ASSERT((size_t)target < vocab_size && "target outside vacabulary");
			loss += (max - slogits.data[target * stride]) + RAI_LOGF(sum);
		}
	}

	return loss / (float)batch_size / (float)seq_len;
}

RaiTensor rai_cross_entropy_grad(RaiArena *arena, RaiTensor logits, size_t batch_size, size_t seq_len, int targets[batch_size * seq_len])
{
	RAI_ASSERT(logits.rank == 3 && "logits is supposed to be a rank 3 tensor of shape [B, S, V]");
	RaiTensor d_logits = rai_tensor_softmax(arena, logits);

	for (size_t b = 0; b < batch_size; ++b) {
		for (size_t s = 0; s < seq_len; ++s) {
			int target = targets[b * seq_len + s];
			size_t vocab_size = logits.dims[RAI__TENSOR_MAXRANK - 1];
			RAI_ASSERT((size_t)target < vocab_size && "target outside vacabulary");
			RAI_TENSOR_SUBTENSOR(d_logits, b, s, target).data[0] -= 1.0f;
		}
	}

	rai__tensor_scale(d_logits, d_logits, 1.0f / (float)batch_size / (float)seq_len);
	return d_logits;
}

static void rai__tensor_matmul_t_grad(RaiTensor d_a, RaiTensor d_b, RaiTensor d_out, RaiTensor a, RaiTensor b)
{
	size_t col_index = RAI__TENSOR_MAXRANK - 1;
	size_t row_index = RAI__TENSOR_MAXRANK - 2;

	// base case
	if (d_out.rank <= 2) {
		size_t M = d_out.dims[row_index];
		size_t N = d_out.dims[col_index];
		size_t P = a.dims[col_index]; // b.dims[col_index] is also P

		size_t sr_dout = d_out.strs[row_index];
		size_t sc_dout = d_out.strs[col_index];

		size_t sr_da = d_a.strs[row_index];
		size_t sc_da = d_a.strs[col_index];
		size_t sr_db = d_b.strs[row_index];
		size_t sc_db = d_b.strs[col_index];

		size_t sr_a = a.strs[row_index];
		size_t sc_a = a.strs[col_index];
		size_t sr_b = b.strs[row_index];
		size_t sc_b = b.strs[col_index];

		float *restrict ptr_dout = d_out.data;
		float *restrict ptr_da = d_a.data;
		float *restrict ptr_db = d_b.data;
		float *restrict ptr_a = a.data;
		float *restrict ptr_b = b.data;

		// d_a = d_out @ b
		for (size_t i = 0; i < M; ++i) {
			for (size_t k = 0; k < P; ++k) {
				float acc = 0.0f;
				for (size_t j = 0; j < N; ++j) {
					acc += ptr_dout[i * sr_dout + j * sc_dout] * ptr_b[j * sr_b + k * sc_b];
				}
				ptr_da[i * sr_da + k * sc_da] += acc;
			}
		}

		// d_b = d_out^T @ a
		for (size_t j = 0; j < N; ++j) {
			for (size_t k = 0; k < P; ++k) {
				float acc = 0.0f;
				for (size_t i = 0; i < M; ++i) {
					acc += ptr_dout[i * sr_dout + j * sc_dout] * ptr_a[i * sr_a + k * sc_a];
				}
				ptr_db[j * sr_db + k * sc_db] += acc;
			}
		}

		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_matmul_t_grad(
			RAI_TENSOR_SUBTENSOR(d_a, i),
			RAI_TENSOR_SUBTENSOR(d_b, i),
			RAI_TENSOR_SUBTENSOR(d_out, i),
			RAI_TENSOR_SUBTENSOR(a, i),
			RAI_TENSOR_SUBTENSOR(b, i)
		);
	}
}

RaiTensorBinOpGrad rai_tensor_matmul_t_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b)
{
	RaiTensor a_p = a, b_p = b;
	// Promote the batch dimensions only
	RAI__TENSOR_BROADCAST_AND_PROMOTE(MatMulGradIn, a_p, b_p, RAI__TENSOR_MAXRANK - 2);

	RaiTensorBinOpGrad grad;
	// Allocate with ZEROES for broadcast accumulation
	grad.d_a = RAI_TENSOR_ALLOC_LIKE_FILL(arena_a, a, 0.0f);
	grad.d_b = RAI_TENSOR_ALLOC_LIKE_FILL(arena_b, b, 0.0f);

	RaiTensor d_a_p = grad.d_a, d_b_p = grad.d_b;
	// Promote the gradient accumulators
	RAI__TENSOR_BROADCAST_AND_PROMOTE(MatMulGradOut, d_a_p, d_b_p, RAI__TENSOR_MAXRANK - 2);

	RaiTensor d_out_p = d_out;
	d_out_p.rank = d_a_p.rank;

	rai__tensor_matmul_t_grad(d_a_p, d_b_p, d_out_p, a_p, b_p);

	// They are natively accumulated into their original shapes!
	grad.d_a = RAI_TENSOR_RESHAPE_LIKE(grad.d_a, a);
	grad.d_b = RAI_TENSOR_RESHAPE_LIKE(grad.d_b, b);

	return grad;
}

static void rai__tensor_rmsnorm_grad(RaiTensor d_in, RaiTensor d_w, RaiTensor d_out, RaiTensor in, RaiTensor weight, float eps)
{
	// base case
	if (d_out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = d_out.dims[idx];

		size_t s_din = d_in.strs[idx];
		size_t s_dw = d_w.strs[idx];
		size_t s_dout = d_out.strs[idx];
		size_t s_in = in.strs[idx];
		size_t s_w = weight.strs[idx];

		float sum_sq = 0.0f;
		for (size_t i = 0; i < N; ++i) {
			float x = in.data[i * s_in];
			sum_sq += x * x;
		}
		float mean_sq = sum_sq / N;
		float s = 1.0f / RAI_SQRTF(mean_sq + eps);

		float dot = 0.0f;
		for (size_t i = 0; i < N; ++i) {
			float dy = d_out.data[i * s_dout];
			float w = weight.data[i * s_w];
			float x = in.data[i * s_in];
			dot += dy * w * x;
		}

		float s_var = s * s / N; // precompute s^2 / N
		for (size_t i = 0; i < N; ++i) {
			float dy = d_out.data[i * s_dout];
			float w = weight.data[i * s_w];
			float x = in.data[i * s_in];

			// d_in = s * (dy * w - x * (s^2 / N) * dot)
			d_in.data[i * s_din] = s * (dy * w - x * s_var * dot);

			// d_w += dy * (x * s)
			d_w.data[i * s_dw] += dy * (x * s);
		}

		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_rmsnorm_grad(
			RAI_TENSOR_SUBTENSOR(d_in, i),
			RAI_TENSOR_SUBTENSOR(d_w, i),
			RAI_TENSOR_SUBTENSOR(d_out, i),
			RAI_TENSOR_SUBTENSOR(in, i),
			RAI_TENSOR_SUBTENSOR(weight, i),
			eps
		);
	}
}

RaiTensorBinOpGrad rai_tensor_rmsnorm_grad(RaiArena *arena_in, RaiArena *arena_weight, RaiTensor d_out, RaiTensor in, RaiTensor weight, float eps)
{
	RaiTensor in_p = in;
	RaiTensor w_p = weight;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(RMSNormGradIn, in_p, w_p, RAI__TENSOR_MAXRANK);

	RaiTensorBinOpGrad grad;

	grad.d_a = RAI_TENSOR_ALLOC_LIKE(arena_in, in);
	grad.d_b = RAI_TENSOR_ALLOC_LIKE_FILL(arena_weight, weight, 0.0f);

	RaiTensor d_in_p = grad.d_a;
	RaiTensor d_w_p = grad.d_b;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(RMSNormGradOut, d_in_p, d_w_p, RAI__TENSOR_MAXRANK);

	RaiTensor d_out_p = d_out;
	d_out_p.rank = d_in_p.rank;

	rai__tensor_rmsnorm_grad(d_in_p, d_w_p, d_out_p, in_p, w_p, eps);

	grad.d_a = RAI_TENSOR_RESHAPE_LIKE(grad.d_a, in);
	grad.d_b = RAI_TENSOR_RESHAPE_LIKE(grad.d_b, weight);

	return grad;
}

static void rai__tensor_add_grad(RaiTensor d_a, RaiTensor d_b, RaiTensor d_out)
{
	if (d_out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = d_out.dims[idx];

		size_t s_da = d_a.strs[idx];
		size_t s_db = d_b.strs[idx];
		size_t s_dout = d_out.strs[idx];

		for (size_t i = 0; i < N; ++i) {
			float g = d_out.data[i * s_dout];
			d_a.data[i * s_da] += g;
			d_b.data[i * s_db] += g;
		}
		return;
	}

	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_add_grad(RAI_TENSOR_SUBTENSOR(d_a, i), RAI_TENSOR_SUBTENSOR(d_b, i), RAI_TENSOR_SUBTENSOR(d_out, i));
	}
}

RaiTensorBinOpGrad rai_tensor_add_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b)
{
	RaiTensor a_p = a, b_p = b;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(AddGradIn, a_p, b_p, RAI__TENSOR_MAXRANK);

	RaiTensorBinOpGrad grad;
	// Allocate with ZEROES for broadcast accumulation
	grad.d_a = RAI_TENSOR_ALLOC_LIKE_FILL(arena_a, a, 0.0f);
	grad.d_b = RAI_TENSOR_ALLOC_LIKE_FILL(arena_b, b, 0.0f);

	RaiTensor d_a_p = grad.d_a, d_b_p = grad.d_b;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(AddGradOut, d_a_p, d_b_p, RAI__TENSOR_MAXRANK);

	RaiTensor d_out_p = d_out;
	d_out_p.rank = d_a_p.rank;

	rai__tensor_add_grad(d_a_p, d_b_p, d_out_p);

	grad.d_a = RAI_TENSOR_RESHAPE_LIKE(grad.d_a, a);
	grad.d_b = RAI_TENSOR_RESHAPE_LIKE(grad.d_b, b);
	return grad;
}

static void rai__tensor_sub_grad(RaiTensor d_a, RaiTensor d_b, RaiTensor d_out)
{
	if (d_out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = d_out.dims[idx];

		size_t s_da = d_a.strs[idx];
		size_t s_db = d_b.strs[idx];
		size_t s_dout = d_out.strs[idx];

		for (size_t i = 0; i < N; ++i) {
			float g = d_out.data[i * s_dout];
			d_a.data[i * s_da] += g;
			d_b.data[i * s_db] -= g;
		}
		return;
	}

	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_sub_grad(RAI_TENSOR_SUBTENSOR(d_a, i), RAI_TENSOR_SUBTENSOR(d_b, i), RAI_TENSOR_SUBTENSOR(d_out, i));
	}
}

RaiTensorBinOpGrad rai_tensor_sub_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b)
{
	RaiTensor a_p = a, b_p = b;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(AddGradIn, a_p, b_p, RAI__TENSOR_MAXRANK);

	RaiTensorBinOpGrad grad;
	// Allocate with ZEROES for broadcast accumulation
	grad.d_a = RAI_TENSOR_ALLOC_LIKE_FILL(arena_a, a, 0.0f);
	grad.d_b = RAI_TENSOR_ALLOC_LIKE_FILL(arena_b, b, 0.0f);

	RaiTensor d_a_p = grad.d_a, d_b_p = grad.d_b;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(AddGradOut, d_a_p, d_b_p, RAI__TENSOR_MAXRANK);

	RaiTensor d_out_p = d_out;
	d_out_p.rank = d_a_p.rank;

	rai__tensor_sub_grad(d_a_p, d_b_p, d_out_p);

	grad.d_a = RAI_TENSOR_RESHAPE_LIKE(grad.d_a, a);
	grad.d_b = RAI_TENSOR_RESHAPE_LIKE(grad.d_b, b);
	return grad;
}

static void rai__tensor_mul_grad(RaiTensor d_a, RaiTensor d_b, RaiTensor d_out, RaiTensor a, RaiTensor b)
{
	if (d_out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = d_out.dims[idx];

		size_t s_da = d_a.strs[idx];
		size_t s_db = d_b.strs[idx];
		size_t s_dout = d_out.strs[idx];
		size_t s_a = a.strs[idx];
		size_t s_b = b.strs[idx];

		for (size_t i = 0; i < N; ++i) {
			float g = d_out.data[i * s_dout];
			d_a.data[i * s_da] += g * b.data[i * s_b];
			d_b.data[i * s_db] += g * a.data[i * s_a];
		}
		return;
	}

	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_mul_grad(
			RAI_TENSOR_SUBTENSOR(d_a, i), RAI_TENSOR_SUBTENSOR(d_b, i),
			RAI_TENSOR_SUBTENSOR(d_out, i),
			RAI_TENSOR_SUBTENSOR(a, i), RAI_TENSOR_SUBTENSOR(b, i)
		);
	}
}

RaiTensorBinOpGrad rai_tensor_mul_grad(RaiArena *arena_a, RaiArena *arena_b, RaiTensor d_out, RaiTensor a, RaiTensor b)
{
	RaiTensor a_p = a, b_p = b;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(MulGradIn, a_p, b_p, RAI__TENSOR_MAXRANK);

	RaiTensorBinOpGrad grad;
	// Allocate with ZEROES for broadcast accumulation
	grad.d_a = RAI_TENSOR_ALLOC_LIKE_FILL(arena_a, a, 0.0f);
	grad.d_b = RAI_TENSOR_ALLOC_LIKE_FILL(arena_b, b, 0.0f);

	RaiTensor d_a_p = grad.d_a, d_b_p = grad.d_b;
	RAI__TENSOR_BROADCAST_AND_PROMOTE(MulGradOut, d_a_p, d_b_p, RAI__TENSOR_MAXRANK);

	RaiTensor d_out_p = d_out;
	d_out_p.rank = d_a_p.rank;

	rai__tensor_mul_grad(d_a_p, d_b_p, d_out_p, a_p, b_p);

	grad.d_a = RAI_TENSOR_RESHAPE_LIKE(grad.d_a, a);
	grad.d_b = RAI_TENSOR_RESHAPE_LIKE(grad.d_b, b);
	return grad;
}

static void rai__tensor_silu_grad(RaiTensor d_in, RaiTensor d_out, RaiTensor in)
{
	// base case
	if (d_out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = d_out.dims[idx];

		size_t s_din = d_in.strs[idx];
		size_t s_dout = d_out.strs[idx];
		size_t s_in = in.strs[idx];

		for (size_t i = 0; i < N; ++i) {
			float x = in.data[i * s_in];
			float dy = d_out.data[i * s_dout];

			float sig = 1.0f / (1.0f + RAI_EXPF(-x));
			float local_grad = sig * (1.0f + x * (1.0f - sig));

			d_in.data[i * s_din] = dy * local_grad;
		}
		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_silu_grad(RAI_TENSOR_SUBTENSOR(d_in, i), RAI_TENSOR_SUBTENSOR(d_out, i), RAI_TENSOR_SUBTENSOR(in, i));
	}
}

RaiTensor rai_tensor_silu_grad(RaiArena *arena, RaiTensor d_out, RaiTensor in)
{
	RaiTensor d_in = RAI_TENSOR_ALLOC_LIKE(arena, in);
	rai__tensor_silu_grad(d_in, d_out, in);
	return d_in;
}

static void rai__tensor_leaky_relu_grad(RaiTensor d_in, RaiTensor d_out, RaiTensor in, float leak)
{
	// base case
	if (d_out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = d_out.dims[idx];

		size_t s_din = d_in.strs[idx];
		size_t s_dout = d_out.strs[idx];
		size_t s_in = in.strs[idx];

		for (size_t i = 0; i < N; ++i) {
			float x = in.data[i * s_in];
			float dy = d_out.data[i * s_dout];

			float local_grad = x > 0.0f ? 1.0f : leak;

			d_in.data[i * s_din] = dy * local_grad;
		}
		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_leaky_relu_grad(RAI_TENSOR_SUBTENSOR(d_in, i), RAI_TENSOR_SUBTENSOR(d_out, i), RAI_TENSOR_SUBTENSOR(in, i), leak);
	}
}

RaiTensor rai_tensor_leaky_relu_grad(RaiArena *arena, RaiTensor d_out, RaiTensor in, float leak)
{
	RaiTensor d_in = RAI_TENSOR_ALLOC_LIKE(arena, in);
	rai__tensor_leaky_relu_grad(d_in, d_out, in, leak);
	return d_in;
}

RaiTensor rai_tensor_relu_grad(RaiArena *arena, RaiTensor d_out, RaiTensor in)
{
	return rai_tensor_leaky_relu_grad(arena, d_out, in, 0.0f);
}

static void rai__tensor_softmax_grad(RaiTensor d_in, RaiTensor d_out, RaiTensor probs)
{
	// base case
	if (d_out.rank <= 1) {
		size_t idx = RAI__TENSOR_MAXRANK - 1;
		size_t N = d_out.dims[idx];

		size_t s_din = d_in.strs[idx];
		size_t s_dout = d_out.strs[idx];
		size_t s_probs = probs.strs[idx];

		// Calculate the dot product: sum(dy_i * y_i)
		float dot = 0.0f;
		for (size_t i = 0; i < N; ++i) {
			float dy = d_out.data[i * s_dout];
			float y = probs.data[i * s_probs];
			dot += dy * y;
		}

		// Compute and write gradients: dx_i = y_i * (dy_i - dot)
		for (size_t i = 0; i < N; ++i) {
			float dy = d_out.data[i * s_dout];
			float y = probs.data[i * s_probs];

			d_in.data[i * s_din] = y * (dy - dot);
		}
		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t outer_dim = d_out.dims[outer_idx];

	for (size_t i = 0; i < outer_dim; ++i) {
		rai__tensor_softmax_grad(
			RAI_TENSOR_SUBTENSOR(d_in, i),
			RAI_TENSOR_SUBTENSOR(d_out, i),
			RAI_TENSOR_SUBTENSOR(probs, i)
		);
	}
}

RaiTensor rai_tensor_softmax_grad(RaiArena *arena, RaiTensor d_out, RaiTensor probs)
{
	RaiTensor d_in = RAI_TENSOR_ALLOC_LIKE(arena, probs);
	rai__tensor_softmax_grad(d_in, d_out, probs);
	return d_in;
}

RaiTensor rai_tensor_scale_grad(RaiArena *arena, RaiTensor d_out, float scale)
{
	return rai_tensor_scale(arena, d_out, scale);
}

static void rai__tensor_rope_grad(RaiTensor d_in, RaiTensor d_out, unsigned long start_idx, float theta_scale)
{
	// base case
	if (d_out.rank <= 2) {
		size_t r = RAI__TENSOR_MAXRANK;

		size_t d0 = d_out.dims[r - 1];
		size_t d1 = d_out.dims[r - 2];

		size_t s0 = d_out.strs[r - 1], s1 = d_out.strs[r - 2];
		size_t os0 = d_in.strs[r - 1], os1 = d_in.strs[r - 2];

		size_t half_d0 = d0 / 2;

		for (size_t i1 = 0; i1 < d1; ++i1) {
			unsigned long current_idx = start_idx + i1;

			// Stack-allocated trig cache
			float cos_v[half_d0], sin_v[half_d0];

			for (size_t k = 0; k < half_d0; ++k) {
				float freq = 1.0f / RAI_POWF(theta_scale, (float)(2 * k) / (float)d0);
				float alpha = (float)current_idx * freq;
				cos_v[k] = RAI_COSF(alpha);
				sin_v[k] = RAI_SINF(alpha);
			}

			float *src = d_out.data + (i1 * s1);
			float *dst = d_in.data + (i1 * os1);

			for (size_t k = 0; k < half_d0; ++k) {
				float v_l = src[k * s0];
				float v_r = src[(k + half_d0) * s0];

				float cv = cos_v[k];
				float sv = sin_v[k];

				// The inverse rotation: flip the signs on the sine terms
				dst[k * os0] = v_l * cv + v_r * sv;
				dst[(k + half_d0) * os0] = -v_l * sv + v_r * cv;
			}
		}
		return;
	}

	// recursive step
	size_t outer_idx = RAI__TENSOR_MAXRANK - d_out.rank;
	size_t loop_len = d_out.dims[outer_idx];

	for (size_t i = 0; i < loop_len; ++i) {
		rai__tensor_rope_grad(RAI_TENSOR_SUBTENSOR(d_in, i), RAI_TENSOR_SUBTENSOR(d_out, i), start_idx, theta_scale);
	}
}

RaiTensor rai_tensor_rope_grad(RaiArena *arena, RaiTensor d_out, unsigned long start_idx, float theta_scale)
{
	RaiTensor d_in = RAI_TENSOR_ALLOC_LIKE(arena, d_out);
	rai__tensor_rope_grad(d_in, d_out, start_idx, theta_scale);
	return d_in;
}

RaiModel rai_engine_backward(
	// Workspaces
	RaiArena *scratch_acts,
	RaiArena *scratch_grads,

	// Engine Context
	RaiEngine engine,

	// Forward Pass Activations
	RaiModelActs acts,

	// Loss Gradient
	RaiTensor d_logits, // [B, S, V]

	// Input Tokens
	size_t batch_size,
	size_t seq_len,
	int tokens[batch_size * seq_len],

	// Execution State
	size_t pos_offset
)
{
	size_t B = batch_size;
	size_t S = seq_len;
	size_t D = engine.model_config.embedding_length;
	size_t H = engine.model_config.attention_head_count;
	size_t D_head = D / H;
	float scale = 1.0f / RAI_SQRTF((float)D_head);
	long L = (long)engine.model_config.block_count; // signed for backwards indexing

	RaiModel grads = { 0 };

	// LM Head
	RaiTensorBinOpGrad d_output = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_logits, acts.output_norm, engine.weights.output);
	grads.output = d_output.d_b; // Direct assignment. Perfect shape [V, D].

	RaiTensorBinOpGrad d_out_norm = rai_tensor_rmsnorm_grad(scratch_acts, scratch_grads, d_output.d_a, acts.blk[L - 1].x_out, engine.weights.output_norm, engine.model_config.attention_layer_norm_rms_epsilon);
	grads.output_norm = d_out_norm.d_b;

	RaiTensor d_x = d_out_norm.d_a;

	// Transformer
	for (long l = L - 1; l >= 0; --l) {
		RaiTensor x_in = l == 0 ? acts.token_embd : acts.blk[l - 1].x_out;

		// --- FFN Block ---
		RaiTensorBinOpGrad d_ffn_res = rai_tensor_add_grad(scratch_acts, scratch_acts, d_x, acts.blk[l].x_mid, acts.blk[l].ffn_down);
		RaiTensor d_x_mid = d_ffn_res.d_a;
		RaiTensor d_ffn_down_out = d_ffn_res.d_b;

		RaiTensorBinOpGrad d_ffn_down = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_ffn_down_out, acts.blk[l].ffn_swiglu, engine.weights.blk[l].ffn_down);
		grads.blk[l].ffn_down = d_ffn_down.d_b;

		RaiTensorBinOpGrad d_swiglu = rai_tensor_mul_grad(scratch_acts, scratch_acts, d_ffn_down.d_a, acts.blk[l].ffn_silu, acts.blk[l].ffn_up);
		RaiTensor d_ffn_silu_out = d_swiglu.d_a;
		RaiTensor d_ffn_up_out = d_swiglu.d_b;

		RaiTensor d_ffn_gate_out = rai_tensor_silu_grad(scratch_acts, d_ffn_silu_out, acts.blk[l].ffn_gate);

		RaiTensorBinOpGrad d_ffn_up = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_ffn_up_out, acts.blk[l].ffn_norm, engine.weights.blk[l].ffn_up);
		grads.blk[l].ffn_up = d_ffn_up.d_b;

		RaiTensorBinOpGrad d_ffn_gate = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_ffn_gate_out, acts.blk[l].ffn_norm, engine.weights.blk[l].ffn_gate);
		grads.blk[l].ffn_gate = d_ffn_gate.d_b;

		RaiTensor d_ffn_norm_in = rai_tensor_add(scratch_acts, d_ffn_up.d_a, d_ffn_gate.d_a);

		RaiTensorBinOpGrad d_ffn_norm = rai_tensor_rmsnorm_grad(scratch_acts, scratch_grads, d_ffn_norm_in, acts.blk[l].x_mid, engine.weights.blk[l].ffn_norm, engine.model_config.attention_layer_norm_rms_epsilon);
		grads.blk[l].ffn_norm = d_ffn_norm.d_b;

		d_x = rai_tensor_add(scratch_acts, d_x_mid, d_ffn_norm.d_a);

		// --- Attention Block ---
		RaiTensorBinOpGrad d_attn_res = rai_tensor_add_grad(scratch_acts, scratch_acts, d_x, x_in, acts.blk[l].attn_output);
		RaiTensor d_x_attn_mid = d_attn_res.d_a;
		RaiTensor d_attn_out_in = d_attn_res.d_b;

		RaiTensorBinOpGrad d_attn_output = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_attn_out_in, acts.blk[l].attn_ctx, engine.weights.blk[l].attn_output);
		grads.blk[l].attn_output = d_attn_output.d_b;

		RaiTensor d_attn_ctx_split = RAI_TENSOR_RESHAPE(d_attn_output.d_a, B, S, H, D_head);
		RaiTensor d_attn_ctx_heads = rai_tensor_transpose(d_attn_ctx_split, 1, 2);

		RaiTensorBinOpGrad d_sdp = rai_tensor_matmul_t_grad(scratch_acts, scratch_acts, d_attn_ctx_heads, acts.blk[l].attn_probs, acts.blk[l].attn_v_heads_t);
		RaiTensor d_attn_probs = d_sdp.d_a;
		RaiTensor d_attn_v_heads_t = d_sdp.d_b;

		RaiTensor d_attn_v_heads = rai_tensor_transpose(d_attn_v_heads_t, 2, 3);
		RaiTensor d_attn_v_split = rai_tensor_transpose(d_attn_v_heads, 1, 2);
		RaiTensor d_attn_v = RAI_TENSOR_RESHAPE(d_attn_v_split, B, S, D);

		RaiTensor d_attn_scores = rai_tensor_softmax_grad(scratch_acts, d_attn_probs, acts.blk[l].attn_probs);
		RaiTensor d_attn_qkt = rai_tensor_scale_grad(scratch_acts, d_attn_scores, scale);

		RaiTensorBinOpGrad d_qk = rai_tensor_matmul_t_grad(scratch_acts, scratch_acts, d_attn_qkt, acts.blk[l].attn_q_heads, acts.blk[l].attn_k_heads);
		RaiTensor d_attn_q_heads = d_qk.d_a;
		RaiTensor d_attn_k_heads = d_qk.d_b;

		RaiTensor d_attn_q_split = rai_tensor_transpose(d_attn_q_heads, 1, 2);
		RaiTensor d_attn_k_split = rai_tensor_transpose(d_attn_k_heads, 1, 2);
		RaiTensor d_attn_q_rope = RAI_TENSOR_RESHAPE(d_attn_q_split, B, S, D);
		RaiTensor d_attn_k_rope = RAI_TENSOR_RESHAPE(d_attn_k_split, B, S, D);

		RaiTensor d_attn_q = rai_tensor_rope_grad(scratch_acts, d_attn_q_rope, pos_offset, engine.model_config.rope_freq_base);
		RaiTensor d_attn_k = rai_tensor_rope_grad(scratch_acts, d_attn_k_rope, pos_offset, engine.model_config.rope_freq_base);

		RaiTensorBinOpGrad d_q = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_attn_q, acts.blk[l].attn_norm, engine.weights.blk[l].attn_q);
		grads.blk[l].attn_q = d_q.d_b;

		RaiTensorBinOpGrad d_k = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_attn_k, acts.blk[l].attn_norm, engine.weights.blk[l].attn_k);
		grads.blk[l].attn_k = d_k.d_b;

		RaiTensorBinOpGrad d_v = rai_tensor_matmul_t_grad(scratch_acts, scratch_grads, d_attn_v, acts.blk[l].attn_norm, engine.weights.blk[l].attn_v);
		grads.blk[l].attn_v = d_v.d_b;

		RaiTensor d_attn_norm_in = rai_tensor_add(scratch_acts, d_q.d_a, d_k.d_a);
		d_attn_norm_in = rai_tensor_add(scratch_acts, d_attn_norm_in, d_v.d_a);

		RaiTensorBinOpGrad d_attn_norm = rai_tensor_rmsnorm_grad(scratch_acts, scratch_grads, d_attn_norm_in, x_in, engine.weights.blk[l].attn_norm, engine.model_config.attention_layer_norm_rms_epsilon);
		grads.blk[l].attn_norm = d_attn_norm.d_b;

		d_x = rai_tensor_add(scratch_acts, d_x_attn_mid, d_attn_norm.d_a);
	}

	// We MUST allocate this one manually with ZEROES because we are routing
	// gradients to specific rows based on the input tokens.
	grads.token_embd = RAI_TENSOR_ALLOC_FILL(scratch_grads, 0.0f, engine.model_config.vocab_size, D);

	size_t s_embd = grads.token_embd.strs[RAI__TENSOR_MAXRANK - 2];
	size_t s_dx = d_x.strs[RAI__TENSOR_MAXRANK - 1];

	// Do not paralleize without atmoic accumulation
	for (size_t b = 0; b < B; ++b) {
		for (size_t s = 0; s < S; ++s) {
			int token = tokens[b * seq_len + s];
			float *dst_row = grads.token_embd.data + (token * s_embd);
			float *src_row = RAI_TENSOR_SUBTENSOR(d_x, b, s).data;

			// In-place raw pointer accumulation to avoid arena bloat
			for (size_t d = 0; d < D; ++d) {
				dst_row[d] += src_row[d * s_dx];
			}
		}
	}

	return grads;
}

RaiEngine rai_engine_create(
	// Configurations
	size_t memory_capacity,
	RaiModelConfig model_config,
	RaiAdamWConfig adamw_config
)
{
	// Important to zero out
	// So that the unused Tensors can be identified later
	RaiEngine engine = { 0 };

	engine.arena = rai_arena_create(memory_capacity);
	engine.model_config = model_config;
	engine.adamw_config = adamw_config;
	engine.weights = rai_model_alloc_weights(&engine.arena, model_config);
	engine.adamw.means = rai_model_alloc_zeroed(&engine.arena, model_config);
	engine.adamw.uvars = rai_model_alloc_zeroed(&engine.arena, model_config);
	engine.adamw.time = 0;

	return engine;
}

void rai_engine_destroy(
	// Mutating Object
	RaiEngine *engine
)
{
	rai_arena_destroy(&engine->arena);
	RAI_MEMSET(engine, 0, sizeof(*engine));
}

static void rai__adamw_update(RaiAdamWConfig adamw_config, float m_corr, float v_corr, RaiTensor w, RaiTensor g, RaiTensor m, RaiTensor v)
{
	float b1 = adamw_config.beta1;
	float b2 = adamw_config.beta2;

	for (size_t i = 0; i < w.count; ++i) {
		float grad = g.data[i];

		// Update moments
		m.data[i] = b1 * m.data[i] + (1.0f - b1) * grad;
		v.data[i] = b2 * v.data[i] + (1.0f - b2) * (grad * grad);

		// Bias correction
		float m_hat = m.data[i] * m_corr;
		float v_hat = v.data[i] * v_corr;

		// Weight Decay + Adam Update
		float update = m_hat / (RAI_SQRTF(v_hat) + adamw_config.eps);
		w.data[i] -= adamw_config.lr * (update + adamw_config.weight_decay * w.data[i]);
	}
}

void rai_engine_step(
	// Mutating Object
	RaiEngine *engine,

	// Gradients
	RaiModel grads
)
{
	engine->adamw.time++;

	float m_corr = 1.0f / (1.0f - RAI_POWF(engine->adamw_config.beta1, (float)engine->adamw.time));
	float v_corr = 1.0f / (1.0f - RAI_POWF(engine->adamw_config.beta2, (float)engine->adamw.time));

	for (size_t i = 0; i < RAI_MODEL_TENSOR_COUNT; ++i) {
		if (engine->weights.array[i].data == nullptr)
			continue;

		rai__adamw_update(engine->adamw_config, m_corr, v_corr, engine->weights.array[i], grads.array[i], engine->adamw.means.array[i], engine->adamw.uvars.array[i]);
	}
}

RaiTokenizer rai_tokenizer_train(RaiArena *arena, RaiArena *scratch, RaiString text, size_t target_vocab_size)
{
	RAI_ASSERT(target_vocab_size >= 256 && "Vocab size must be at least 256");

	RaiTokenizer tok = { 0 };
	tok.vocab_size = target_vocab_size;
	tok.merges = (RaiMerge *)rai_arena_alloc(arena, (target_vocab_size - 256) * sizeof(RaiMerge));
	tok.vocab = (RaiString *)rai_arena_alloc(arena, target_vocab_size * sizeof(RaiString));

	// Initialize Base Vocabulary (0-255 raw bytes)
	for (size_t i = 0; i < 256; ++i) {
		tok.vocab[i].data = (char *)rai_arena_alloc(arena, 1);
		tok.vocab[i].data[0] = (char)i;
		tok.vocab[i].length = 1;
	}

	size_t num_tokens = text.length;
	int *tokens = (int *)rai_arena_alloc(scratch, num_tokens * sizeof(int));
	for (size_t i = 0; i < num_tokens; ++i) {
		tokens[i] = (unsigned char)text.data[i];
	}

	int *counts = (int *)rai_arena_alloc(scratch, target_vocab_size * target_vocab_size * sizeof(int));
	RAI_MEMSET(counts, 0, target_vocab_size * target_vocab_size * sizeof(int));

	for (size_t new_id = 256; new_id < target_vocab_size; ++new_id) {
		int best_left = -1, best_right = -1;
		int max_count = -1;

		for (size_t i = 0; i < num_tokens - 1; ++i) {
			int left = tokens[i];
			int right = tokens[i + 1];
			size_t idx = (size_t)left * target_vocab_size + right;

			counts[idx]++;
			if (counts[idx] > max_count) {
				max_count = counts[idx];
				best_left = left;
				best_right = right;
			}
		}

		for (size_t i = 0; i < num_tokens - 1; ++i) {
			int left = tokens[i];
			int right = tokens[i + 1];
			size_t idx = (size_t)left * target_vocab_size + right;
			counts[idx] = 0;
		}

		if (max_count < 1) {
			tok.vocab_size = new_id;
			break;
		}

		tok.merges[new_id - 256].left = best_left;
		tok.merges[new_id - 256].right = best_right;

		size_t len_l = tok.vocab[best_left].length;
		size_t len_r = tok.vocab[best_right].length;
		tok.vocab[new_id].length = len_l + len_r;
		tok.vocab[new_id].data = (char *)rai_arena_alloc(arena, len_l + len_r);
		RAI_MEMCPY(tok.vocab[new_id].data, tok.vocab[best_left].data, len_l);
		RAI_MEMCPY(tok.vocab[new_id].data + len_l, tok.vocab[best_right].data, len_r);

		size_t write_idx = 0;
		for (size_t i = 0; i < num_tokens; ++i) {
			if (i < num_tokens - 1 && tokens[i] == best_left && tokens[i + 1] == best_right) {
				tokens[write_idx++] = new_id;
				i++;
			} else {
				tokens[write_idx++] = tokens[i];
			}
		}
		num_tokens = write_idx;
	}

	return tok;
}

RaiTokenArray rai_tokenizer_encode(RaiArena *arena, RaiTokenizer tok, RaiString text)
{
	RaiTokenArray out = { 0 };
	if (text.length == 0)
		return out;

	int *tokens = (int *)rai_arena_alloc(arena, text.length * sizeof(int));
	for (size_t i = 0; i < text.length; ++i) {
		tokens[i] = (unsigned char)text.data[i];
	}

	size_t current_len = text.length;
	size_t num_merges = tok.vocab_size - 256;

	for (size_t m = 0; m < num_merges; ++m) {
		int target_left = tok.merges[m].left;
		int target_right = tok.merges[m].right;
		int replacement = 256 + (int)m;

		size_t write_idx = 0;
		for (size_t i = 0; i < current_len; ++i) {
			if (i < current_len - 1 && tokens[i] == target_left && tokens[i + 1] == target_right) {
				tokens[write_idx++] = replacement;
				i++;
			} else {
				tokens[write_idx++] = tokens[i];
			}
		}
		current_len = write_idx;
	}

	out.count = current_len;
	out.data = tokens;
	return out;
}

RaiString rai_tokenizer_decode(RaiArena *arena, RaiTokenizer tok, RaiTokenArray tokens)
{
	RaiString out = { 0 };
	if (tokens.count == 0)
		return out;

	size_t total_len = 0;
	for (size_t i = 0; i < tokens.count; ++i) {
		RAI_ASSERT((size_t)tokens.data[i] < tok.vocab_size && "Decode ID out of bounds");
		total_len += tok.vocab[tokens.data[i]].length;
	}

	char *text = (char *)rai_arena_alloc(arena, total_len + 1);
	size_t cursor = 0;
	for (size_t i = 0; i < tokens.count; ++i) {
		size_t tlen = tok.vocab[tokens.data[i]].length;
		RAI_MEMCPY(text + cursor, tok.vocab[tokens.data[i]].data, tlen);
		cursor += tlen;
	}

	text[total_len] = '\0'; // Null-terminate for ease of standard C printing

	out.length = total_len;
	out.data = text;
	return out;
}

float rai_lr_cosine(float step, float warmup_steps, float max_steps, float max_lr, float min_lr)
{
	// Linear Warmup Phase
	if (step < warmup_steps) {
		return max_lr * (step / warmup_steps);
	}
	// Minimum LR Floor
	if (step > max_steps) {
		return min_lr;
	}
	// Cosine Decay Phase
	float decay_ratio = (step - warmup_steps) / (max_steps - warmup_steps);
	float coeff = 0.5f * (1.0f + RAI_COSF(RAI_PIF * decay_ratio));
	return min_lr + coeff * (max_lr - min_lr);
}

RaiMlpLayer rai_mlp_layer_alloc_randn(RaiArena* arena, size_t num_inputs, size_t num_outputs) {
	RaiMlpLayer layer = { 0 };

	float std_dev = 1.0f / RAI_SQRTF((float)num_inputs);
	layer.weight = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, std_dev, num_outputs, num_inputs);
	layer.bias   = RAI_TENSOR_ALLOC_RANDN(arena, 0.0f, std_dev, num_outputs);

	return layer;
}

RaiMlpLayerActs rai_mlp_layer_forward(RaiArena* arena, RaiMlpLayer layer, RaiTensor in) {
	RaiMlpLayerActs acts = { 0 };

	acts.prod = rai_tensor_matmul_t(arena, in, layer.weight);
	acts.sum = rai_tensor_add(arena, acts.prod, layer.bias);
	acts.out = rai_tensor_silu(arena, acts.sum);

	return acts;
}

RaiMlpLayerGrad rai_mlp_layer_backward(
	RaiArena* arena,
	RaiArena* scratch,
	RaiMlpLayer layer,
	RaiMlpLayerActs acts,
	RaiTensor d_out,
	RaiTensor in
) {
	RaiMlpLayerGrad grad = { 0 };

	RaiTensor d_sum = rai_tensor_silu_grad(scratch, d_out, acts.sum);
	RaiTensorBinOpGrad g_sum = rai_tensor_add_grad(scratch, arena, d_sum, acts.prod, layer.bias);
	RaiTensor d_prod = g_sum.d_a;
	grad.d_bias = g_sum.d_b;
	RaiTensorBinOpGrad g_prod = rai_tensor_matmul_t_grad(scratch, arena, d_prod, in, layer.weight);
	grad.d_weight = g_prod.d_b;

	return grad;
}

#endif // RRIFTT_AI_IMPLEMENTATION

