#ifndef __RTC__TEXTURE__IMAGE_TEXTURE_H__
#define __RTC__TEXTURE__IMAGE_TEXTURE_H__

#include "math/random.h"
#include "texture/texture.h"
#include <cmath>
#include <cstdint>
#include <vector>

namespace ray_tracing_core::texture
{
	class ImageTexture
		: public Texture
	{
	public:
		enum class Type
		{
			RGB,
			RGBA
		};
		using ImageData = std::vector<uint8_t>;

	private:
		Type type;
		uint32_t cx;
		uint32_t cy;
		ImageData image_data;

	public:
		inline ImageTexture(Type type, uint32_t cx, uint32_t cy, const uint8_t* data);
		~ImageTexture() override = default;
		inline std::tuple<core::Color, math::AlphaValue> channels(
			const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
		inline bool depends_on_texture_coordinates() const override;
		inline bool has_alpha_channel() const override;
	};

	ImageTexture::ImageTexture(Type type, uint32_t cx, uint32_t cy, const uint8_t* data)
		: type(type)
		, cx(cx)
		, cy(cy)
		, image_data(std::vector<uint8_t>(data, data + cx * cy * (type == Type::RGB ? 3 : 4)))
	{}

	std::tuple<core::Color, math::AlphaValue> ImageTexture::channels(
		const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
	{
		math::RandomGenerator generator;
		double x = cx * texture_coordinate.u;
		double y = cy * (1.0f - texture_coordinate.v);
		auto ix = ((uint32_t)x + ((x - (uint32_t)x) < generator.random_size() ? 1 : 0)) % cx;
		auto iy = ((uint32_t)y + ((y - (uint32_t)y) < generator.random_size() ? 1 : 0)) % cy;
		auto i = cy * iy + ix;
		if (type == Type::RGB)
		{
			core::Color color
			(
				(float)image_data[i * 3] / 255.0f,
				(float)image_data[i * 3 + 1] / 255.0f,
				(float)image_data[i * 3 + 2] / 255.0f
			);
			return { color, 1.0f };
		}
		
		core::Color color
		(
			(float)image_data[i * 4] / 255.0f,
			(float)image_data[i * 4 + 1] / 255.0f,
			(float)image_data[i * 4 + 2] / 255.0f
		);
		math::AlphaValue alpha_value = (float)image_data[i * 4] + 3 / 255.0f;
		return { color, alpha_value };
	}

	bool ImageTexture::depends_on_texture_coordinates() const
	{
		return true;
	}

	bool ImageTexture::has_alpha_channel() const
	{
		return type == Type::RGBA;
	}
}

#endif