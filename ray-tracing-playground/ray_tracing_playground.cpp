// ray-tracing-core.cpp : Defines the entry point for the application.
//

#include "ray_tracing_playground.h"
#include <core/camera.h>
#include <core/configuration.h>
#include <core/scene.h>
#include <core/shape.h>
#include <core/shape_list.h>
#include <environment/sky.h>
#include <geometry/sphere.h>
#include <material/lambertian_material.h>
#include <math/random.h>
#include <texture/constant_texture.h>
#include <vector>
#include <cmath>

using namespace ray_tracing_core;


int main()
{
	std::cout << "start" << std::endl;

	// TODO TestSceneSimple

    auto sphere_texture = texture::ConstantTexture(math::ColorRGB(0.5f, 0.1f, 0.1f), 1);
    auto sphere_material = material::LambertianMaterial(&sphere_texture);
    auto sphere_geometry = geometry::Sphere(math::Point3D(0, 0, 0), 0.5);
    auto sphere_shape = core::Shape(&sphere_geometry, &sphere_material);

    auto ground_texture = texture::ConstantTexture(math::ColorRGB(0.1f, 0.1f, 0.1f), 1);
    auto ground_material = material::LambertianMaterial(&ground_texture);
    auto ground_geometry = geometry::Sphere(math::Point3D(0, -100.5, 0), 100.0);
    auto ground_shape = core::Shape(&ground_geometry, &ground_material);

    auto world = core::ShapeList({ &sphere_shape, &ground_shape });
	auto camera = core::Camera::new_camera_from_look_at(
        math::Point3D(0, 0, 1),
        math::Point3D(0),
        math::Vector3D(0, 0, 1),
        90, 2, 0, 1, math::TimeRange{ 0, 0 }
    );
    auto sky = environment::Sky::new_sky(math::ColorRGB(1, 1, 1), math::ColorRGB(0.5, 0.7, 1));

    auto configuration = core::Configuration
    {
        .maximum_depth = 100
    };
    auto scene = core::Scene(configuration, camera, sky, world); 

    std::cout << "render" << std::endl;

    uint32_t cx = 400;
    uint32_t cy = 200;
    uint32_t samples = 10;

    std::vector<uint8_t> pixel_data(cx * cy * 4);
    math::RandomGenerator randomGenerator;
    for (uint32_t x = 0; x < cx; ++x) {
        for (uint32_t y = 0; y < cy; ++y) {
            math::ColorRGB fragment_color;
            for (uint32_t s = 0; s < samples; ++s) {
                double u = (static_cast<double>(x) + randomGenerator.random_size()) / static_cast<double>(cx);
                double v = (static_cast<double>(y) + randomGenerator.random_size()) / static_cast<double>(cy);
                fragment_color += scene.ray_trace_color(u, v);
            }
            fragment_color /= static_cast<double>(samples);

            uint32_t i = (y * cx) + x;
            pixel_data[i * 4] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[0] * 255.0)));
            pixel_data[i * 4 + 1] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[1] * 255.0)));
            pixel_data[i * 4 + 2] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[2] * 255.0)));
            pixel_data[i * 4 + 3] = 255;
        }   
    }

    std::cout << "end" << std::endl;
	return 0;
}
