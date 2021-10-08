#include <math/random.h>
#include <math/constants.h>
#include <cmath>

namespace ray_tracing_core
{
	namespace math
	{
		std::default_random_engine RandomGenerator::generator;
		std::uniform_real_distribution<Distance> RandomGenerator::distribution_size =
			std::uniform_real_distribution<Distance>(0.0, 1.0);
		std::uniform_real_distribution<Distance> RandomGenerator::distribution_unit =
			std::uniform_real_distribution<Distance>(-1.0, 1.0);

		Distance RandomGenerator::random_size(void) const
		{
			return distribution_size(generator);
		}

		Distance RandomGenerator::random_unit(void) const
		{
			return distribution_unit(generator);
		}

		Vector3D RandomGenerator::random_vector_to_sphere(Distance radius, Distance distance_squared) const
		{
			auto r1 = random_size();
			auto r2 = random_size();
			auto z = 1.0 + r2 * (std::sqrt(1.0 - radius * radius / distance_squared) - 1.0);
			auto phi = 2.0 * pi<Distance> * r1;
			auto x = std::cos(phi) * std::sqrt(1.0 - z * z);
			auto y = std::sin(phi) * std::sqrt(1.0 - z * z);
			return Vector3D(x, y, z);
		}
	}
}
