#ifndef __RTC__GEOMETRY__SPHERE_H__
#define __RTC__GEOMETRY__SPHERE_H__

#include "geometry.h"
#include <math/types.h>
#include <math/sphere.h>

namespace ray_tracing_core
{
    namespace geometry
    {
        class Sphere
            : public Geometry
        {
        private:

            math::Sphere sphere_geometry;

        public:

            Sphere(const math::Point3D& center_point, math::Distance sphere_radius);
            virtual ~Sphere() = default;
            virtual math::AxisAlignedBoundingBox bounding_box(void) const override;
            virtual bool hit(const math::Ray& ray, const math::DistanceRange& distance_range, bool set_texture_coordinatne,
                core::HitRecord& hit_record) const override;
            virtual math::Distance probability_density_function_value(
                const math::Point3D& origin, const math::Vector3D& direction) const override;
            virtual math::Vector3D random(const math::Point3D& origin) const override;
        };
    }
}

#endif
