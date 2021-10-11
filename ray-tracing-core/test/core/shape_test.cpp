#include "core_test.h"
#include <core/shape.h>
#include <core/texture_coordinate.h>
#include <geometry/sphere.h>
#include <material/mockup_material.h>
#include <rtc_core_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core::core;

        void shape_hit_test(void)
        {
            std::vector<std::tuple<bool, TextureCoordinate, bool, bool, bool>> test_data
            {
                { true, TextureCoordinate::null(), false, false, false},
                { true, TextureCoordinate::constant(0, 0.5), false, true, false},
                { false, TextureCoordinate::null(), false, false, true},
                { true, TextureCoordinate::null(), true, false, true},
                { true, TextureCoordinate::constant(0, 0.5), true, true, true},
            };

            ray_tracing_core::geometry::Sphere sphere(ray_tracing_core::math::Vector3D(0), 1);
            for (auto [expected_hit, expected_texture_coordinate, material_hit_result, has_texture, has_mask] : test_data)
            {
                material::MockupMaterial material;
                material.hit_result = material_hit_result;
                material.has_texture_result = has_texture;
                material.has_mask_result = has_mask;
                Shape shape(&sphere, &material);
                auto hit_record = HitRecord::empty();
                bool hit = shape.hit(
                    ray_tracing_core::math::Ray(
                        ray_tracing_core::math::Point3D(-2, 0, 0),
                        ray_tracing_core::math::Vector3D(1, 0, 0)),
                    { 0, 5 },
                    hit_record);
                TEST_ASSERT_EQUAL(expected_hit, hit);
                assert_equal_texture_coordinate(expected_texture_coordinate, hit_record.texture_coordinate, 0.001);
            }
        }
    }
}
