#include "ortho_normal_base_test.h"
#include <math/ortho_normal_base.h>
#include <cute.h>
#include <test_assert_helper.h>
#include <vector>
#include <tuple>

namespace math
{
	using namespace ray_tracing_core::math;

	void ortho_normal_base_form_normal_test(void)
	{
		std::vector<std::tuple<Vector3D, Vector3D, Vector3D, Vector3D>> test_data
		{
			{Vector3D(-1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1), Vector3D(0, 0, 1)},
			{Vector3D(-1, 0, 0), Vector3D(0, 0, -1), Vector3D(0, 1, 0), Vector3D(0, 1, 0)},
			{Vector3D(0, -1, 0), Vector3D(0, 0, 1), Vector3D(1, 0, 0), Vector3D(1, 0, 0)}
		};

		for (auto [expected_u, expected_v, expected_w, normal_vector] : test_data)
		{
			auto actual_ortho_normal_base = OrthoNormalBase::from_normal(normal_vector);
			assert_equal_vector(expected_u, actual_ortho_normal_base.u(), 0.001);
			assert_equal_vector(expected_v, actual_ortho_normal_base.v(), 0.001);
			assert_equal_vector(expected_w, actual_ortho_normal_base.w(), 0.001);
		}
	}

	void ortho_normal_base_taransform(void)
	{
		std::vector<std::tuple<Vector3D, Vector3D, Vector3D>> test_data
		{
			{Vector3D(0, 0, 1), Vector3D(0, 0, 1), Vector3D(0, 0, 1)},
			{Vector3D(0, 1, 0), Vector3D(0, 0, 1), Vector3D(0, 1, 0)},
			{Vector3D(-1, 0, 0), Vector3D(0, 0, 1), Vector3D(1, 0, 0)},
			{Vector3D(-1, 2, 3), Vector3D(0, 0, 1), Vector3D(1, 2, 3)},
			{Vector3D(-1, 2, -3), Vector3D(0, 1, 0), Vector3D(1, 2, 3)},
			{Vector3D(1, -2, 3), Vector3D(1, 0, 0), Vector3D(1, 2, 3)},
		};

		for (auto [expected_vector, normal_vector, source_vector] : test_data)
		{
			auto actual_vector = OrthoNormalBase::from_normal(normal_vector)
				.transform(source_vector);
			assert_equal_vector(expected_vector, actual_vector, 0.001);
		}
	}
}
