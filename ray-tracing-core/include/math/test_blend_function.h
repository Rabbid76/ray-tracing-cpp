#ifndef __RTC__MATH__TEST_BLEND_FUNCTION_H__
#define __RTC__MATH__TEST_BLEND_FUNCTION_H__

#include "core/texture_coordinate.h"
#include "math/blend_function.h"
#include "math/types.h"
#include <algorithm>
#include <vector>

namespace ray_tracing_core::math
{
    class TestBlendFunction
            : public BlendFunction
    {
    public:
        enum class Type
        {
            Default,
            CosineHole,
            IcosahedronHoles
        };
    private:
        Type blend_type;
        Vector3D direction;
        Distance cosine;
        std::vector<Vector3D> directions;

    public:
        inline TestBlendFunction(Type blend_type, Distance cosine_value, const Vector3D &source_direction);
        inline math::AlphaValue evaluate(
                const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
        inline bool depends_on_texture_coordinates(void) const override;
    };

    TestBlendFunction::TestBlendFunction(Type blend_type, Distance cosine_value, const Vector3D &source_direction)
            : blend_type(blend_type)
            , direction(source_direction)
            , cosine(cosine_value)
    {
        switch (blend_type)
        {
            default:
            case Type::Default:
            case Type::CosineHole:
                if (math::dot(direction, direction) == 0)
                    direction = Vector3D(0, -1, 0);
                directions = { direction };
                break;
            case Type::IcosahedronHoles:
                cosine = std::max(cosine, 0.85);
                directions =
                        {
                                Vector3D(0,0,1), Vector3D(0.894f,0,0.447f), Vector3D(0.276f,0.851f,0.447f),
                                Vector3D(-0.724f,0.526f,0.447f), Vector3D(- 0.724f,-0.526f,0.447f), Vector3D(0.276f,-0.851f, 0.447f),
                                Vector3D(0.724f, 0.526f,-0.447f), Vector3D (-0.276f,0.851f,-0.447f), Vector3D(- 0.894f,0,-0.447f),
                                Vector3D(- 0.276f,-0.851f,-0.447f), Vector3D(0.724f,-0.526f,-0.447f), Vector3D(0,0,-1),
                        };
                break;
        }
    }

    math::AlphaValue TestBlendFunction::evaluate(
            const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
    {
        auto sphere_direction = core::TextureCoordinate::to_sphere(texture_coordinate);
        double maximum_dot = -1.0;
        for (auto& direction : directions)
        {
            auto d = math::dot(direction, sphere_direction);
            if (d > maximum_dot)
                maximum_dot = d;
        }
        return maximum_dot >= cosine ? 0.0f : 1.0f;
    }

    bool TestBlendFunction::depends_on_texture_coordinates(void) const
    {
        return true;
    }
}

#endif