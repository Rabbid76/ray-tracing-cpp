#include <geometry/constant_medium.h>
#include <math/ortho_normal_base.h>
#include <math/random.h>
#include <algorithm>
#include <cmath>
#include <limits>

namespace {
    float __int_as_float (int32_t a) { float r; memcpy (&r, &a, sizeof r); return r;}
    int32_t __float_as_int (float a) { int32_t r; memcpy (&r, &a, sizeof r); return r;}

    float my_faster_logf(float a)
    {
        float m, r, s, t, i, f;
        int32_t e;

        e = (__float_as_int (a) - 0x3f2aaaab) & 0xff800000;
        m = __int_as_float (__float_as_int (a) - e);
        i = (float)e * 1.19209290e-7f; // 0x1.0p-23
        /* m in [2/3, 4/3] */
        f = m - 1.0f;
        s = f * f;
        /* Compute log1p(f) for f in [-1/3, 1/3] */
        r = fmaf (0.230836749f, f, -0.279208571f); // 0x1.d8c0f0p-3, -0x1.1de8dap-2
        t = fmaf (0.331826031f, f, -0.498910338f); // 0x1.53ca34p-2, -0x1.fee25ap-2
        r = fmaf (r, s, t);
        r = fmaf (r, s, f);
        r = fmaf (i, 0.693147182f, r); // 0x1.62e430p-1 // log(2)
        return r;
    }
}

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

    bool ConstantMedium::hit_distance_range(const math::Ray &ray, math::DistanceRange &hit_range) const
    {
        return boundary->hit_distance_range(ray, hit_range);
    }

    bool ConstantMedium::hit(
            const math::Ray &ray,
            const math::DistanceRange &distance_range,
            bool set_texture_coordinate,
            core::HitRecord &hit_record) const
    {
        math::DistanceRange range_in_medium;
        if (!boundary->hit_distance_range(ray, range_in_medium))
            return false;
        auto from = std::max(0.0, std::max(std::get<0>(distance_range), std::get<0>(range_in_medium)));
        auto to = std::min(std::get<1>(distance_range), std::get<1>(range_in_medium));
        if (from >= to)
            return false;

        math::RandomGenerator generator;
        auto ray_length = glm::length(ray.direction);
        auto distance_inside_boundary = (to - from) * ray_length;
        // Very fast approximate Logarithm (natural log) function in C++?
        // https://stackoverflow.com/questions/39821367/very-fast-approximate-logarithm-natural-log-function-in-c
        //auto hit_distance = negative_inverse_density * std::log(generator.random_size());
        auto hit_distance = negative_inverse_density * my_faster_logf(static_cast<float>(generator.random_size()));
        if (hit_distance >= distance_inside_boundary)
            return false;

        hit_record.hit_point.distance = from + hit_distance / ray_length;
        hit_record.hit_point.position = ray.point_at(hit_record.hit_point.distance);
        hit_record.hit_point.normal = math::Vector3D(0); // arbitrary generator.random_in_unit_sphere();
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