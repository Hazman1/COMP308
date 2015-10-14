#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "imageLoader.hpp"
#include "comp308.hpp"

class Texture{
private:
	image *image;
	void generateHeightmap();
	void generateTexture();
	double lerp(float, float, float);
	void setup(int, float, float, float, float);


public:
	Texture();
	image getImage();
};