#include "geometry/transform.h"
#include <cmath>

namespace ray_tracing_core::geometry
{
	Transform::Transform(
		const Geometry* geometry, 
		const math::Point3D& translate, 
		double angle, const math::Vector3D& axis, 
		const math::Vector3D& scale)
		: geometry(geometry)
	{
		model_matrix = glm::dmat4(1.0);
		if (glm::dot(translate, translate) > 0)
			model_matrix = glm::translate(model_matrix, translate);
		if (angle != 0 && glm::dot(axis, axis) != 0)
			model_matrix = glm::rotate(model_matrix, glm::radians(angle), axis);
		if (glm::dot(scale, scale) != 0)
			model_matrix = glm::scale(model_matrix, scale);
		inverse_model_matrix = glm::inverse(model_matrix);
		inverse_normal_matrix = glm::transpose(glm::dmat3(model_matrix));
		normal_matrix = glm::inverse(inverse_normal_matrix);
	}

	math::AxisAlignedBoundingBox Transform::bounding_box(void) const
	{
		return geometry->bounding_box().transform(model_matrix);
	}

	bool Transform::hit(
		const math::Ray& ray, 
		const math::DistanceRange& distance_range, 
		bool set_texture_coordinatne,
		core::HitRecord& hit_record) const
	{
		auto transformed_ray = ray.new_ray_with_attributes(
			math::transform(ray.origin, inverse_model_matrix),
			math::transform(ray.direction, inverse_normal_matrix),
			ray);
		if (!geometry->hit(transformed_ray, distance_range, set_texture_coordinatne, hit_record))
			return false;
		hit_record.hit_point = math::HitPoint::new_hit(
			hit_record.hit_point.distance,
			math::transform(hit_record.hit_point.position, model_matrix),
			math::transform(hit_record.hit_point.normal, normal_matrix));
		return true;
	}

	math::Distance Transform::probability_density_function_value(const math::Point3D& origin, const math::Vector3D& direction) const
	{
		return geometry->probability_density_function_value(
			math::transform(origin, inverse_model_matrix),
			math::transform(direction, inverse_normal_matrix));
	}

	math::Vector3D Transform::random(const math::Point3D& origin) const
	{
		return geometry->random(math::transform(origin, inverse_model_matrix));
	}
}
