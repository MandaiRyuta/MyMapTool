# pragma once
# include <cstdint>
# include <numeric>
# include <algorithm>
# include <random>

namespace siv
{
	class PerlinNoise
	{
	private:

		std::int32_t p[512];

		static float Fade(float t) noexcept;

		static float Lerp(float t, float a, float b) noexcept;

		static float Grad(std::int32_t hash, float x, float y, float z) noexcept;

	public:

		explicit PerlinNoise(std::uint32_t seed = std::default_random_engine::default_seed);

		void reseed(std::uint32_t seed);

		float noise(float x) const;

		float noise(float x, float y) const;

		float noise(float x, float y, float z) const;

		float octaveNoise(float x, std::int32_t octaves) const;

		float octaveNoise(float x, float y, std::int32_t octaves) const;

		float octaveNoise(float x, float y, float z, std::int32_t octaves) const;

		float noise0_1(float x) const;

		float noise0_1(float x, float y) const;

		float noise0_1(float x, float y, float z) const;

		float octaveNoise0_1(float x, std::int32_t octaves) const;

		float octaveNoise0_1(float x, float y, std::int32_t octaves) const;

		float octaveNoise0_1(float x, float y, float z, std::int32_t octaves) const;
	};
}

# include <iostream>
# include <fstream>
# include <sstream>

# pragma pack (push, 1)

struct BMPHeader
{
	std::uint16_t bfType;
	std::uint32_t bfSize;
	std::uint16_t bfReserved1;
	std::uint16_t bfReserved2;
	std::uint32_t bfOffBits;
	std::uint32_t biSize;
	std::int32_t  biWidth;
	std::int32_t  biHeight;
	std::uint16_t biPlanes;
	std::uint16_t biBitCount;
	std::uint32_t biCompression;
	std::uint32_t biSizeImage;
	std::int32_t  biXPelsPerMeter;
	std::int32_t  biYPelsPerMeter;
	std::uint32_t biClrUsed;
	std::uint32_t biClrImportant;
};

struct sRGB
{
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
	constexpr sRGB() = default;
	constexpr sRGB(float _rgb)
		: r(_rgb), g(_rgb), b(_rgb) {}
	constexpr sRGB(float _r, float _g, float _b)
		: r(_r), g(_g), b(_b) {}
};

static_assert(sizeof(BMPHeader) == 54, "sizeof(BMPHeader) != 54");
# pragma pack ( pop )

class Image
{
private:

	std::vector<sRGB> m_data;

	std::int32_t m_width = 0, m_height = 0;

	bool inBounds(std::int32_t y, std::int32_t x) const
	{
		return (0 <= y) && (y < m_height) && (0 <= x) && (x < m_width);
	}

	static constexpr std::uint8_t ToUint8(float x)
	{
		return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
	}

public:

	Image() = default;

	Image(std::int32_t width, std::int32_t height)
		: m_data(width * height)
		, m_width(width)
		, m_height(height) {}

	void set(std::int32_t x, std::int32_t y, const sRGB& color);

	int width() const;

	int height() const;

	bool saveBMP(const std::string& path);
};

template <class Type>
inline constexpr const Type& Clamp(const Type& x, const Type& min, const Type& max)
{
	return (x < min) ? min : ((max < x) ? max : x);
}
