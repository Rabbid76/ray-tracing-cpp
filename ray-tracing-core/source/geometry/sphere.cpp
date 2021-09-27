#include <geometry/sphere.h>

namespace ray_tracing_core
{
	namespace geometry
	{
		Sphere::Sphere(const math::Point3D &center_point, math::Distance sphere_radius)
			: sphere_geometry(math::Sphere::new_sphere(center_point, sphere_radius))
		{}

		math::AxisAlignedBoundingBox Sphere::bounding_box(void) const
		{
			return sphere_geometry.bounding_box();
		}

		bool Sphere::hit(const math::Ray &ray, const math::DistanceRange &distance_range, bool set_texture_coordinatne,
				core::HitRecord &hit_record) const
		{
			if (!sphere_geometry.hit(ray, distance_range, hit_record.hit_point))
			    return false;
			hit_record.texture_coordinate = set_texture_coordinatne
					? core::TextureCoordinate::from_sphere(hit_record.hit_point.normal)
					: core::TextureCoordinate::null();
			return true;
		}
	}
}
