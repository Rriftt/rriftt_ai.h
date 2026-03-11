#define RAI_IMPLEMENTATION
#include "../rriftt_ai.h"

#include <stdio.h>

#define NUM_SAMPLES 1000000
#define NUM_BINS 40
#define RANGE_MIN -4.0f
#define RANGE_MAX 4.0f

int main()
{
	RAI_SRAND(42);

	float target_mean = 0.0f;
	float target_std = 1.0f;

	double sum = 0.0;
	double sum_sq = 0.0;
	int bins[NUM_BINS] = { 0 };

	for (size_t i = 0; i < NUM_SAMPLES; ++i) {
		float val = rai_randn(target_mean, target_std);

		sum += val;
		sum_sq += (val * val);

		int bin_idx = (int)(((val - RANGE_MIN) / (RANGE_MAX - RANGE_MIN)) * NUM_BINS);
		if (bin_idx >= 0 && bin_idx < NUM_BINS) {
			bins[bin_idx]++;
		}
	}

	double actual_mean = sum / NUM_SAMPLES;
	double actual_variance = (sum_sq / NUM_SAMPLES) - (actual_mean * actual_mean);
	double expected_variance = (double)(target_std * target_std);

	printf("Samples: %d\n", NUM_SAMPLES);
	printf("Target Mean: %f, Target Var: %f\n", (double)target_mean, expected_variance);
	printf("Actual Mean: %f, Actual Var: %f\n\n", actual_mean, actual_variance);

	int max_bin_val = 0;
	for (int i = 0; i < NUM_BINS; ++i) {
		if (bins[i] > max_bin_val) {
			max_bin_val = bins[i];
		}
	}

	for (int i = 0; i < NUM_BINS; ++i) {
		float bin_edge = RANGE_MIN + i * ((RANGE_MAX - RANGE_MIN) / NUM_BINS);
		printf("%5.2f | ", bin_edge);

		int bar_length = (int)(((float)bins[i] / max_bin_val) * 60);
		for (int j = 0; j < bar_length; ++j) {
			putchar('#');
		}
		putchar('\n');
	}

	return 0;
}
