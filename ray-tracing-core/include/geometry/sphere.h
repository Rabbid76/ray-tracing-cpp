#ifndef __RTC__GEOMETRY__SPHERE_H__
#define __RTC__GEOMETRY__SPHERE_H__

#include "geometry.h"
#include <math/types.h>

namespace ray_tracing_core
{
	namespace geometry
	{
		class Sphere
			: public Geometry
		{
		private:

			math::Point3D center;
			math::Distance radius;

		public:

			Sphere(const math::Point3D &center_point, math::Distance sphere_radius);
			virtual math::AxisAlignedBoundingBox bounding_box(void) const override;
		};
	}
}

#endif
