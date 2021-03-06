#ifndef __RTC__MATH__RECTANGLE_H__
#define __RTC__MATH__RECTANGLE_H__

#include "core/texture_coordinate.h"
#include "math/axis_aligned_bounding_box.h"
#include "math/hit_point.h"
#include "math/ray.h"
#include "math/random.h"
#include "math/types.h"
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace ray_tracing_core::math
{
    class Rectangle
    {
    public:
        enum class Orientation : uint32_t
        {
            YZ=0, XZ=1, XY=2
        };

        Orientation orientation;
        Distance k;
        DistanceRange rangeA;
        DistanceRange rangeB;

        inline static Rectangle new_rectangle(Orientation orientation, Distance k, const Point2D& a, const Point2D& b);
        inline AxisAlignedBoundingBox bounding_box(void) const;
        inline bool hit_distance_range(const Ray &ray, DistanceRange &hit_range) const;
        inline bool hit(const Ray& ray, const DistanceRange& distance_range, HitPoint& hit_point) const;
        inline Vector3D normal_vector() const;
        inline core::TextureCoordinate texture_coordinates(const Point3D & point_in_rectangle) const;
        inline math::Distance area() const;
        inline math::Vector3D random_point() const;
    };

    Rectangle Rectangle::new_rectangle(Orientation orientation, Distance k, const Point2D& a, const Point2D& b)
    {
        return Rectangle
                {
                        .orientation = orientation,
                        .k = k,
                        .rangeA = { std::min(a.x, b.x), std::max(a.x, b.x) },
                        .rangeB = { std::min(a.y, b.y), std::max(a.y, b.y) },
                };
    }

    AxisAlignedBoundingBox Rectangle::bounding_box(void) const
    {
        auto& [a0, a1] = rangeA;
        auto& [b0, b1] = rangeB;
        switch (orientation)
        {
            default:
            case Orientation::XY: return AxisAlignedBoundingBox::new_box(Point3D{ a0, b0, k - 0.0001 }, Point3D{ a1, b1, k + 0.0001 });
            case Orientation::YZ: return AxisAlignedBoundingBox::new_box(Point3D{ k - 0.0001, a0, b0 }, Point3D{ k + 0.0001, a1, b1 });
            case Orientation::XZ: return AxisAlignedBoundingBox::new_box(Point3D{ a0, k - 0.0001, b0 }, Point3D{ a1, k + 0.0001, b1 });
        }
    }

    bool Rectangle::hit_distance_range(const Ray &ray, DistanceRange &hit_range) const
    {
        if (ray.direction[(uint32_t)orientation] == 0)
            return false;
        auto t = (k - ray.origin[(uint32_t)orientation]) / ray.direction[(uint32_t)orientation];
        auto& [a0, a1] = rangeA;
        auto& [b0, b1] = rangeB;
        uint32_t iA = (orientation == Orientation::YZ) ? 1 : 0;
        uint32_t iB = (orientation == Orientation::XY) ? 1 : 2;
        auto a = ray.origin[iA] + t * ray.direction[iA];
        auto b = ray.origin[iB] + t * ray.direction[iB];
        if (a < a0 || a > a1 || b < b0 || b > b1)
            return false;
        hit_range = { t, t};
        return true;
    }

    bool Rectangle::hit(const Ray& ray, const DistanceRange& distance_range, HitPoint& hit_point) const
    {
        auto [from, to] = distance_range;
        if (ray.direction[(uint32_t)orientation] == 0)
            return false;
        auto t = (k - ray.origin[(uint32_t)orientation]) / ray.direction[(uint32_t)orientation];
        if (t < from || t > to)
            return false;

        auto& [a0, a1] = rangeA;
        auto& [b0, b1] = rangeB;
        uint32_t iA = (orientation == Orientation::YZ) ? 1 : 0;
        uint32_t iB = (orientation == Orientation::XY) ? 1 : 2;
        auto a = ray.origin[iA] + t * ray.direction[iA];
        auto b = ray.origin[iB] + t * ray.direction[iB];
        if (a < a0 || a > a1 || b < b0 || b > b1)
            return false;

        hit_point.distance = t;
        hit_point.position = ray.point_at(t);
        hit_point.normal = normal_vector();
        return true;
    }

    Vector3D Rectangle::normal_vector() const
    {
        switch (orientation)
        {
            default:
            case Orientation::XY: return Vector3D(0, 0, -1);
            case Orientation::YZ: return Vector3D(-1, 0, 0);
            case Orientation::XZ: return Vector3D(0, -1, 0);
        }
    }

    core::TextureCoordinate Rectangle::texture_coordinates(const Point3D& point_in_rectangle) const
    {
        auto& [a0, a1] = rangeA;
        auto& [b0, b1] = rangeB;
        uint32_t iA = (orientation == Orientation::YZ) ? 1 : 0;
        uint32_t iB = (orientation == Orientation::XY) ? 1 : 2;
        return core::TextureCoordinate::constant
                (
                        static_cast<math::TextureDistance>((point_in_rectangle[iA] - a0) / (a1 - a0)),
                        static_cast<math::TextureDistance>((point_in_rectangle[iB] - b0) / (b1 - b0))
                );
    }

    math::Distance Rectangle::area() const
    {
        auto& [a0, a1] = rangeA;
        auto& [b0, b1] = rangeB;
        return (a1 - a0) * (b1 - b0);
    }

    math::Vector3D Rectangle::random_point() const
    {
        math::RandomGenerator generator;
        auto& [a0, a1] = rangeA;
        auto& [b0, b1] = rangeB;
        uint32_t iA = (orientation == Orientation::YZ) ? 1 : 0;
        uint32_t iB = (orientation == Orientation::XY) ? 1 : 2;
        Point3D point;
        point[iA] = a0 + (a1 - a0) * generator.random_size();
        point[iB] = b0 + (b1 - b0) * generator.random_size();
        point[(uint32_t)orientation] = k;
        return point;
    }
}

#endif