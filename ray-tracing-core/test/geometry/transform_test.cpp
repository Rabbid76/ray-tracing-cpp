#include "transform_test.h"
#include <geometry/rectangle.h>
#include <geometry/transform.h>
#include <rtc_test_assert.h>
#include <tuple>
#include <vector>
#include <cmath>

namespace ray_tracing_core_unit_test::geometry
{
    using namespace ray_tracing_core::geometry;
    using namespace ray_tracing_core;

    void transform_hit_test(void)
    {
        std::vector<std::tuple<bool, math::HitPoint, math::Point3D, double, math::Vector3D, Rectangle, math::Ray, math::DistanceRange>> test_data
        {
            { 
                true, math::HitPoint::new_hit(1, math::Point3D(0, 0, 0), math::Vector3D(-1, 0, 0)),
                math::Point3D(0), 0, math::Vector3D(1, 0, 0), 
                geometry::Rectangle(math::Rectangle::Orientation::YZ, 0, math::Point2D{-1, -1}, math::Point2D{1, 1}),
                math::Ray::new_ray(math::Point3D(-1, 0, 0), math::Vector3D(1, 0, 0)), { 0, 4 }
            },
            {
                true, math::HitPoint::new_hit(1, math::Point3D(0, 0, 0), math::Vector3D(-1, 0, 0)),
                math::Point3D(0), 0, math::Vector3D(0),
                geometry::Rectangle(math::Rectangle::Orientation::YZ, 0, math::Point2D{-1, -1}, math::Point2D{1, 1}),
                math::Ray::new_ray(math::Point3D(-1, 0, 0), math::Vector3D(1, 0, 0)), { 0, 4 }
            },
            {
                true, math::HitPoint::new_hit(1, math::Point3D(0, 0, 0), math::Vector3D(1, 0, 0)),
                math::Point3D(0), 180, math::Vector3D(0, 1, 0),
                geometry::Rectangle(math::Rectangle::Orientation::YZ, 0, math::Point2D{-1, -1}, math::Point2D{1, 1}),
                math::Ray::new_ray(math::Point3D(-1, 0, 0), math::Vector3D(1, 0, 0)), { 0, 4 }
            },
            {
                false, math::HitPoint::zero(),
                math::Point3D(0), 90, math::Vector3D(0, 0, 1),
                geometry::Rectangle(math::Rectangle::Orientation::YZ, 0, math::Point2D{-1, -1}, math::Point2D{1, 1}),
                math::Ray::new_ray(math::Point3D(-1, 0.5, 0), math::Vector3D(1, 0, 0)), { 0, 4 }
            },
            {
                false, math::HitPoint::zero(),
                math::Point3D(0, 2, 0), 0, math::Vector3D(0, 0, 1),
                geometry::Rectangle(math::Rectangle::Orientation::YZ, 0, math::Point2D{-1, -1}, math::Point2D{1, 1}),
                math::Ray::new_ray(math::Point3D(-1, 0, 0), math::Vector3D(1, 0, 0)), { 0, 4 }
            },
        };

        for (auto [expected_result, expected_hit_point, tramslate, angle, axis, rectnagle, in_ray, distance_range] : test_data)
        {
            geometry::Transform transform(&rectnagle, tramslate, angle, axis, math::Vector3D(1));
            core::HitRecord actual_hit_record;
            bool actual_result = transform.hit(in_ray, distance_range, false, actual_hit_record);
            TEST_ASSERT_EQUAL(expected_result, actual_result);
            if (expected_result && actual_result)
            {
                TEST_ASSERT_EQUAL_DELTA(expected_hit_point.distance, actual_hit_record.hit_point.distance, 0.001);
                assert_equal_point(expected_hit_point.position, actual_hit_record.hit_point.position, 0.001);
                assert_equal_vector(expected_hit_point.normal, actual_hit_record.hit_point.normal, 0.001);
            }
        }
    }
}