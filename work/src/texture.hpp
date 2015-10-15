#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "imageLoader.hpp"
#include "TinyPngOut.hpp"
#include "comp308.hpp"

using namespace std;

class Texture{
private:
	image* Image;
	float gradient[256][256];
	float heatMap[256*3][256*3];
	void generateHeightmap();
	void generateTexture();
	void generateGradiant();
	float lerp(float, float, float);
	void setup(int, float, float, float, float);
	float randomValue(int);
	float dotGradient(int, int, float, float);
	float noiseMap(float, float);
	void makeHeatmap();
	void writePNGFile(char*, int, int, float*, char*);
	float cubicLerp(float, int, int, int, int);
	float smoothNoise(int, int);


public:
	Texture(std::string);
	image* getImage();
};