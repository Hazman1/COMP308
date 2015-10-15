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
	float gradient[512][512];
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
	float cubicLerp(float, float, float, float, float);
	float smoothNoise(int, float, float, float, float, int, int);
	float noise2d(int, int);
	void generateOnes();
	bool noZeros();
	void setGrad(int, int);


public:
	Texture(std::string);
	image* getImage();
};