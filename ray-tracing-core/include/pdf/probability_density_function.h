#ifndef __RTC__PDF__PROBABILITY_DENSITY_FUNCTION_H__
#define __RTC__PDF__PROBABILITY_DENSITY_FUNCTION_H__

#include <math/types.h>

namespace ray_tracing_core::pdf
{
    enum class ProbabilityDensityFunctionType
    {
        None,
        CosinePDFFromNormal
    };

    class ProbabilityDensityFunction
    {
    public:

        virtual ~ProbabilityDensityFunction() = default;
        virtual math::Distance value(const math::Vector3D &direction) const = 0;
        virtual math::Vector3D generate() const = 0;
    };
}

#endif
