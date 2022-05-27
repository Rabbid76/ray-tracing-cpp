#ifndef __RTC__GEOMETRY__RECTANGLE_H__
#define __RTC__GEOMETRY__RECTANGLE_H__

#include "geometry/geometry.h"
#include "math/rectangle.h"
#include "math/types.h"

namespace ray_tracing_core::geometry
{
    using namespace ray_tracing_core;

	class Rectangle
		: public Geometry
	{
    private:

        math::Rectangle rectangle_geometry;

    public:

        Rectangle(math::Rectangle::Orientation orientation, math::Distance k, const math::Point2D& a, const math::Point2D& b);
        virtual ~Rectangle() = default;
        virtual math::AxisAlignedBoundingBox bounding_box(void) const override;
        virtual bool hit(const math::Ray& ray, const math::DistanceRange& distance_range, bool set_texture_coordinatne,
            core::HitRecord& hit_record) const override;
        virtual math::Distance probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const override;
        virtual math::Vector3D random(const math::Point3D& origin) const override;
	};
}

#endif