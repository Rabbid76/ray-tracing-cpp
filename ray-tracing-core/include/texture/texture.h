#ifndef __RTC__TEXTURE__TEXTURE_TEST_H__
#define __RTC__TEXTURE__TExTURE_TEST_H__

#include <math/types.h>
#include <core/color.h>
#include <core/texture_coordinate.h>
#include <tuple>

namespace ray_tracing_core
{
	namespace texture
	{
		class Texture
		{
		public:

			virtual ~Texture(void) = default;
			virtual std::tuple<core::Color, math::AlphaValue> channels(
				const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const = 0;
			virtual bool depends_on_texture_coordinates(void) const = 0;
			virtual bool has_alpha_channel(void) const = 0;
		};
	}
}

#endif
