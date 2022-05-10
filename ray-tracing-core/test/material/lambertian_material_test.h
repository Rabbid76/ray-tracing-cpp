#ifndef __RTC_UT__MATERIAL__LAMBERTIAN_MATERIAL_TEST_H__
#define __RTC_UT__MATERIAL__LAMBERTIAN_MATERIAL_TEST_H__

namespace ray_tracing_core_unit_test::material
{
    extern void lambertain_material_hit_test();
    extern void lambertian_material_scatter_test();
    extern void lambertian_material_scattering_pfd_test();
    extern void lambertian_material_emit_test();
    extern void lambertian_material_has_texture_test();
    extern void lambertian_material_has_mask_test();
}

#endif
