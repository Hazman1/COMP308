#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "imageLoader.hpp"
#include "comp308.hpp"
#include "texture.hpp"
#include "imageLoader.hpp"

Texture::Texture(){
	image = new image("word/res/texture/wood.jpg");


}

float Texture::lerp(float t, float a, float b){
	return (1.0 - t)*a + t*b;
}

float dotGradient(int xi, int yi, float xf, float yf){
	float f = gradient[x][y];

	float dx = x - (double)xi;
	float dy = y - (double)iy;
}

float noiseMap(float x, float y){
	int x0 = (x > 0.0 ? (int)x : (int)x-1);
	int x1 = x0+1;
	int y0 = (y > 0.0 ? (int)y : (int)y-1);
	int y1 = y0+1;

	float sx = x - (double)x0;
	float sy = y - (double)y0;

	float n0, n1, ix0, ix1, value;

	n0 = dotGradient(x0, y0, x, y);
	n1 = dotGradient(x1, y0, x, y);
	ix0 = lerp(sx, n0, n1);
	n0 = dotGradient(x0, y1, x, y);
	n1 = dotGradient(x1, y1, x, y);
	ix1 = lerp(sx, n0, n1);
	return lerp(sy, ix1, ix0);
}

image getImage(){
	return Texture::image;
}