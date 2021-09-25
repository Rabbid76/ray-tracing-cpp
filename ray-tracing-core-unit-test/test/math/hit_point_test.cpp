#include "hit_point_test.h"
#include <cute.h>
#include <math/hit_point.h>

namespace math
{
	using namespace ray_tracing_core::math;

	void hit_point_new_hit_test(void)
	{
		auto hit_point = HitPoint::new_hit(1, Point3D(2, 3, 4), Vector3D(5, 6, 7));
		ASSERT_EQUAL(1, hit_point.distance);
		ASSERT_EQUAL(2, hit_point.position.x);
		ASSERT_EQUAL(3, hit_point.position.y);
		ASSERT_EQUAL(4, hit_point.position.z);
		ASSERT_EQUAL(5, hit_point.normal.x);
		ASSERT_EQUAL(6, hit_point.normal.y);
		ASSERT_EQUAL(7, hit_point.normal.z);
	}
}
