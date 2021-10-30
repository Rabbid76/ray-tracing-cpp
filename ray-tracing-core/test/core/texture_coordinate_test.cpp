#include "texture_coordinate_test.h"
#include <core/texture_coordinate.h>
#include <rtc_test_assert.h>
#include <tuple>
#include <vector>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core::core;
        using namespace ray_tracing_core;

        void texture_coordinate_form_sphere_unit_test()
        {
            std::vector<std::tuple<math::TextureDistance, math::TextureDistance, math::Vector3D>> test_data
            {
                { 0.0f, 0.5f, math::Vector3D(-1, 0, 0) },
                { 0.5f, 0.5f, math::Vector3D(1, 0, 0) },
                { 0.5f, 0.0f, math::Vector3D(0, -1, 0) },
                { 0.5f, 1.0f, math::Vector3D(0, 1, 0) },
                { 0.75f, 0.5f, math::Vector3D(0, 0, -1) },
                { 0.25f, 0.5f, math::Vector3D(0, 0, 1) },
            };

            for (auto [expected_u, expected_v, point_on_sphere] : test_data)
            {
                auto actual_texture_coordiante = TextureCoordinate::from_sphere(point_on_sphere);
                assert_equal_texture_coordinate(
                    TextureCoordinate::constant(expected_u, expected_v), actual_texture_coordiante, 0.001f);
            }
        }
    }
}
