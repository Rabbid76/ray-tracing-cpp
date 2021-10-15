#include "lambertian_material_test.h"
#include <material/lambertian_material.h>
#include <texture/constant_texture.h>
#include <rtc_core_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test
{
    namespace material
    {
        using namespace ray_tracing_core::material;
        using namespace ray_tracing_core;

        void lambertain_material_hit_test(void)
        {

        }

        void lambertian_material_scatter_test(void)
        {

        }

        void lambertian_material_scattering_pfd_test(void)
        {

        }

        void lambertian_material_emitt_test(void)
        {

        }

        void lambertian_material_has_texture_test(void)
        {
            std::vector<std::tuple<bool, std::shared_ptr<texture::Texture>>> test_data
            {
                { false, std::make_shared<texture::ConstantTexture>(core::Color(0), 0.0f) },
            };

            for (auto [expected_has_texture, albedo] : test_data)
            {
                auto actual_has_texture = LambertianMaterial(albedo.get()).has_texture();
                TEST_ASSERT_EQUAL(expected_has_texture, actual_has_texture);
            }
        }

        void lambertian_material_has_mask_test(void)
        {
            std::vector<std::tuple<bool, texture::ConstantTexture>> test_data
            {
                { false, texture::ConstantTexture(core::Color(0), 1.0f) },
                { true, texture::ConstantTexture(core::Color(0), 0.5f) },
                { true, texture::ConstantTexture(core::Color(0), 0.0f) },
            };

            for (auto [expected_has_mask, albedo] : test_data)
            {
                auto actual_has_mask = LambertianMaterial(&albedo).has_mask();
                TEST_ASSERT_EQUAL(expected_has_mask, actual_has_mask);
            }
        }
    }
}
