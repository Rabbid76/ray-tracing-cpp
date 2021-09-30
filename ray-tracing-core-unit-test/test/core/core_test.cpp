#include "core_test.h"
#include "hit_record_test.h"
#include "texture_coordinate_test.h"
#include "shape_test.h"
#include "cute.h"

namespace core
{
	void add_core_unit_tests(cute::suite &suite)
	{
		suite.push_back(CUTE(hit_record_empty_test));
		suite.push_back(CUTE(texture_coordinate_form_sphere_unit_test));
		suite.push_back(CUTE(shape_hit_test));
	}
}
