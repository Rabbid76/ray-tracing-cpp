#ifndef __RTC__GEOMETRY__GEOMETRY_H__
#define __RTC__GEOMETRY__GEOMETRY_H__

#include "math/axis_aligned_bounding_box.h"

namespace ray_tracing_core
{
	namespace geometry
	{
		class Geometry
		{
		public:

			virtual math::AxisAlignedBoundingBox bounding_box(void) const = 0;
		};
	}
}

#endif
