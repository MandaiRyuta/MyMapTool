#include "Noise.h"

void Image::set(std::int32_t x, std::int32_t y, const sRGB & color)
{
	if (!inBounds(y, x))
	{
		return;
	}

	m_data[y * m_width + x] = color;
}

int Image::width() const
{
	return m_width;
}

int Image::height() const
{
	return m_height;
}

bool Image::saveBMP(const std::string & path)
{
	const std::int32_t  rowSize = m_width * 3 + m_width % 4;
	const std::uint32_t bmpsize = rowSize * m_height;
	const BMPHeader header =
	{
		0x4d42,
		static_cast<std::uint32_t>(bmpsize + sizeof(BMPHeader)),
		0,
		0,
		sizeof(BMPHeader),
		40,
		m_width,
		m_height,
		1,
		24,
		0,
		bmpsize,
		0,
		0,
		0,
		0
	};

	if (std::ofstream ofs{ path, std::ios_base::binary })
	{
		ofs.write((const char*)&header, sizeof(header));

		std::vector<std::uint8_t> line(rowSize);

		for (std::int32_t y = m_height - 1; -1 < y; --y)
		{
			size_t pos = 0;

			for (std::int32_t x = 0; x < m_width; ++x)
			{
				const sRGB& col = m_data[y * m_width + x];
				line[pos++] = ToUint8(col.b);
				line[pos++] = ToUint8(col.g);
				line[pos++] = ToUint8(col.r);
			}

			ofs.write((const char*)line.data(), line.size());
		}

		return true;
	}
	else
	{
		return false;
	}
}

float siv::PerlinNoise::Fade(float t) noexcept
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float siv::PerlinNoise::Lerp(float t, float a, float b) noexcept
{
	return a + t * (b - a);
}

float siv::PerlinNoise::Grad(std::int32_t hash, float x, float y, float z) noexcept
{
	const std::int32_t h = hash & 15;
	const float u = h < 8 ? x : y;
	const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

siv::PerlinNoise::PerlinNoise(std::uint32_t seed)
{
	reseed(seed);
}

void siv::PerlinNoise::reseed(std::uint32_t seed)
{
	for (size_t i = 0; i < 256; ++i)
	{
		p[i] = i;
	}

	std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

	for (size_t i = 0; i < 256; ++i)
	{
		p[256 + i] = p[i];
	}
}

float siv::PerlinNoise::noise(float x) const
{
	return noise(x, 0.0, 0.0);
}

float siv::PerlinNoise::noise(float x, float y) const
{
	return noise(x, y, 0.0);
}

float siv::PerlinNoise::noise(float x, float y, float z) const
{
	const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
	const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
	const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);
	const float u = Fade(x);
	const float v = Fade(y);
	const float w = Fade(z);

	const std::int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
	const std::int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

	return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
		Grad(p[BA], x - 1, y, z)),
		Lerp(u, Grad(p[AB], x, y - 1, z),
			Grad(p[BB], x - 1, y - 1, z))),
		Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
			Grad(p[BA + 1], x - 1, y, z - 1)),
			Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
				Grad(p[BB + 1], x - 1, y - 1, z - 1))));
}
float siv::PerlinNoise::octaveNoise(float x, std::int32_t octaves) const
{
	float result = 0.0;
	float amp = 1.0;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += noise(x) * amp;
		x *= 2.0;
		amp *= 0.5;
	}

	return result;
}
float siv::PerlinNoise::octaveNoise(float x, float y, std::int32_t octaves) const
{
	float result = 0.0;
	float amp = 1.0;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += noise(x, y) * amp;
		x *= 2.0;
		y *= 2.0;
		amp *= 0.5;
	}

	return result;
}

float siv::PerlinNoise::octaveNoise(float x, float y, float z, std::int32_t octaves) const
{
	float result = 0.0;
	float amp = 1.0;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += noise(x, y, z) * amp;
		x *= 2.0;
		y *= 2.0;
		z *= 2.0;
		amp *= 0.5;
	}

	return result;
}

float siv::PerlinNoise::noise0_1(float x) const
{
	return noise(x) * 0.5f + 0.5f;
}

float siv::PerlinNoise::noise0_1(float x, float y) const
{
	return noise(x, y) * 0.5f + 0.5f;
}

float siv::PerlinNoise::noise0_1(float x, float y, float z) const
{
	return noise(x, y, z) * 0.5f + 0.5f;
}

float siv::PerlinNoise::octaveNoise0_1(float x, std::int32_t octaves) const
{
	return octaveNoise(x, octaves) * 0.5f + 0.5f;
}

float siv::PerlinNoise::octaveNoise0_1(float x, float y, std::int32_t octaves) const
{
	return octaveNoise(x, y, octaves) * 0.5f + 0.5f;
}

float siv::PerlinNoise::octaveNoise0_1(float x, float y, float z, std::int32_t octaves) const
{
	return octaveNoise(x, y, z, octaves) * 0.5f + 0.5f;
}
