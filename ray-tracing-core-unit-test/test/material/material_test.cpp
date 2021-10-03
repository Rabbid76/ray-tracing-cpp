#include "no_material_test.h"
#include "lambertian_material_test.h"
#include "cute.h"

namespace material
{
	void add_material_unit_test(cute::suite &suite)
	{
		suite.push_back(CUTE(no_material_new_material));
		suite.push_back(CUTE(lambertain_material_hit_test));
		suite.push_back(CUTE(lambertian_material_scatter_test));
		suite.push_back(CUTE(lambertian_material_scattering_pfd_test));
		suite.push_back(CUTE(lambertian_material_emitt_test));
		suite.push_back(CUTE(lambertian_material_has_texture_test));
		suite.push_back(CUTE(lambertian_material_has_mask_test));
	}
}
