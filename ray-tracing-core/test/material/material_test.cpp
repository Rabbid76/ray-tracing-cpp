#include "no_material_test.h"
#include "lambertian_material_test.h"

namespace ray_tracing_core_unit_test
{
    namespace material
    {
        void material_unit_test(void)
        {
            no_material_new_material();
            lambertain_material_hit_test();
            lambertian_material_scatter_test();
            lambertian_material_scattering_pfd_test();
            lambertian_material_emitt_test();
            lambertian_material_has_texture_test();
            lambertian_material_has_mask_test();
        }
    }
}
