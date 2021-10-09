#include "cosine_pdf_test.h"
#include "pdf/cosine_pdf.h"
#include <math/ortho_normal_base.h>
#include <test_assert_helper.h>
#include <vector>
#include <tuple>

namespace pdf
{
	using namespace ray_tracing_core::pdf;
	using namespace ray_tracing_core;

	void cosine_pdf_test(void)
	{
		const int test_iterations = 10;

		std::vector<std::tuple<math::Distance, math::Vector3D, math::Vector3D, math::Vector3D>> test_data
		{
			{1/math::pi<math::Distance>, math::Vector3D(0, 0, 1), math::Vector3D(0, 0, 1), math::Vector3D(0, 0, 1)}
		};

		for (auto [expected_value, expected_vector, normal_vector, direction_vector] : test_data)
		{
			for (int iteration = 0; iteration < test_iterations; ++iteration)
			{
				auto cosine_pdf = CosinePDF::from_normal(normal_vector);
				auto actual_value = cosine_pdf.value(direction_vector);
				auto actual_vector = cosine_pdf.generate();
				ASSERT_EQUAL_DELTA(expected_value, actual_value, 0.001);
				assert_equal_vector(expected_vector, actual_vector, 2);
			}
		}
	}
}
