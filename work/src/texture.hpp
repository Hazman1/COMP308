#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "imageLoader.hpp"
#include "comp308.hpp"

class Texture{
private:
	image* Image;
	float gradient[256][256];
	void generateHeightmap();
	void generateTexture();
	void generateGradiant();
	float lerp(float, float, float);
	void setup(int, float, float, float, float);
	float randomValue(int);
	float dotGradient(int, int, float, float);
	float noiseMap(float, float);


public:
	Texture(std::string&);
	image* getImage();
};