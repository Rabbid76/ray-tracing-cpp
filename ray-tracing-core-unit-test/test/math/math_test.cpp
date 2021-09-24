#include "math_test.h"
#include "ray_test.h"
#include "axis_aligned_bounding_box_test.h"

namespace math
{
	void add_math_unit_test(cute::suite &suite)
	{
		suite.push_back(CUTE(math::ray_point_at_test));
		suite.push_back(CUTE(math::aabb_new_box_test));
		suite.push_back(CUTE(math::aabb_hit_test));
		suite.push_back(CUTE(math::aabb_or_point_test));
	}
}
