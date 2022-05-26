#include "rectangle_test.h"
#include <math/rectangle.h>
#include <rtc_test_assert.h>
#include <cmath>
#include <tuple>
#include <vector>

namespace ray_tracing_core_unit_test::math
{
    using namespace ray_tracing_core::math;

    void rectangle_bounding_box_test(void)
    {
        std::vector<std::tuple<Point3D, Point3D, Rectangle::Orientation, Distance, Point2D, Point2D>> test_data
        {
            {Point3D(2, 3, 1), Point3D(4, 5, 1), Rectangle::Orientation::XY, 1, Point2D(2, 3), Point2D(4, 5)},
            {Point3D(1, 2, 3), Point3D(1, 4, 5), Rectangle::Orientation::YZ, 1, Point2D(4, 3), Point2D(2, 5)},
            {Point3D(2, 1, 3), Point3D(4, 1, 5), Rectangle::Orientation::XZ, 1, Point2D(4, 5), Point2D(2, 3)},
        };

        for (auto [expected_minimum, expected_maximum, orientation, k, a, b] : test_data)
        {
            auto actual_box = Rectangle::new_rectangle(orientation, k, a, b)
                .bounding_box();
            assert_equal_point(expected_minimum, actual_box.minimum_point, 0.01);
            assert_equal_point(expected_maximum, actual_box.maximum_point, 0.01);
        }
    }

    void rectangle_hit_test(void)
    {
        std::vector<std::tuple<bool, HitPoint, Rectangle, Ray, DistanceRange>> test_data
        {
            {
                false, HitPoint::zero(),
                Rectangle::new_rectangle(Rectangle::Orientation::XY, 0, Point2D{-1, -1}, Point2D{1, 1}),
                Ray::new_ray(Point3D(-2, 0, 0), Vector3D(-1, 0, 0)), { 0, 4 }
            },
            {
                true, HitPoint::new_hit(1, Point3D(0, 0, 0), Vector3D(0, 0, -1)),
                Rectangle::new_rectangle(Rectangle::Orientation::XY, 0, Point2D{-1, -1}, Point2D{1, 1}),
                Ray::new_ray(Point3D(0, 0, -1), Vector3D(0, 0, 1)), { 0, 2 }
            },
            {
                false, HitPoint::zero(),
                Rectangle::new_rectangle(Rectangle::Orientation::YZ, 0, Point2D{-1, -1}, Point2D{1, 1}),
                Ray::new_ray(Point3D(-1, -1, -1), Vector3D(-1, -1, -1)), { 0, 4 }
            },
            {
                true, HitPoint::new_hit(1, Point3D(0, 0, 0), Vector3D(-1, 0, 0)),
                Rectangle::new_rectangle(Rectangle::Orientation::YZ, 0, Point2D{-1, -1}, Point2D{1, 1}),
                Ray::new_ray(Point3D(-1, -1, -1), Vector3D(1, 1, 1)), { 0, 4 }
            },
            {
                false, HitPoint::zero(),
                Rectangle::new_rectangle(Rectangle::Orientation::XZ, 0, Point2D{-1, -1}, Point2D{1, 1}),
                Ray::new_ray(Point3D(2, -2, 0), Vector3D(0, 1, 0)), { 0, 4 }
            },
            {
                true, HitPoint::new_hit(1, Point3D(0, 0, 0), Vector3D(0, -1, 0)),
                Rectangle::new_rectangle(Rectangle::Orientation::XZ, 0, Point2D{-1, -1}, Point2D{1, 1}),
                Ray::new_ray(Point3D(0, -1, -1), Vector3D(0, 1, 1)), { 0, 4 }
            },
        };

        for (auto [expected_result, expected_hit_point, sphere, ray, distance_range] : test_data)
        {
            auto actual_hit_point = HitPoint::zero();
            auto actual_result = sphere.hit(ray, distance_range, actual_hit_point);
            TEST_ASSERT_EQUAL(expected_result, actual_result);
            if (expected_result && actual_result)
            {
                TEST_ASSERT_EQUAL_DELTA(expected_hit_point.distance, actual_hit_point.distance, 0.001);
                assert_equal_point(expected_hit_point.position, actual_hit_point.position, 0.001);
                assert_equal_vector(expected_hit_point.normal, actual_hit_point.normal, 0.001);
            }
        }
    }
}