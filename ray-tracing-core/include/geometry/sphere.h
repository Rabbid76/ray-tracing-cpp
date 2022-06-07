#ifndef __RTC__GEOMETRY__SPHERE_H__
#define __RTC__GEOMETRY__SPHERE_H__

#include "geometry/geometry.h"
#include "math/sphere.h"
#include "math/types.h"

namespace ray_tracing_core::geometry
{
    class Sphere
        : public Geometry
    {
    private:
        math::Sphere sphere_geometry;

    public:
        Sphere(const math::Point3D& center_point, math::Distance sphere_radius);
        ~Sphere() override = default;
        math::AxisAlignedBoundingBox bounding_box(void) const override;
        bool hit_distance_range(const math::Ray &ray, math::DistanceRange &hit_range) const override;
        bool hit(const math::Ray& ray, const math::DistanceRange& distance_range, bool set_texture_coordinate,
            core::HitRecord& hit_record) const override;
        math::Distance probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const override;
        math::Vector3D random(const math::Point3D& origin) const override;
    };
}

#endif
