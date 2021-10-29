#include "sky_test.h"
#include <environment/sky.h>
#include <rtc_core_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test
{
    namespace environment
    {
        using namespace ray_tracing_core;
        using namespace ray_tracing_core::environment;

        void sky_color_at_test()
        {
            std::vector<std::tuple<core::Color, core::Color, core::Color, math::Ray>> test_data
            {
                { core::Color(1.0f, 0.5f, 0.5f), core::Color(1.0f, 0, 0), core::Color(1.0f, 1.0f, 1.0f), 
                  math::Ray::new_ray(math::Point3D(0), math::Vector3D(0, 0, 1)) }
            };

            for (auto [expected_color, nadir_color, zenith_color, ray] : test_data)
            {
                auto actual_color = Sky::new_sky(nadir_color, zenith_color)
                    .color_at(ray);
                assert_equal_color(expected_color, actual_color, 0.001f);
            }
        }
    }
}