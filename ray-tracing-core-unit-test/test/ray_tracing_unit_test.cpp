#include "../test/ray_tracing_unit_test.h"

#include "cute.h"
#include "core/core_test.h"
#include "math/math_test.h"

cute::suite make_suite_ray_tracing_unit_test() {
	cute::suite s { };
	core::add_core_unit_tests(s);
	math::add_math_unit_test(s);
	return s;
}
