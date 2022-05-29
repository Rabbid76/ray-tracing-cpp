#include "scene_test.h"
#include <core/scene.h>
#include <core/shape.h>
#include <geometry/sphere.h>
#include <texture/constant_texture.h>
#include <material/lambertian_material.h>
#include <rtc_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core;
        using namespace ray_tracing_core::core;

        void scene_ray_trace_color_test()
        {
            Configuration configuration
            {
                .maximum_depth = 50
            };
            auto camera = Camera::new_camera_from_vertical_field(45.0, 2);
            auto sky = environment::Sky::new_sky(Color(1.0f), Color(0.5f, 0.7f, 1.0f));
            geometry::Sphere sphere_geometry(math::Point3D(0.0f, 0.0f, -3.0f), 1.0f);
            texture::ConstantTexture sphere_color(Color(1.0f, 0, 0), 1.0f);
            material::LambertianMaterial sphere_material(&sphere_color);
            Shape sphere(&sphere_geometry, &sphere_material);
            Scene scene(configuration, camera, sky, sphere);

            std::vector<std::tuple<Color, math::AlphaValue, double, double>> test_data
            {
                { Color(0.7f, 0, 0), 0.25f, 0.5, 0.5 },
                { Color(0.5f, 0.7f, 1.0f), 0.2f, 0.5, 1.0 },
                { Color(1.0f), 0.2f, 0.5, 0 },
            };

            for (auto [expected_color, delta, u, v] : test_data)
            {
                auto actual_color = scene.ray_trace_color(u, v);
                assert_equal_color(expected_color, actual_color, delta);
            }
        }
    }
}
