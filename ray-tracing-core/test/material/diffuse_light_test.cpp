#include "diffuse_light_test.h"
#include <material/diffuse_light.h>
#include <texture/constant_texture.h>
#include <rtc_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test::material
{
    using namespace ray_tracing_core::material;
    using namespace ray_tracing_core;

    void diffuse_light_hit_test()
    {
        std::vector<std::tuple<bool, texture::ConstantTexture>> test_data
        {
            { true, texture::ConstantTexture(core::Color(0), 1.0f) },
            { false, texture::ConstantTexture(core::Color(0), 0.0f) }
        };

        for (auto [expected_hit, emit] : test_data)
        {
            auto actual_hit = DiffuseLight(&emit)
                    .hit(core::HitRecord::empty());
            TEST_ASSERT_EQUAL(expected_hit, actual_hit);
        }
    }

    void diffuse_light_scatter_test()
    {
        std::vector<std::tuple<core::Color, math::AlphaValue, math::Ray, math::HitPoint>> test_data
        {
            { core::Color(0.1f, 0.2f, 0.3f), 1.0f,
              math::Ray{ math::Point3D(0), math::Vector3D(1, 0, 0), 1 },
              math::HitPoint::new_hit(0, math::Point3D(0), math::Vector3D(-1, 0, 0)) },
        };

        for (auto [color, alpha_value, ray_in, hit_point] : test_data)
        {
            auto hit_record = core::HitRecord::empty();
            hit_record.hit_point = hit_point;
            core::ScatterRecord scatter_record;
            texture::ConstantTexture emit(color, alpha_value);
            DiffuseLight material(&emit);
            auto actual_scatter = material
                    .scatter(ray_in, hit_record, scatter_record);
            TEST_ASSERT_EQUAL(false, actual_scatter);
        }
    }

    void diffuse_light_scattering_pfd_test()
    {
        std::vector<std::tuple<math::Distance, math::Vector3D, math::Vector3D>> test_data
        {
            { 1, math::Vector3D(-1, 0, 0), math::Vector3D(1, 0, 0) },
            { 1, math::Vector3D(0, 1, 0), math::Vector3D(1, 0, 0) },
            { 1, math::Vector3D(1, 0, 0), math::Vector3D(1, 0, 0) },
        };

        for (auto [expected_distance, normal, direction] : test_data)
        {
            auto hit_record = core::HitRecord::empty();
            hit_record.hit_point.normal = normal;
            texture::ConstantTexture emit(core::Color(0), 1.0f);
            auto actual_distance = DiffuseLight(&emit)
                    .scattering_pdf(
                            math::Ray::new_ray(math::Point3D(0), math::Vector3D(0)),
                            hit_record,
                            math::Ray::new_ray(math::Point3D(0), direction));
            TEST_ASSERT_EQUAL_DELTA(expected_distance, actual_distance, 0.001);
        }
    }

    void diffuse_light_emit_test()
    {
        std::vector<std::tuple<core::Color, core::Color, math::Vector3D, math::Vector3D>> test_data
        {
            { core::Color(0), core::Color(1), math::Vector3D(1, 0, 0), math::Vector3D(1, 0, 0) },
            { core::Color(1), core::Color(1), math::Vector3D(1, 0, 0), math::Vector3D(-1, 0, 0) },
            { core::Color(0), core::Color(1), math::Vector3D(0, 1, 0), math::Vector3D(0, 1, 0) },
            { core::Color(1), core::Color(1), math::Vector3D(0, 0, -1), math::Vector3D(0, 0, 1) },
        };
        for (auto [expected_color, texture_color, direction, normal] : test_data) {
            auto emit = texture::ConstantTexture(texture_color, 1.0f);
            auto hit_record = core::HitRecord::empty();
            hit_record.hit_point = math::HitPoint::new_hit(1, math::Point3D(0), normal);
            auto actual_emit = DiffuseLight(&emit)
                    .emit(
                            math::Ray::new_ray(math::Point3D(0), direction),
                            hit_record);
            assert_equal_color(expected_color, actual_emit, false);
        }
    }

    void diffuse_light_has_texture_test()
    {
        std::vector<std::tuple<bool, std::shared_ptr<texture::Texture>>> test_data
        {
            { false, std::make_shared<texture::ConstantTexture>(core::Color(0), 0.0f) },
        };

        for (auto [expected_has_texture, emit] : test_data)
        {
            auto actual_has_texture = DiffuseLight(emit.get())
                    .has_texture();
            TEST_ASSERT_EQUAL(expected_has_texture, actual_has_texture);
        }
    }

    void diffuse_light_has_mask_test()
    {
        std::vector<std::tuple<bool, texture::ConstantTexture>> test_data
        {
            { false, texture::ConstantTexture(core::Color(0), 1.0f) },
            { true, texture::ConstantTexture(core::Color(0), 0.5f) },
            { true, texture::ConstantTexture(core::Color(0), 0.0f) },
        };

        for (auto [expected_has_mask, emit] : test_data)
        {
            auto actual_has_mask = DiffuseLight(&emit)
                    .has_mask();
            TEST_ASSERT_EQUAL(expected_has_mask, actual_has_mask);
        }
    }
}
