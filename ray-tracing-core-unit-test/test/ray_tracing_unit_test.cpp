#include "../test/ray_tracing_unit_test.h"

#include "cute.h"
#include "core/core_test.h"
#include "math/math_test.h"
#include "geometry/geometry_test.h"
#include "material/material_test.h"

cute::suite make_suite_ray_tracing_unit_test() {
	cute::suite suite { };
	core::add_core_unit_tests(suite);
	math::add_math_unit_test(suite);
	geometry::add_geometry_unit_test(suite);
	material::add_material_unit_test(suite);
	return suite;
}
