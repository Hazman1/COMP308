#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "imageLoader.hpp"
#include "comp308.hpp"
#include "texture.hpp"
#include "imageLoader.hpp"

Texture::Texture(std::string &s){
	Image = new image(s);
	generateGradiant();
	makeHeatmap();
}

void Texture::makeHeatmap(){
	for(unsigned int i=0; i<sizeof(heatMap); i++){
		for(unsigned int j=0; j<sizeof(heatMap[0]); j++){
			float x= randomValue(i*j+i);
			float y= randomValue(i*j+j);
			heatMap[i][j] = noiseMap(x, y);
		}
	}
}

void Texture::generateGradiant(){
	for(unsigned int i=0; i<sizeof(Texture::gradient); i++){
		for(unsigned int j=0; j<sizeof(Texture::gradient[0]); j++){
			gradient[i][j] = randomValue(i*j+i+j);
		}
	}
}

float Texture::randomValue(int val){
	int x = (val<<13) ^ val;
	x = 1.0-((x*(x*x*15731 + 789221) + 1376312589) & 2147483647);
	float z = x / 1073741824.0;
	return z;
}

float Texture::lerp(float t, float a, float b){
	return (1.0 - t)*a + t*b;
}

float Texture::dotGradient(int xi, int yi, float xf, float yf){
	float f = Texture::gradient[xi][yi];

	float dx = xf - (double)xi;
	float dy = yf - (double)yi;
	return dx*f + dy*f;
}

float Texture::noiseMap(float x, float y){
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

image* Texture::getImage(){
	return Texture::Image;
}