#include "camera_test.h"
#include <core/camera.h>
#include <rtc_core_test_assert.h>
#include <vector>
#include <tuple>
#include <algorithm>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core;
        using namespace ray_tracing_core::core;
        using namespace ray_tracing_core::math;

        void camera_from_vertical_field_test()
        {
            std::vector<std::tuple<Point3D, Vector3D, Vector3D, Point3D, 
                Vector3D, Vector3D, Vector3D, Distance, TimeRange,
                double, double>> test_data
            {
                { Point3D(-2, -1, -1), Vector3D(4, 0, 0), Vector3D(0, 2, 0), Point3D(0),
                  Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1), 0, {0, 0}, 90, 2 }
            };

            for (auto [expected_lower_left, expected_horizontal, expected_vertical, expected_origin,
                expected_u, expected_v, expected_w, expected_lense_radius, expected_time_range,
                field_of_view_y, aspect] : test_data)
            {
                auto actual_camera = Camera::new_camera_from_vertical_field(field_of_view_y, aspect);
                assert_equal_point(expected_lower_left, actual_camera.lower_left_corner, 0.001);
                assert_equal_vector(expected_horizontal, actual_camera.horizontal, 0.001);
                assert_equal_vector(expected_vertical, actual_camera.vertical, 0.001);
                assert_equal_point(expected_origin, actual_camera.origin, 0.001);
                assert_equal_vector(expected_u, actual_camera.u, 0.001);
                assert_equal_vector(expected_v, actual_camera.v, 0.001);
                assert_equal_vector(expected_w, actual_camera.w, 0.001);
                TEST_ASSERT_EQUAL_DELTA(expected_lense_radius, actual_camera.lens_radius, 0.001);
                assert_equal_time_range(expected_time_range, actual_camera.time_range, 0.001);
            }
        }

        void camera_from_look_at_test()
        {
            std::vector<std::tuple<Point3D, Vector3D, Vector3D, Point3D,
                Vector3D, Vector3D, Vector3D, Distance, TimeRange,
                Point3D, Point3D, Vector3D, double, double, 
                double, Distance, TimeRange>> test_data
            {
                { Point3D(-2, -1, -1), Vector3D(4, 0, 0), Vector3D(0, 2, 0), Point3D(0),
                  Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1), 0.25, {0, 100}, 
                  Point3D(0), Point3D(0, 0, -1), Vector3D(0, 1, 0), 90, 2, 0.5, 1, {0, 100} }
            };

            for (auto [expected_lower_left, expected_horizontal, expected_vertical, expected_origin,
                expected_u, expected_v, expected_w, expected_lense_radius, expected_time_range,
                look_form, look_at, up_vector, field_of_view_y, aspect, 
                adepture, focus_distance, time_range] : test_data)
            {
                auto actual_camera = Camera::new_camera_from_look_at(
                    look_form, look_at, up_vector, field_of_view_y, aspect,
                    adepture, focus_distance, time_range);
                assert_equal_point(expected_lower_left, actual_camera.lower_left_corner, 0.001);
                assert_equal_vector(expected_horizontal, actual_camera.horizontal, 0.001);
                assert_equal_vector(expected_vertical, actual_camera.vertical, 0.001);
                assert_equal_point(expected_origin, actual_camera.origin, 0.001);
                assert_equal_vector(expected_u, actual_camera.u, 0.001);
                assert_equal_vector(expected_v, actual_camera.v, 0.001);
                assert_equal_vector(expected_w, actual_camera.w, 0.001);
                TEST_ASSERT_EQUAL_DELTA(expected_lense_radius, actual_camera.lens_radius, 0.001);
                assert_equal_time_range(expected_time_range, actual_camera.time_range, 0.001);
            }
        }

        void camera_ray_to_test()
        {
            const auto _1_sqrt_3 = 1 / std::sqrt(3);
            std::vector<std::tuple<Point3D, Vector3D, Distance, TimeRange, Distance, Distance, Camera>> test_data
            {
                { Point3D(0), Vector3D(0.0, 0.0, -1.0), 0.001, {0, 0}, 0.5, 0.5, Camera::new_camera_from_vertical_field(45, 2) },
                { Point3D(0), Vector3D(_1_sqrt_3, _1_sqrt_3, -_1_sqrt_3), 0.001, {0, 0}, 1, 1, Camera::new_camera_from_vertical_field(90, 1) },
            };

            for (auto [expected_origin, expected_direction, delta, expected_time_range, u, v, camera] : test_data)
            {
                auto actual_ray = camera.ray_to(u, v);
                assert_equal_point(expected_origin, actual_ray.origin, delta);
                auto actual_direction = math::normalize(actual_ray.direction);
                assert_equal_vector(expected_direction, actual_direction, delta);
                auto [ts, te] = expected_time_range;
                TEST_ASSERT_EQUAL_DELTA((ts + te) / 2, actual_ray.time, std::max((te - ts) / 2, 0.001));
            }
        }
        
        void camera_change_aspect_test()
        {
            std::vector<std::tuple<Point3D, Vector3D, Vector3D, Point3D,
                Vector3D, Vector3D, Vector3D, Distance, TimeRange,
                Camera, double>> test_data
            {
                { Point3D(-2, -1, -1), Vector3D(4, 0, 0), Vector3D(0, 2, 0), Point3D(0),
                  Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1), 0, {0, 0}, 
                  Camera::new_camera_from_vertical_field(90, 1), 2 }
            };

            for (auto [expected_lower_left, expected_horizontal, expected_vertical, expected_origin,
                expected_u, expected_v, expected_w, expected_lense_radius, expected_time_range,
                camera, new_aspect] : test_data)
            {
                const auto& actual_camera = camera
                    .change_aspect(new_aspect);
                assert_equal_point(expected_lower_left, actual_camera.lower_left_corner, 0.001);
                assert_equal_vector(expected_horizontal, actual_camera.horizontal, 0.001);
                assert_equal_vector(expected_vertical, actual_camera.vertical, 0.001);
                assert_equal_point(expected_origin, actual_camera.origin, 0.001);
                assert_equal_vector(expected_u, actual_camera.u, 0.001);
                assert_equal_vector(expected_v, actual_camera.v, 0.001);
                assert_equal_vector(expected_w, actual_camera.w, 0.001);
                TEST_ASSERT_EQUAL_DELTA(expected_lense_radius, actual_camera.lens_radius, 0.001);
                assert_equal_time_range(expected_time_range, actual_camera.time_range, 0.001);
            }
        }
    }
}