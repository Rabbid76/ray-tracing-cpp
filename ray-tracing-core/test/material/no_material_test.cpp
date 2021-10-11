#include "no_material_test.h"
#include <material/no_material.h>
#include <math/ray.h>
#include <core/hit_record.h>
#include <core/scatter_record.h>
#include <rtc_core_test_assert.h>

namespace ray_tracing_core_unit_test
{
    namespace material
    {
        using namespace ray_tracing_core::material;
        using namespace ray_tracing_core;

        void no_material_new_material(void)
        {
            auto no_material = NoMaterial();
            auto ray = math::Ray::new_ray(math::Point3D(), math::Vector3D());
            core::HitRecord hit_record;
            core::ScatterRecord scatter_record;
            TEST_ASSERT_EQUAL(false, no_material.scatter(ray, hit_record, scatter_record));
            TEST_ASSERT_EQUAL(1, no_material.scattering_pdf(ray, hit_record, ray));
            assert_equal_color(core::Color(0), no_material.emitt(ray, hit_record), 0.001f);
            TEST_ASSERT_EQUAL(false, no_material.has_texture());
            TEST_ASSERT_EQUAL(false, no_material.has_mask());
        }
    }
}
