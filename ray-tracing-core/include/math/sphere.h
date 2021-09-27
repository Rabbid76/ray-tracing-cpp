#ifndef __RTC__MATH__SPHERE_H__
#define __RTC__MATH__SPHERE_H__

#include "types.h"
#include "axis_aligned_bounding_box.h"
#include "ray.h"
#include "hit_point.h"
#include <cmath>

namespace ray_tracing_core
{
	namespace math
	{
		class Sphere
		{
		public:

			Point3D center;
			Distance radius;

			static inline Sphere new_sphere(const Point3D &sphere_cente, const Distance &radius);
			inline AxisAlignedBoundingBox bounding_box(void) const;
			inline bool hit(const Ray &ray, const DistanceRange &distance_range, HitPoint &hit_point) const;
			inline Vector3D normal_vector(const Point3D &point_on_sphere) const;
		};

		Sphere Sphere::new_sphere(const Point3D &sphere_center, const Distance &sphere_radius)
		{
			return Sphere
			{
				center : sphere_center,
			    radius : sphere_radius,
			};
		}

		AxisAlignedBoundingBox Sphere::bounding_box(void) const
		{
			return AxisAlignedBoundingBox::new_box(
				center - Vector3D(radius),
				center + Vector3D(radius)
			);
		}

		/// Sphere:         dot(p-C, p-C) = R*R            `C`: center, `p`: point on the sphere, `R`, radius
		/// Ray:            p(t) = A + B * t               `A`: origin, `B`: direction
		/// Intersection:   dot(A +B*t-C, A+B*t-C) = R*R
		/// t*t*dot(B,B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R*R = 0
		bool Sphere::hit(const Ray &ray, const DistanceRange &distance_range, HitPoint &hit_point) const
		{
			const auto oc = ray.origin - center;
			const auto a = dot(ray.direction, ray.direction);
			const auto b = 2 * dot(oc, ray.direction);
			const auto c = dot(oc, oc) - radius * radius;

			const auto discriminant = b * b - 4 * a * c;
			if (discriminant <= 0)
				return false;
			const auto sqrt_discriminant = sqrt(discriminant);

			auto distance = (-b - sqrt_discriminant) / (2 * a);
			if (std::get<0>(distance_range) <= distance && distance < std::get<1>(distance_range))
			{
				hit_point.distance = distance;
				hit_point.position = ray.point_at(distance);
				hit_point.normal = normal_vector(hit_point.position);
				return true;
			}

			distance = (-b + sqrt_discriminant) / (2 * a);
			if (std::get<0>(distance_range) <= distance && distance < std::get<1>(distance_range))
			{
				hit_point.distance = distance;
				hit_point.position = ray.point_at(distance);
				hit_point.normal = normal_vector(hit_point.position);
				return true;
			}

			return false;
		}

		Vector3D Sphere::normal_vector(const Point3D &point_on_sphere) const
		{
			return (point_on_sphere - center) / radius;
		}
	}
}

#endif
