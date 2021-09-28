#include "random_test.h"
#include <math/random.h>
#include <cute.h>
#include <cmath>

namespace math
{
	using namespace ray_tracing_core::math;

	const int test_iterations = 10;

	void random_size_test(void)
	{
		for (int iteration = 0; iteration < test_iterations; ++iteration)
		{
			auto actual_random_value = RandomGenerator().random_size();
			ASSERT_EQUAL_DELTA(0.5, actual_random_value, 0.5);
		}
	}

	void extern random_vector_to_sphere(void)
	{
		for (int iteration = 0; iteration < test_iterations; ++iteration)
		{
			auto actual_random_vector = RandomGenerator().random_vector_to_sphere(1, 1);
			auto actual_length = std::sqrt(dot(actual_random_vector, actual_random_vector));
			ASSERT_EQUAL_DELTA(1.0, actual_length, 0.001);
		}
	}
}
