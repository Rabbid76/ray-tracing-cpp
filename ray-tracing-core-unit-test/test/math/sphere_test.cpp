#include "sphere_test.h"
#include <math/sphere.h>
#include "cute.h"
#include <tuple>
#include <vector>

namespace math
{
	using namespace ray_tracing_core::math;

	void sphere_bounding_box_test(void)
	{
		std::vector<std::tuple<Point3D, Point3D, Point3D, Distance>> test_data
		{
			{Point3D(-3, -2, -1), Point3D(5, 6, 7), Point3D(1, 2, 3), 4},
		};

		for (auto [expected_minimum, expected_maximum, center, radius] : test_data)
		{
			auto actual_box = Sphere(center, radius)
				.bounding_box();
			ASSERT_EQUAL_DELTA(expected_minimum.x, actual_box.minimum_point.x, 0.001);
			ASSERT_EQUAL_DELTA(expected_minimum.y, actual_box.minimum_point.y, 0.001);
			ASSERT_EQUAL_DELTA(expected_minimum.z, actual_box.minimum_point.z, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.x, actual_box.maximum_point.x, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.y, actual_box.maximum_point.y, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.z, actual_box.maximum_point.z, 0.001);
		}
	}

	void sphere_hit_test(void)
	{

	}
}
