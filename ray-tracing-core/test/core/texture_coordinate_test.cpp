#include "texture_coordinate_test.h"
#include <core/texture_coordinate.h>
#include <rtc_core_test_assert.h>
#include <tuple>
#include <vector>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core::core;
        using namespace ray_tracing_core;

        void texture_coordinate_form_sphere_unit_test(void)
        {
            std::vector<std::tuple<math::TextureDistance, math::TextureDistance, math::Vector3D>> test_data
            {
                { 0, 0.5, math::Vector3D(-1, 0, 0) },
                { 0.5, 0.5, math::Vector3D(1, 0, 0) },
                { 0.5, 0, math::Vector3D(0, -1, 0) },
                { 0.5, 1, math::Vector3D(0, 1, 0) },
                { 0.75, 0.5, math::Vector3D(0, 0, -1) },
                { 0.25, 0.5, math::Vector3D(0, 0, 1) },
            };

            for (auto [expected_u, expected_v, point_on_sphere] : test_data)
            {
                auto actual_texture_coordiante = TextureCoordinate::from_sphere(point_on_sphere);
                TEST_ASSERT_EQUAL_DELTA(expected_u, actual_texture_coordiante.u, 0.001);
                TEST_ASSERT_EQUAL_DELTA(expected_v, actual_texture_coordiante.v, 0.001);
            }
        }
    }
}
