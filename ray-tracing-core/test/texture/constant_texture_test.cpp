#include "constant_texture_test.h"
#include "texture/constant_texture.h"
#include <rtc_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test::texture
{
    using namespace ray_tracing_core::texture;
    using namespace ray_tracing_core;

    void constant_texture_channels_test()
    {
        std::vector<std::tuple<core::Color, math::AlphaValue, bool, bool, ConstantTexture>> test_data
        {
            {core::Color(0), 1.0f, false, false, ConstantTexture(core::Color(0), 1.0f)},
            {core::Color(0), 0.5f, false, true, ConstantTexture(core::Color(0), 0.5f)}
        };

        for (auto [expected_color, expected_alpha_value, expected_depends_on_texture, expected_has_alpha, texture] : test_data)
        {
            auto [actual_color, actual_alpha] = texture.channels(core::TextureCoordinate::null(), math::Point3D(0));
            auto actual_depends_on_texture = texture.depends_on_texture_coordinates();
            auto actual_has_alpha = texture.has_alpha_channel();
            assert_equal_color(expected_color, actual_color, 0.001f);
            TEST_ASSERT_EQUAL_DELTA(expected_alpha_value, actual_alpha, 0.001);
            TEST_ASSERT_EQUAL(expected_depends_on_texture, actual_depends_on_texture);
            TEST_ASSERT_EQUAL(expected_has_alpha, actual_has_alpha);
        }
    }
}
