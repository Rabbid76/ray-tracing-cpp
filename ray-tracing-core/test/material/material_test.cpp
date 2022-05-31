#include "no_material_test.h"
#include "dielectric_material_test.h"
#include "diffuse_light_test.h"
#include "lambertian_material_test.h"
#include "metal_material_test.h"
#include "isotropic_material_test.h"

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
        dielectric_material_hit_test();
        dielectric_material_scatter_test();
        dielectric_material_scattering_pfd_test();
        dielectric_material_emit_test();
        dielectric_material_has_texture_test();
        dielectric_material_has_mask_test();
        diffuse_light_hit_test();
        diffuse_light_scatter_test();
        diffuse_light_scattering_pfd_test();
        diffuse_light_emit_test();
        diffuse_light_has_texture_test();
        diffuse_light_has_mask_test();
        isotropic_material_hit_test();
        isotropic_material_scatter_test();
        isotropic_material_scattering_pfd_test();
        isotropic_material_emit_test();
        isotropic_material_has_texture_test();
        isotropic_material_has_mask_test();
    }
}
