#include "math_test.h"
#include "ray_test.h"
#include "hit_point_test.h"
#include "axis_aligned_bounding_box_test.h"
#include "sphere_test.h"
#include "ortho_normal_base_test.h"
#include "random_test.h"

namespace math
{
	void add_math_unit_test(cute::suite &suite)
	{
		suite.push_back(CUTE(ray_point_at_test));
		suite.push_back(CUTE(hit_point_new_hit_test));
		suite.push_back(CUTE(aabb_new_box_test));
		suite.push_back(CUTE(aabb_hit_test));
		suite.push_back(CUTE(aabb_or_point_test));
		suite.push_back(CUTE(sphere_bounding_box_test));
		suite.push_back(CUTE(sphere_hit_test));
		suite.push_back(CUTE(ortho_normal_base_form_normal_test));
		suite.push_back(CUTE(ortho_normal_base_taransform_test));
		suite.push_back(CUTE(random_size_test));
		suite.push_back(CUTE(random_vector_to_sphere));
	}
}
