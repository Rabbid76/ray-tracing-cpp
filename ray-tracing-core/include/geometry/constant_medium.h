#ifndef __RTC__CONSTANT_MEDIUM_SPHERE_H__
#define __RTC__CONSTANT_MEDIUM_SPHERE_H__

#include "geometry/geometry.h"
#include "math/types.h"

namespace ray_tracing_core::geometry
{
    class ConstantMedium
        : public Geometry
    {
    private:

        math::Distance density;
        math::Distance negative_inverse_density;
        const Geometry *boundary;

    public:

        ConstantMedium(math::Distance density, const Geometry *boundary);
        virtual ~ConstantMedium() = default;
        virtual math::AxisAlignedBoundingBox bounding_box() const override;
        virtual bool hit(const math::Ray& ray, const math::DistanceRange& distance_range, bool set_texture_coordinatne,
            core::HitRecord& hit_record) const override;
        virtual math::Distance probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const override;
        virtual math::Vector3D random(const math::Point3D& origin) const override;
    };
}

#endif