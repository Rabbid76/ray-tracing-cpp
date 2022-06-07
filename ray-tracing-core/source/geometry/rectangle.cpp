#include "geometry/rectangle.h"
#include "math/random.h"
#include <cmath>

namespace ray_tracing_core::geometry
{
	Rectangle::Rectangle(math::Rectangle::Orientation orientation, math::Distance k, const math::Point2D& a, const math::Point2D& b)
		: rectangle_geometry(math::Rectangle::new_rectangle(orientation, k, a, b))
	{}

	math::AxisAlignedBoundingBox Rectangle::bounding_box(void) const
	{
		return rectangle_geometry.bounding_box();
	}

    bool Rectangle::hit_distance_range(const math::Ray &ray, math::DistanceRange &hit_range) const
    {
        return rectangle_geometry.hit_distance_range(ray, hit_range);
    }

	bool Rectangle::hit(
		const math::Ray& ray, 
		const math::DistanceRange& distance_range, 
		bool set_texture_coordinatne,
		core::HitRecord& hit_record) const
	{
		if (!rectangle_geometry.hit(ray, distance_range, hit_record.hit_point))
			return false;
		hit_record.texture_coordinate = set_texture_coordinatne
			? rectangle_geometry.texture_coordinates(hit_record.hit_point.position)
			: core::TextureCoordinate::null();
		return true;
	}

	math::Distance Rectangle::probability_density_function_value(const math::Point3D& origin, const math::Vector3D& direction) const
	{
		math::HitPoint hit_point;
		const math::DistanceRange distance_range(0.001, std::numeric_limits<math::Distance>::max());
		if (!rectangle_geometry.hit(math::Ray::new_ray(origin, direction), distance_range, hit_point))
			return 0;

		auto area = rectangle_geometry.area();
		auto distance_squared = hit_point.distance * hit_point.distance * glm::dot(direction, direction);
		auto cosine = std::fabs(glm::dot(direction, hit_point.normal) / glm::length(direction));
		return distance_squared / (cosine * area);
	}

	math::Vector3D Rectangle::random(const math::Point3D& origin) const
	{
		return rectangle_geometry.random_point() - origin;
	}
}