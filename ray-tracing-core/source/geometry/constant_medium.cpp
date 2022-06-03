#include <geometry/constant_medium.h>
#include <math/ortho_normal_base.h>
#include <math/random.h>
#include <algorithm>
#include <cmath>
#include <limits>

namespace ray_tracing_core::geometry
{
    ConstantMedium::ConstantMedium(math::Distance density, const Geometry *boundary)
            : density(density)
            , negative_inverse_density(-1.0 / density)
            , boundary(boundary)
    {}

    math::AxisAlignedBoundingBox ConstantMedium::bounding_box() const
    {
        return boundary->bounding_box();
    }

    bool ConstantMedium::hit(
            const math::Ray &ray,
            const math::DistanceRange &distance_range,
            bool set_texture_coordinatne,
            core::HitRecord &hit_record) const
    {
        core::HitRecord hit_record_1;
        math::DistanceRange maximum_range = {std::numeric_limits<math::Distance>::lowest(), std::numeric_limits<math::Distance>::max()};
        if (!boundary->hit(ray, maximum_range, false, hit_record_1))
            return false;

        math::DistanceRange range2 = { hit_record_1.hit_point.distance + 0.0001, std::numeric_limits<math::Distance>::max()};
        core::HitRecord hit_record_2;
        if (!boundary->hit(ray, range2, false, hit_record_2))
            return false;

        hit_record_1.hit_point.distance = std::max(hit_record_1.hit_point.distance, std::get<0>(distance_range));
        hit_record_2.hit_point.distance = std::min(hit_record_2.hit_point.distance, std::get<1>(distance_range));
        if (hit_record_1.hit_point.distance >= hit_record_2.hit_point.distance)
            return false;
        hit_record_1.hit_point.distance = std::max(hit_record_1.hit_point.distance, 0.0);

        math::RandomGenerator genrator;
        auto ray_length = glm::length(ray.direction);
        auto distance_inside_boundary = (hit_record_2.hit_point.distance - hit_record_1.hit_point.distance) * ray_length;
        // Very fast approximate Logarithm (natural log) function in C++?
        // https://stackoverflow.com/questions/39821367/very-fast-approximate-logarithm-natural-log-function-in-c
        auto hit_distance = negative_inverse_density * std::log(genrator.random_size());
        if (hit_distance >= distance_inside_boundary)
            return false;

        hit_record.hit_point.distance = hit_record_1.hit_point.distance + hit_distance / ray_length;
        hit_record.hit_point.position = ray.point_at(hit_record.hit_point.distance);
        hit_record.hit_point.normal = math::Vector3D(0); // arbitrary genrator.random_in_unit_sphere();
        hit_record.texture_coordinate = core::TextureCoordinate{0.0f, 0.0f};
        return true;
    }

    math::Distance ConstantMedium::probability_density_function_value(const math::Point3D &origin, const math::Vector3D &direction) const
    {
        return boundary->probability_density_function_value(origin, direction);
    }

    math::Vector3D ConstantMedium::random(const math::Point3D &origin) const
    {
        return boundary->random(origin);
    }
}