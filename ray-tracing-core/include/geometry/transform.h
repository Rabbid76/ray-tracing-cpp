#ifndef __RTC__GEOMETRY__TRANSFORM_H__
#define __RTC__GEOMETRY__TRANSFORM_H__

#include "geometry/geometry.h"
#include "math/sphere.h"
#include "math/types.h"

namespace ray_tracing_core::geometry
{
    class Transform
        : public Geometry
    {
    private:
        const Geometry *geometry;
        math::TransformationMatrix model_matrix;
        math::TransformationMatrix inverse_model_matrix;
        math::NormalMatrix normal_matrix;
        math::NormalMatrix inverse_normal_matrix;

    public:
        Transform(const Geometry* geometry, const math::Point3D &translate, double angle, const math::Vector3D &axis, const math::Vector3D &scale);
        ~Transform() override = default;
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
