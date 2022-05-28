#ifndef __RTC__MATH__AXIS_ALIGNED_BOUNDIG_BOX_H__
#define __RTC__MATH__AXIS_ALIGNED_BOUNDIG_BOX_H__

#include "types.h"
#include "ray.h"
#include <utility>
#include <algorithm>

namespace ray_tracing_core
{
    namespace math
    {
        class AxisAlignedBoundingBox;
        using AABB = AxisAlignedBoundingBox;

        class AxisAlignedBoundingBox
        {
        public:

            Point3D minimum_point;
            Point3D maximum_point;

            static inline AxisAlignedBoundingBox new_box(const Point3D& pt1, const Point3D& pt2);

            inline bool hit(const Ray& ray, const DistanceRange& range) const;
            inline AxisAlignedBoundingBox& operator |= (const Point3D& point);
            inline AxisAlignedBoundingBox& operator |= (const AxisAlignedBoundingBox& other_box);
            inline AxisAlignedBoundingBox transform(const math::TransformationMatrix &trasformation) const;
        };

        AxisAlignedBoundingBox AxisAlignedBoundingBox::new_box(const Point3D& pt1, const Point3D& pt2)
        {
            return AxisAlignedBoundingBox
            {
                .minimum_point = minimum(pt1, pt2),
                .maximum_point = maximum(pt1, pt2)
            };
        }

        bool AxisAlignedBoundingBox::hit(const Ray& ray, const DistanceRange& range) const
        {
            for (int axis = 0; axis < 3; axis++)
            {
                const auto inv_d = 1.0 / ray.direction[axis];
                auto t0 = (minimum_point[axis] - ray.origin[axis]) * inv_d;
                auto t1 = (maximum_point[axis] - ray.origin[axis]) * inv_d;
                if (inv_d < 0.0)
                    std::swap(t0, t1);
                if (std::max(t0, std::get<0>(range)) >= std::min(t1, std::get<1>(range)))
                    return false;
            }
            return true;
        }

        AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator |= (const Vector3D& point)
        {
            minimum_point = minimum(minimum_point, point);
            maximum_point = maximum(maximum_point, point);
            return *this;
        }

        AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator |= (const AxisAlignedBoundingBox& other_box)
        {
            minimum_point = minimum(minimum_point, other_box.minimum_point);
            maximum_point = maximum(maximum_point, other_box.maximum_point);
            return *this;
        }

        AxisAlignedBoundingBox AxisAlignedBoundingBox::transform(const math::TransformationMatrix& trasformation) const
        {
            return AxisAlignedBoundingBox::new_box(
                math::transform(minimum_point, trasformation),
                math::transform(maximum_point, trasformation));
        }
    }
}

#endif
