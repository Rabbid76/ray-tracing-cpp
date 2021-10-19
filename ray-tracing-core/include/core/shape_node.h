#ifndef __RTC__CORE__SHAPE_NODE_H__
#define __RTC__CORE__SHAPE_NODE_H__

#include <math/ray.h>
#include <math/axis_aligned_bounding_box.h>
#include <core/hit_record.h>

namespace ray_tracing_core
{
    namespace core
    {
        class ShapeNode
        {
        public:

            virtual ~ShapeNode() = default;
            virtual math::AxisAlignedBoundingBox bounding_box(void) const = 0;
            virtual bool hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const = 0;
            virtual math::Distance probability_density_function_value(const math::Point3D& origin, const math::Vector3D& direction) const = 0;
            virtual math::Vector3D random(const math::Point3D& origin) const = 0;
        };
    }
}

#endif