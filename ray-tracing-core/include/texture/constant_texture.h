#ifndef __RTC_UT__TEXTURE__CONSTANT_TEXTURE_H__
#define __RTC_UT__TEXTURE__CONSTANT_TEXTURE_H__

#include "texture.h"

namespace ray_tracing_core
{
	namespace texture
	{
		class ConstantTexture
			: public Texture
	    {
	    private:

			core::Color color;
			math::AlphaValue alpha_value;

	    public:

			inline ConstantTexture(const core::Color& texture_color, const math::AlphaValue texuture_alpha_value);

			virtual ~ConstantTexture(void) = default;
			inline virtual std::tuple<core::Color, math::AlphaValue> channels(
				const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
			inline virtual bool depends_on_texture_coordinates(void) const override;
			inline virtual bool has_alpha_channel(void) const override;
		};

		ConstantTexture::ConstantTexture(const core::Color& texture_color, const math::AlphaValue texuture_alpha_value)
			: color(texture_color)
			, alpha_value(texuture_alpha_value)
		{}

		std::tuple<core::Color, math::AlphaValue> ConstantTexture::channels(
			const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
		{
			return {color, alpha_value};
		}

		bool ConstantTexture::depends_on_texture_coordinates(void) const
		{
			return false;
		}

		bool ConstantTexture::has_alpha_channel(void) const
		{
			return alpha_value < 1;
		}
	}
}

#endif
