#include "core_test.h"
#include "hit_record_test.h"
#include "texture_coordinate_test.h"
#include "shape_test.h"
#include "camera_test.h"
#include "shape_list_test.h"

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        void core_unit_tests()
        {
            hit_record_empty_test();
            texture_coordinate_form_sphere_unit_test();
            shape_hit_test();
            camera_from_vertical_field_test();
            camera_from_look_at_test();
            camera_ray_to_test();
            camera_change_aspect_test();
            shape_list_bounding_box_test();
        }
    }
}
