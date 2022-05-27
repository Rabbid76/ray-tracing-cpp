#include "geometry_test.h"
#include "rectangle_test.h"
#include "sphere_test.h"

namespace ray_tracing_core_unit_test
{
    namespace geometry
    {
        void geometry_unit_test(void)
        {
            sphere_hit_texture_coordinate_test();
            sphere_probability_density_function_value_test();
            sphere_random_test();
            rectangle_hit_texture_coordinate_test();
            rectangle_probability_density_function_value_test();
            rectangle_random_test();
        }
    }
}
