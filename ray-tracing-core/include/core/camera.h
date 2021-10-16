#ifndef __RTC__CORE__CAMERA_H__
#define __RTC__CORE__CAMERA_H__

#include <math/types.h>
#include <math/constants.h>
#include <cmath>

namespace ray_tracing_core
{
    namespace core
    {
        class Camera
        {
        public:

            math::Point3D lower_left_corner;
            math::Vector3D horizontal;
            math::Vector3D vertical;
            math::Point3D origin;
            math::Vector3D u;
            math::Vector3D v;
            math::Vector3D w;
            math::Distance lens_radius;
            math::TimeRange time_range;

            static inline Camera new_camera_from_vertical_field(double field_of_view_y, double aspect);

            Camera(const math::Point3D& lower_left_point, const math::Vector3D& horzontal_direction, const math::Vector3D& vertical_direction,
                const math::Point3D& origin_point, double lens_radius_distance, const math::TimeRange& time_range_form_to);
            Camera() = default;
            Camera(const Camera&) = default;
            Camera(Camera&&) = default;
        };

        Camera Camera::new_camera_from_vertical_field(double field_of_view_y, double aspect)
        {
            auto theta = field_of_view_y * math::pi<math::Distance> / 180.0;
            auto half_height = std::tan(theta / 2.0);
            auto half_width = half_height * aspect;
            return Camera(
                math::Point3D(-half_width, -half_height, -1.0),
                math::Vector3D(2.0 * half_width, 0.0, 0.0),
                math::Vector3D(0.0, 2.0 * half_height, 0.0),
                math::Point3D(0.0),
                0.0,
                { 0.0, 0.0 } );
        }

        Camera::Camera(const math::Point3D& lower_left_point, const math::Vector3D& horizontal_direction, const math::Vector3D& vertical_direction,
            const math::Point3D& origin_point, double lens_radius_distance, const math::TimeRange& time_range_form_to)
            : lower_left_corner(lower_left_point)
            , horizontal(horizontal_direction)
            , vertical(vertical_direction)
            , origin(origin_point)
            , lens_radius(lens_radius_distance)
            , time_range(time_range_form_to)
            , u(math::normalize(horizontal_direction))
            , v(math::normalize(vertical_direction))
            , w(math::normalize(origin_point - lower_left_point - horizontal_direction * 0.5 - vertical_direction * 0.5))
        {}
    }
}

#endif