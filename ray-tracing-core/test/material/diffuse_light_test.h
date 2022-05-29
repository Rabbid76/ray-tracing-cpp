#ifndef __RTC_UT__MATERIAL__DIFFUSE_MATERIAL_TEST_H__
#define __RTC_UT__MATERIAL__DIFFUSE_MATERIAL_TEST_H__

namespace ray_tracing_core_unit_test::material
{
    extern void diffuse_light_hit_test();
    extern void diffuse_light_scatter_test();
    extern void diffuse_light_scattering_pfd_test();
    extern void diffuse_light_emit_test();
    extern void diffuse_light_has_texture_test();
    extern void diffuse_light_has_mask_test();
}

#endif
