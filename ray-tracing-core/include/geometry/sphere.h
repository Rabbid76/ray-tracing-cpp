#ifndef __RTC__GEOMETRY__SPHERE_H__
#define __RTC__GEOMETRY__SPHERE_H__

#include "geometry.h"
#include <math/types.h>
#include <math/Sphere.h>

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

			Sphere(const math::Point3D &center_point, math::Distance sphere_radius);
			virtual ~Sphere() = default;
			virtual math::AxisAlignedBoundingBox bounding_box(void) const override;
			virtual bool hit(const math::Ray &ray, const math::DistanceRange &distance_range, core::HitRecord &hit_record) const override;
		};
	}
}

#endif
