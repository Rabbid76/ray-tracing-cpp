#include <geometry/sphere.h>

namespace ray_tracing_core
{
	namespace geometry
	{
		Sphere::Sphere(const math::Point3D &center_point, math::Distance sphere_radius)
			: center(center_point)
			, radius(sphere_radius)
		{}

		math::AxisAlignedBoundingBox Sphere::bounding_box(void) const
		{
			return math::AxisAlignedBoundingBox::new_box(
				center - math::Vector3D(radius),
				center + math::Vector3D(radius)
			);
		}
	}
}
