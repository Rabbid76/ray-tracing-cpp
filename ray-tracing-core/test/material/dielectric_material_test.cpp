#include "dielectric_material_test.h"
#include <material/dielectric_material.h>
#include <texture/constant_texture.h>
#include <rtc_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test::material
{
    using namespace ray_tracing_core::material;
    using namespace ray_tracing_core;

    void dielectric_material_hit_test()
    {
        std::vector<std::tuple<bool, texture::ConstantTexture>> test_data
                {
                        { true, texture::ConstantTexture(core::Color(0), 1.0f) },
                        { false, texture::ConstantTexture(core::Color(0), 0.0f) }
                };

        for (auto [expected_hit, albedo] : test_data)
        {
            auto actual_hit = DielectricMaterial({1.0f, 1.0f}, &albedo)
                    .hit(core::HitRecord::empty());
            TEST_ASSERT_EQUAL(expected_hit, actual_hit);
        }
    }

    void dielectric_material_scatter_test()
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
            texture::ConstantTexture albedo(color, alpha_value);
            DielectricMaterial material({1.0f, 1.0f}, &albedo);
            auto actual_scatter = material
                    .scatter(ray_in, hit_record, scatter_record);

            TEST_ASSERT_EQUAL(true, actual_scatter);
            assert_equal_point(hit_point.position, scatter_record.ray.origin, 0);
            //assert_equal_point(hit_point.normal, scatter_record.ray.direction, 1.0f);
            TEST_ASSERT_EQUAL(ray_in.time, scatter_record.ray.time);
            assert_equal_color(color, scatter_record.attenuation, 0);
            TEST_ASSERT_EQUAL(alpha_value, scatter_record.alpha);
            TEST_ASSERT_EQUAL(nullptr, scatter_record.probability_density_function.get());
            TEST_ASSERT_EQUAL(&material, scatter_record.material);
        }
    }

    void dielectric_material_scattering_pfd_test()
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
            texture::ConstantTexture albedo(core::Color(0), 1.0f);
            auto actual_distance = DielectricMaterial({1.0f, 1.0f}, &albedo)
                    .scattering_pdf(
                            math::Ray::new_ray(math::Point3D(0), math::Vector3D(0)),
                            hit_record,
                            math::Ray::new_ray(math::Point3D(0), direction));
            TEST_ASSERT_EQUAL_DELTA(expected_distance, actual_distance, 0.001);
        }
    }

    void dielectric_material_emit_test()
    {
        auto albedo = texture::ConstantTexture(core::Color(0), 0.0f);
        auto actual_emit = DielectricMaterial({1.0f, 1.0f}, &albedo)
                .emitt(
                        math::Ray::new_ray(math::Point3D(0), math::Vector3D(0)),
                        core::HitRecord::empty());
        assert_equal_color(core::Color(0), actual_emit, false);
    }

    void dielectric_material_has_texture_test()
    {
        std::vector<std::tuple<bool, std::shared_ptr<texture::Texture>>> test_data
                {
                        { false, std::make_shared<texture::ConstantTexture>(core::Color(0), 0.0f) },
                };

        for (auto [expected_has_texture, albedo] : test_data)
        {
            auto actual_has_texture = DielectricMaterial({1.0f, 1.0f}, albedo.get())
                    .has_texture();
            TEST_ASSERT_EQUAL(expected_has_texture, actual_has_texture);
        }
    }

    void dielectric_material_has_mask_test()
    {
        std::vector<std::tuple<bool, texture::ConstantTexture>> test_data
                {
                        { false, texture::ConstantTexture(core::Color(0), 1.0f) },
                        { true, texture::ConstantTexture(core::Color(0), 0.5f) },
                        { true, texture::ConstantTexture(core::Color(0), 0.0f) },
                };

        for (auto [expected_has_mask, albedo] : test_data)
        {
            auto actual_has_mask = DielectricMaterial({1.0f, 1.0f}, &albedo)
                    .has_mask();
            TEST_ASSERT_EQUAL(expected_has_mask, actual_has_mask);
        }
    }
}
