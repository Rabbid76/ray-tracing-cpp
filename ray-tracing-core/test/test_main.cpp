#include "rtc_core_test_assert.h"
#include "math/math_test.h"
#include "pdf/probability_density_function_test.h"
#include "texture/texture_test.h"
#include "geometry/geometry_test.h"
#include "material/material_test.h"
#include "core/core_test.h"

void unit_test(void);

int main(int, char**)
{
    unit_test();
    return 0;
}

void unit_test(void)
{
    using namespace ray_tracing_core_unit_test;

    math::math_unit_test();
    pdf::probability_density_function_unit_test();
    texture::texture_unit_test();
    geometry::geometry_unit_test();
    material::material_unit_test();
    core::core_unit_tests();
}