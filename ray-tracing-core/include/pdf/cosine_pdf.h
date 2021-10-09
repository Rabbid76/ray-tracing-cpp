#ifndef __RTC_CORE__COSINE_PDF_H__
#define __RTC_CORE__COSINE_PDF_H__

#include "probability_density_function.h"
#include "math/ortho_normal_base.h"
#include "math/random.h"
#include "math/constants.h"

namespace ray_tracing_core
{
	namespace pdf
	{
		class CosinePDF
			: public ProbabilityDensityFunction
		{
		private:

			math::OrthoNormalBase ortho_normal_base;

		public:

			static inline CosinePDF from_normal(const math::Vector3D &normal_vector);

			inline CosinePDF(math::OrthoNormalBase&& ortho_normal);
			virtual ~CosinePDF(void) = default;
			inline virtual math::Distance value(const math::Vector3D &direction) const;
			inline virtual math::Vector3D generate(void) const;

		};

		CosinePDF CosinePDF::from_normal(const math::Vector3D &normal_vector)
		{
			return CosinePDF(math::OrthoNormalBase::from_normal(normal_vector));
		}

		CosinePDF::CosinePDF(math::OrthoNormalBase&& ortho_normal)
			: ortho_normal_base(ortho_normal)
		{}

		math::Distance CosinePDF::value(const math::Vector3D &direction) const
		{
			auto n_dot_d = math::dot(math::normalize(direction), ortho_normal_base.w());
			return n_dot_d < 0 ? 0 : n_dot_d / math::pi<math::Distance>;
		}

		math::Vector3D CosinePDF::generate(void) const
		{
			return ortho_normal_base
				.transform(math::RandomGenerator()
					.random_cosine_direction());
		}
	}
}

#endif
