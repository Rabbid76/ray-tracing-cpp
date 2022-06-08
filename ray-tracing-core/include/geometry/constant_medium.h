#ifndef __RTC__CONSTANT_MEDIUM_SPHERE_H__
#define __RTC__CONSTANT_MEDIUM_SPHERE_H__

#include "geometry/geometry.h"
#include "math/types.h"

namespace ray_tracing_core::geometry
{
    class ConstantMedium
        : public Geometry
    {
    public:
        enum class Type
        {
            Environment,
            Volume
        };

    private:
        Type type;
        math::Distance density;
        math::Distance negative_inverse_density;
        const Geometry *boundary;

    public:
        ConstantMedium(Type environment, math::Distance density, const Geometry *boundary);
        ~ConstantMedium() override = default;
        math::AxisAlignedBoundingBox bounding_box() const override;
        bool hit_distance_range(const math::Ray &ray, math::DistanceRange &hit_range) const override;
        bool hit(const math::Ray& ray, const math::DistanceRange& distance_range, bool set_texture_coordinate,
            core::HitRecord& hit_record) const override;
        math::Distance probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const override;
        math::Vector3D random(const math::Point3D& origin) const override;
    };
}

#endif