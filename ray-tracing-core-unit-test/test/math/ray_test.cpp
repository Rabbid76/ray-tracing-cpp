#include "ray_test.h"
#include <math/ray.h>
#include <cute.h>
#include <tuple>
#include <vector>

using namespace ray_tracing_core::math;

namespace math
{
	void ray_point_at_test(void)
	{
		std::vector<std::tuple<Point3D, Point3D, Vector3D, Distance>> test_data =
		{
			{ Point3D(9, 12, 15), Point3D(1, 2, 3), Vector3D(4, 5, 6), 2},
			{ Point3D(1, 2, 3), Point3D(1, 2, 3), Vector3D(0, 0, 0), 2},
			{ Point3D(1, 2, 3), Point3D(1, 2, 3), Vector3D(4, 5, 6), 0},
			{ Point3D(8, 10, 12), Point3D(0, 0, 0), Vector3D(4, 5, 6), 2},
			{ Point3D(0, 0, 0), Point3D(0, 0, 0), Vector3D(0, 0, 0), 2},
			{ Point3D(0, 0, 0), Point3D(0, 0, 0), Vector3D(4, 5, 6), 0}
		};

		for (auto [expected_point, origin, direction, distance] : test_data)
		{
			auto actual_point = Ray::new_ray(origin, direction).point_at(distance);
			ASSERT_EQUAL_DELTA(expected_point.x, actual_point.x, 0.001);
			ASSERT_EQUAL_DELTA(expected_point.y, actual_point.y, 0.001);
			ASSERT_EQUAL_DELTA(expected_point.z, actual_point.z, 0.001);
		}
	}
}
