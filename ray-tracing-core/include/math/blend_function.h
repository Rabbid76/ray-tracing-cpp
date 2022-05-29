#ifndef __RTC__MATH__BLEND_FUNCTION_H__
#define __RTC__MATH__BLEND_FUNCTION_H__

#include "core/color.h"
#include "core/texture_coordinate.h"
#include "types.h"

namespace ray_tracing_core::math {
    class BlendFunction {
    public:
        virtual ~BlendFunction() = default;
        virtual math::AlphaValue evaluate(
                const core::TextureCoordinate &texture_coordinate, const math::Point3D &position) const = 0;
        virtual bool depends_on_texture_coordinates(void) const = 0;
    };
}

#endif