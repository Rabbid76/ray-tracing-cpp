#include "sphere_test.h"
#include <geometry/sphere.h>
#include <rtc_test_assert.h>
#include <tuple>
#include <vector>
#include <cmath>

namespace ray_tracing_core_unit_test
{
    namespace geometry
    {
        using namespace ray_tracing_core::geometry;
        using namespace ray_tracing_core;

        void sphere_hit_texture_coordinate_test(void)
        {
            std::vector<std::tuple<core::TextureCoordinate, Sphere, math::Ray, math::DistanceRange, bool>> test_data
            {
                { core::TextureCoordinate{0, 0}, Sphere(math::Point3D(0), 1),
                    math::Ray::new_ray(math::Point3D(-2, 0, 0), math::Vector3D(1, 0, 0)), { 0, 4 }, false},
                { core::TextureCoordinate{0, 0.5}, Sphere(math::Point3D(0), 1),
                    math::Ray::new_ray(math::Point3D(-2, 0, 0), math::Vector3D(1, 0, 0)), { 0, 4 }, true}
            };

            for (auto [expected_coordinates, sphere, in_ray, distance_range, set_texture_coordinate] : test_data)
            {
                core::HitRecord hit_record;
                sphere.hit(in_ray, distance_range, set_texture_coordinate, hit_record);
                assert_equal_texture_coordinate(expected_coordinates, hit_record.texture_coordinate, set_texture_coordinate ? 0.001f : 0);
            }
        }

        void sphere_probability_density_function_value_test()
        {
            std::vector<std::tuple<math::Distance, Sphere, math::Point3D, math::Vector3D>> test_data
            {
                {0, Sphere(math::Point3D(0), 1), math::Point3D(-2, 0, 0), math::Vector3D(-1, 0, 0)},
                {1.18795, Sphere(math::Point3D(0), 1), math::Point3D(-2, 0, 0), math::Vector3D(1, 0, 0)}
            };

            for (auto [expected_pdf_value, sphere, origin, direction] : test_data)
            {
                auto actual_pdf_value = sphere.probability_density_function_value(origin, direction);
                TEST_ASSERT_EQUAL_DELTA(expected_pdf_value, actual_pdf_value, 0.001);
            }
        }

        void sphere_random_test(void)
        {
            const int test_iterations = 10;

            std::vector<std::tuple<math::Vector3D, math::Distance, Sphere, math::Point3D>> test_data
            {
                { math::Vector3D(1, 0, 0), 0.5, Sphere(math::Point3D(0), 1), math::Vector3D(-2, 0, 0) }
            };

            for (auto [expected_vector, expected_delta, sphere, origin] : test_data)
            {
                for (int iteration = 0; iteration < test_iterations; ++iteration)
                {
                    auto actual_vector = sphere.random(origin);
                    assert_equal_vector(expected_vector, actual_vector, expected_delta);
                    auto actual_length = std::sqrt(math::dot(actual_vector, actual_vector));
                    TEST_ASSERT_EQUAL_DELTA(1, actual_length, 0.001);
                }
            }
        }
    }
}
