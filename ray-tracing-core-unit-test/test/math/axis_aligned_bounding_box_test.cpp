#include "axis_aligned_bounding_box_test.h"
#include "cute.h"
#include <math/axis_aligned_bounding_box.h>
#include <math/ray.h>
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

	void aabb_hit_test(void)
	{
		std::vector<std::tuple<bool, Point3D, Vector3D, Point3D, Point3D, DistanceRange>> test_data
		{
			{true, Point3D(0, 0, 0), Vector3D(1, 0, 0), Point3D(3, -1, -1), Point3D(5, 1, 1), {2, 6}},
			{true, Point3D(0, 0, 0), Vector3D(1, 0, 0), Point3D(3, -1, -1), Point3D(5, 1, 1), {2, 4}},
			{true, Point3D(0, 0, 0), Vector3D(1, 0, 0), Point3D(3, -1, -1), Point3D(5, 1, 1), {4, 6}},
			{false, Point3D(0, 0, 0), Vector3D(1, 0, 0), Point3D(3, -1, -1), Point3D(5, 1, 1), {1, 2}},
			{false, Point3D(0, 0, 0), Vector3D(1, 0, 0), Point3D(3, -1, -1), Point3D(5, 1, 1), {6, 7}},
			{false, Point3D(0, 2, 0), Vector3D(1, 0, 0), Point3D(3, -1, -1), Point3D(5, 1, 1), {6, 7}},
			{true, Point3D(-3, -3, 0), Vector3D(1, 1, 0), Point3D(-1, -1, -1), Point3D(1, 1, 1), {2, 6}},
			{true, Point3D(-3, -3, -3), Vector3D(1, 1, 1), Point3D(-1, -1, -1), Point3D(1, 1, 1), {2, 6}},
		};

		for (auto [expected_result, origin, direction, box_minimum, box_maximum, range] : test_data)
		{
			auto actual_result = AxisAlignedBoundingBox::new_box(box_minimum, box_maximum)
				.hit(Ray::new_ray(origin, direction), range);
			ASSERT_EQUAL(expected_result, actual_result);
		}
	}

	void aabb_or_point_test(void)
	{
		std::vector<std::tuple<Point3D, Point3D, Point3D, Point3D, Point3D>> test_data
		{
			{Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(4, 5, 6)},
			{Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(4, 2, 3), Point3D(7, 8, 9), Point3D(1, 5, 6)},
			{Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(1, 5, 3), Point3D(7, 8, 9), Point3D(4, 2, 6)},
			{Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(1, 2, 6), Point3D(7, 8, 9), Point3D(4, 5, 3)},
			{Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(1, 2, 3), Point3D(4, 8, 9), Point3D(7, 5, 6)},
			{Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(1, 2, 3), Point3D(7, 5, 9), Point3D(4, 8, 6)},
			{Point3D(1, 2, 3), Point3D(7, 8, 9), Point3D(1, 2, 3), Point3D(7, 8, 6), Point3D(4, 5, 9)},
		};

		for (auto [expected_minimum, expected_maximum, box_minimum, box_maximum, point] : test_data)
		{
			auto actual_box = AxisAlignedBoundingBox::new_box(box_minimum, box_maximum);
			actual_box |= point;
			ASSERT_EQUAL_DELTA(expected_minimum.x, actual_box.minimum_point.x, 0.001);
			ASSERT_EQUAL_DELTA(expected_minimum.y, actual_box.minimum_point.y, 0.001);
			ASSERT_EQUAL_DELTA(expected_minimum.z, actual_box.minimum_point.z, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.x, actual_box.maximum_point.x, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.y, actual_box.maximum_point.y, 0.001);
			ASSERT_EQUAL_DELTA(expected_maximum.z, actual_box.maximum_point.z, 0.001);
		}
	}
}
