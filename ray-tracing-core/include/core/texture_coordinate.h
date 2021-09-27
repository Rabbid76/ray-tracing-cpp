#ifndef __RTC__CORE__TEXTURE_COORDINATE_H__
#define __RTC__CORE__TEXTURE_COORDINATE_H__

#include <math/types.h>
#include <math/constants.h>
#include <cmath>

namespace ray_tracing_core
{
	namespace core
	{
		class TextureCoordinate
		{
		public:

			math::TextureDistance u;
			math::TextureDistance v;

			inline static TextureCoordinate null(void);
			inline static TextureCoordinate from_sphere(const math::Vector3D &vector_form_center);
		};

		TextureCoordinate TextureCoordinate::null(void)
		{
			return TextureCoordinate{ 0, 0 };
		}

		TextureCoordinate TextureCoordinate::from_sphere(const math::Vector3D &vector_form_center)
		{
			auto phi = std::atan2(vector_form_center.z, vector_form_center.x);
			auto theta = std::asin(vector_form_center.y);
			const float pi = math::pi<math::TextureDistance>;
			return TextureCoordinate
			{
				u : static_cast<math::TextureDistance>(1.0 - (phi + pi) / (2.0 * pi)),
				v : static_cast<math::TextureDistance>((theta + pi / 2.0) / pi)
			};
		}
	}
}

#endif
