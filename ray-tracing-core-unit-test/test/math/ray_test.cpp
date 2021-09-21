#include "ray_test.h"
#include <math/ray.h>
#include <cute.h>

namespace math
{
	void ray_new_ray_test(void)
	{
		auto ray = ray_tracing_core::math::Ray::new_ray(
			ray_tracing_core::math::Point3D(1, 2, 3),
			ray_tracing_core::math::Vector3D(4, 5, 6));
		ASSERT_EQUAL(1, ray.origin.x);
		ASSERT_EQUAL(2, ray.origin.y);
		ASSERT_EQUAL(3, ray.origin.z);
		ASSERT_EQUAL(4, ray.direction.x);
		ASSERT_EQUAL(5, ray.direction.y);
		ASSERT_EQUAL(6, ray.direction.z);
	}
}
