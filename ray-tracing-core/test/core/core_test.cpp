#include "core_test.h"
#include "hit_record_test.h"
#include "texture_coordinate_test.h"
#include "shape_test.h"

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        void core_unit_tests(void)
        {
            hit_record_empty_test();
            texture_coordinate_form_sphere_unit_test();
            shape_hit_test();
        }
    }
}
