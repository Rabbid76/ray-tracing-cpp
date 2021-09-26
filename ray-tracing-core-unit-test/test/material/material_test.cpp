#include "no_material_test.h"
#include "cute.h"

namespace material
{
	void add_material_unit_test(cute::suite &suite)
	{
		suite.push_back(CUTE(no_material_new_material));
	}
}
