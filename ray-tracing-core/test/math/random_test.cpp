#include "random_test.h"
#include <math/random.h>
#include <rtc_core_test_assert.h>
#include <cmath>

namespace ray_tracing_core_unit_test
{
    namespace math
    {
        using namespace ray_tracing_core::math;

        const int test_iterations = 10;

        void random_size_test(void)
        {
            for (int iteration = 0; iteration < test_iterations; ++iteration)
            {
                auto actual_random_value = RandomGenerator().random_size();
                TEST_ASSERT_EQUAL_DELTA(0.5, actual_random_value, 0.5);
            }
        }

        void random_unit_test(void)
        {
            for (int iteration = 0; iteration < test_iterations; ++iteration)
            {
                auto actual_random_value = RandomGenerator().random_unit();
                TEST_ASSERT_EQUAL_DELTA(0.0, actual_random_value, 1.0);
            }
        }

        void random_cosine_direction_test(void)
        {
            for (int iteration = 0; iteration < test_iterations; ++iteration)
            {
                auto actual_random_vector = RandomGenerator().random_cosine_direction();
                auto actual_length = std::sqrt(dot(actual_random_vector, actual_random_vector));
                TEST_ASSERT_EQUAL_DELTA(1.5, actual_length, 0.5);
            }
        }

        void random_vector_to_sphere_test(void)
        {
            for (int iteration = 0; iteration < test_iterations; ++iteration)
            {
                auto actual_random_vector = RandomGenerator().random_vector_to_sphere(1, 1);
                auto actual_length = std::sqrt(dot(actual_random_vector, actual_random_vector));
                TEST_ASSERT_EQUAL_DELTA(1.0, actual_length, 0.001);
            }
        }
    }
}
