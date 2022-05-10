#include "no_material_test.h"
#include "lambertian_material_test.h"
#include "metal_material_test.h"

namespace ray_tracing_core_unit_test::material
{
    void material_unit_test()
    {
        no_material_new_material();
        lambertain_material_hit_test();
        lambertian_material_scatter_test();
        lambertian_material_scattering_pfd_test();
        lambertian_material_emit_test();
        lambertian_material_has_texture_test();
        lambertian_material_has_mask_test();
        metal_material_hit_test();
        metal_material_scatter_test();
        metal_material_scattering_pfd_test();
        metal_material_emit_test();
        metal_material_has_texture_test();
        metal_material_has_mask_test();
    }
}
