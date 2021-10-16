#include "camera_test.h"
#include <core/camera.h>
#include <rtc_core_test_assert.h>
#include <vector>
#include <tuple>

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

        }

        void camera_ray_to_test()
        {

        }
        
        void camera_change_aspect_test()
        {

        }
    }
}