#include "pch.h"
#include "ToolBox.h"

int ToolBox::StrLength(const char* _str)
{
	int count = 0;
	while (_str[count] != '\0') {
		count++;
	}
	return count;
}

float ToolBox::Pow(float _x, int _exp)
{
	if (_exp == 1)
		return _x;
	if (_exp == 0)
		return 1;
	
	float halfPow = Pow(_x, _exp / 2);
	if (_exp % 2 == 1)
		return halfPow * halfPow * _x;
	return halfPow * halfPow;
}

float ToolBox::RandomFloat(float _min, float _max)
{
	return _min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (_max - _min)));
}

//const sf::Image* ToolBox::LoadImageFromFile(const char* filePath, sf::Image& image)
//{
//	if (!image.loadFromFile(filePath)) {
//		HEAD("WARNING") << "LOADING IMAGE FAILED FOR: " << filePath << ". DEFAULT COLOR WILL BE USED\n";
//		return nullptr;
//	}
//	return (image.getvSize() != sf::Vector2u(0, 0)) ? &image : nullptr;
//}
