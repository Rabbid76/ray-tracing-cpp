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
			inline static TextureCoordinate constant(math::TextureDistance textue_u, math::TextureDistance texture_v);
			inline static TextureCoordinate from_sphere(const math::Vector3D &vector_form_center);
			inline static math::Vector3D to_sphere(const TextureCoordinate& texture_coordiante);
		};

		TextureCoordinate TextureCoordinate::null(void)
		{
			return TextureCoordinate{ 0, 0 };
		}

		TextureCoordinate TextureCoordinate::constant(math::TextureDistance textue_u, math::TextureDistance texture_v)
		{
			return TextureCoordinate{ textue_u, texture_v };
		}

		TextureCoordinate TextureCoordinate::from_sphere(const math::Vector3D &vector_form_center)
		{
			auto phi = std::atan2(vector_form_center.z, vector_form_center.x);
			auto theta = std::asin(vector_form_center.y);
			const float pi = math::pi<math::TextureDistance>;
			return TextureCoordinate
			{
				.u = static_cast<math::TextureDistance>(1.0 - (phi + pi) / (2.0 * pi)),
				.v = static_cast<math::TextureDistance>((theta + pi/2.0) / pi)
			};
		}

		math::Vector3D TextureCoordinate::to_sphere(const TextureCoordinate& texture_coordiante)
		{
			const float pi = math::pi<math::TextureDistance>;
			auto phi = (texture_coordiante.u * 2.0 - 1.0) * pi;
			auto theta = (texture_coordiante.v * 2.0 - 1.0) * pi / 2.0;
			auto y = std::sin(theta);
			auto xy_scale = std::sqrt(1.0 - y*y);
			auto x = xy_scale * std::sin(phi);
			auto z = xy_scale * std::cos(phi);
			return math::Vector3D{ x, y, z };
		}
	}
}

#endif
