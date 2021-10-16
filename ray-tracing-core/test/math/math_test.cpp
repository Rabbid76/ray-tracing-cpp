#include "math_test.h"
#include "ray_test.h"
#include "hit_point_test.h"
#include "axis_aligned_bounding_box_test.h"
#include "sphere_test.h"
#include "ortho_normal_base_test.h"
#include "random_test.h"

namespace ray_tracing_core_unit_test
{
    namespace math
    {
        void math_unit_test(void)
        {
            ray_point_at_test();
            ray_with_attributes_test();
            hit_point_new_hit_test();
            aabb_new_box_test();
            aabb_hit_test();
            aabb_or_point_test();
            sphere_bounding_box_test();
            sphere_hit_test();
            ortho_normal_base_form_normal_test();
            ortho_normal_base_taransform_test();
            random_size_test();
            random_unit_test();
            random_cosine_direction_test();
            random_vector_to_sphere_test();
        }
    }
}
