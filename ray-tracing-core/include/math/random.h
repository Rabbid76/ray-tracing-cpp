#ifndef __RTC__MATH__RANDOM_H__
#define __RTC__MATH__RANDOM_H__

#include "types.h"
#include <random>

namespace ray_tracing_core
{
	namespace math
	{
		class RandomGenerator
		{
		private:

			static std::default_random_engine generator;
	  	    static std::uniform_real_distribution<Distance> distribution_size;

		public:

	  	    Distance random_size(void) const;
	  	    Vector3D random_vector_to_sphere(Distance radius, Distance distance_squared) const;
		};
	}
}

#endif