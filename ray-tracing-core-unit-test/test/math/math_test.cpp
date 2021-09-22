#include "math_test.h"
#include "ray_test.h"

namespace math
{
	void add_math_unit_test(cute::suite &suite)
	{
		suite.push_back(CUTE(math::ray_point_at_test));
	}
}
