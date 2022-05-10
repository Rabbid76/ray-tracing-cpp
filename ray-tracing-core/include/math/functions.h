#ifndef __RTC__MATH__FUNCTIONS_H__
#define __RTC__MATH__FUNCTIONS_H__

#include "types.h"
#include <cmath>
#include <optional>

namespace ray_tracing_core::math
{
    std::optional<Vector3D> refract(const Vector3D &v, const Vector3D &n, double ni_over_nt)
    {
        auto uv = glm::normalize(v);
        auto dt = glm::dot(uv, n);
        auto discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
        return discriminant > 0.0
            ? std::optional<Vector3D>((uv - n * dt) * ni_over_nt - n * std::sqrt(discriminant))
            : std::nullopt;
    }

    double schlick(double cosine, double ref_idx)
    {
        auto r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * std::powf(1.0 - cosine, 5.0);
    }
}

#endif
