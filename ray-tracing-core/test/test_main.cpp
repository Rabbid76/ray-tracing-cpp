#include "rtc_core_test_assert.h"
#include "math/math_test.h"
#include "pdf/probability_density_function_test.h"
#include "texture/texture_test.h"
#include "geometry/geometry_test.h"
#include "material/material_test.h"
#include "core/core_test.h"
#include "environment/environment_test.h"
#include <string>

void unit_test(std::string selected_test);

int main(int noargs, char** args)
{
    const char* selected_test = noargs > 1 ? args[1] : "";
    unit_test(selected_test);
    return 0;
}

void unit_test(std::string selected_test)
{
    using namespace ray_tracing_core_unit_test;

    if (selected_test.empty() || selected_test.starts_with("math"))
        math::math_unit_test();
    if (selected_test.empty() || selected_test.starts_with("pdf"))
        pdf::probability_density_function_unit_test();
    if (selected_test.empty() || selected_test.starts_with("texture"))
        texture::texture_unit_test();
    if (selected_test.empty() || selected_test.starts_with("geometry"))
        geometry::geometry_unit_test();
    if (selected_test.empty() || selected_test.starts_with("material"))
        material::material_unit_test();
    if (selected_test.empty() || selected_test.starts_with("core"))
        core::core_unit_tests();
    if (selected_test.empty() || selected_test.starts_with("environment"))
        environment::environment_unit_test();
}