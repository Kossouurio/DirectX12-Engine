#pragma once


#define minimum(a, b) (a < b ? a : b)
#define maximum(a, b) (a > b ? a : b)
#define clamp(v, a, b) (v < a ? a : (v > b ? b : a))
#define clamp01(v) clamp(v, 0, 1)
#define lerp(a, b, t) (a) * (1 - t) + (b) * (t)

class ToolBox
{
public:
	static int StrLength(const char* _str);
	static float Pow(float _x, int _exp);
	//static const sf::Image* LoadImageFromFile(const char* filePath, sf::Image& image);
	//static const sf::Texture* LoadTextureFromFile(const char* filePath, sf::Texture& texture);
	static float RandomFloat(float _min, float _max);
};

