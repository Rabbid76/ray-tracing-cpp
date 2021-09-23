#include "axis_aligned_bounding_box_test.h"
#include "cute.h"
#include <math/axis_aligned_bounding_box.h>
#include <tuple>
#include <vector>

using namespace ray_tracing_core::math;

namespace math
{
	void aabb_new_box_test(void)
	{
		std::vector<std::tuple<Point3D, Point3D, Point3D, Point3D>> test_data =
		{
			{ Point3D(1, 2, 3), Point3D(4, 5, 6), Point3D(1, 2, 3), Point3D(4, 5, 6)},
			{ Point3D(1, 2, 3), Point3D(4, 5, 6), Point3D(4, 2, 3), Point3D(1, 5, 6)},
			{ Point3D(1, 2, 3), Point3D(4, 5, 6), Point3D(1, 5, 3), Point3D(4, 2, 6)},
			{ Point3D(1, 2, 3), Point3D(4, 5, 6), Point3D(1, 2, 6), Point3D(4, 5, 3)},
		};

		for (auto [expected_minimum, expected_maximum, first_point, second_point] : test_data)
		{
			auto actual_box = AxisAlignedBoundingBox::new_box(first_point, second_point);
			ASSERT_EQUAL_DELTA(expected_minimum.x, actual_box.minimum_point.x, 0.001);
			ASSERT_EQUAL_DELTA(expected_minimum.y, actual_box.minimum_point.y, 0.001);
			ASSERT_EQUAL_DELTA(expected_minimum.z, actual_box.minimum_point.z, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.x, actual_box.maximum_point.x, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.y, actual_box.maximum_point.y, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.z, actual_box.maximum_point.z, 0.001);
		}
	}
}
