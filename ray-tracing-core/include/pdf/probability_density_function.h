#ifndef __RTC_CORE__PDF__PROBABILITY_DENSITY_FUNCTION_H__
#define __RTC_CORE__PDF__PROBABILITY_DENSITY_FUNCTION_H__

#include <math/types.h>

namespace ray_tracing_core
{
	namespace pdf
	{
		class ProbabilityDensityFunction
		{
		public:

			virtual ~ProbabilityDensityFunction(void) = default;
			virtual math::Distance value(const math::Vector3D &direction) const = 0;
			virtual math::Vector3D generate(void) const = 0;
		};
	}
}

#endif
