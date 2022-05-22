#ifndef __RTC__MATH__IMAGE_CHANNEL_BLEND_FUNCTION_H__
#define __RTC__MATH__IMAGE_CHANNEL_BLEND_FUNCTION_H__

#include "math/blend_function.h"
#include "texture/texture.h"

namespace ray_tracing_core::math
{
	class ImageChannelBlendFunction
		: public BlendFunction
	{
	public:
		enum class Channel
		{
			Red, 
			Green, 
			Blue, 
			Alpha
		};
	
	private:
		Channel channel;
		const texture::Texture *blend_texture;

	public:
		inline ImageChannelBlendFunction(Channel channel, const texture::Texture* blend_texture);
		inline math::AlphaValue evaluate(
			const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
		inline bool depends_on_texture_coordinates(void) const override;
	};

	ImageChannelBlendFunction::ImageChannelBlendFunction(Channel channel, const texture::Texture* blend_texture)
		: channel(channel)
		, blend_texture(blend_texture)
	{}

	math::AlphaValue ImageChannelBlendFunction::evaluate(
		const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
	{
		auto [color, alpha] = blend_texture->channels(texture_coordinate, position);
		math::AlphaValue alpha_value;
		switch (channel)
		{
		case Channel::Red: alpha_value = color.r; break;
		case Channel::Green: alpha_value = color.g; break;
		case Channel::Blue: alpha_value = color.b; break;
		default:
		case Channel::Alpha: alpha_value = alpha; break;

		}
		return alpha_value;
	}

	bool ImageChannelBlendFunction::depends_on_texture_coordinates(void) const
	{
		return true;
	}
}

#endif