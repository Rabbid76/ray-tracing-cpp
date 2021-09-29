#include "geometry_test.h"
#include "sphere_test.h"

namespace geometry
{
	void add_geometry_unit_test(cute::suite &suite)
	{
		suite.push_back(CUTE(sphere_hit_texture_coordinate_test));
		suite.push_back(CUTE(sphere_probability_density_function_value_test));
		suite.push_back(CUTE(sphere_random_test));
	}
}
