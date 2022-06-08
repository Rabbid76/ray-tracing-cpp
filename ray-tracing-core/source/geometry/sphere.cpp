#include <geometry/sphere.h>
#include <math/ortho_normal_base.h>
#include <math/random.h>
#include <limits>
#include <cmath>

namespace ray_tracing_core::geometry
{
    Sphere::Sphere(const math::Point3D &center_point, math::Distance sphere_radius)
            : sphere_geometry(math::Sphere::new_sphere(center_point, sphere_radius))
    {}

    math::AxisAlignedBoundingBox Sphere::bounding_box(void) const
    {
        return sphere_geometry.bounding_box();
    }

    bool Sphere::hit_distance_range(const math::Ray &ray, math::DistanceRange &hit_range) const
    {
        return sphere_geometry.hit_distance_range(ray, hit_range);
    }

    bool Sphere::hit(
            const math::Ray &ray,
            const math::DistanceRange &distance_range,
            bool set_texture_coordinate,
            core::HitRecord &hit_record) const
    {
        if (!sphere_geometry.hit(ray, distance_range, hit_record.hit_point))
            return false;
        hit_record.texture_coordinate = set_texture_coordinate
                                        ? core::TextureCoordinate::from_sphere(hit_record.hit_point.normal)
                                        : core::TextureCoordinate::null();
        return true;
    }

    math::Distance Sphere::probability_density_function_value(const math::Point3D &origin, const math::Vector3D &direction) const
    {
        math::HitPoint hit_point;
        const math::DistanceRange distance_range(0.001, std::numeric_limits<math::Distance>::max());
        if (!sphere_geometry.hit(math::Ray::new_ray(origin, direction), distance_range, hit_point))
            return 0;

        const auto cos_theta_max = std::sqrt(1 - sphere_geometry.radius * sphere_geometry.radius /
                                                 dot(sphere_geometry.center - origin, sphere_geometry.center - origin));
        const auto solid_angle = 2 * math::pi<math::Distance> * (1 - cos_theta_max);
        return 1 / solid_angle;
    }

    math::Vector3D Sphere::random(const math::Point3D &origin) const
    {
        auto direction = sphere_geometry.center - origin;
        auto distance_squared = math::dot(direction, direction);
        auto ortho_normal_base = math::OrthoNormalBase::from_normal(direction);
        auto random_vector = math::RandomGenerator().random_vector_to_sphere(sphere_geometry.radius, distance_squared);
        return ortho_normal_base.transform(random_vector);
    }
}
