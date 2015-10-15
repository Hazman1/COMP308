#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "imageLoader.hpp"
#include "comp308.hpp"
#include "texture.hpp"
#include "imageLoader.hpp"

using namespace std;
using namespace comp308;

int width = 256, height =256;

Texture::Texture(std::string s){
	//Image = new image(s);
	generateGradiant();
	makeHeatmap();
	uint8_t *dMap;
	dMap = (uint8_t*) calloc((width*height)*512, sizeof(uint8_t));
	unsigned int x = 0;
	for(int i=0; i<width;i++){
		for(int j=0; j<height;j++){
			heatMap[i][j] = smoothNoise(i,j);
			dMap[x * 3 + 0] = (uint8_t) (heatMap[i][j] * 100);
			dMap[x * 3 + 1] = (uint8_t) (heatMap[i+1][j+1] * 100);
			dMap[x * 3 + 2] = (uint8_t) (heatMap[i+2][j+2] * 100);
			x++;
		}
	}

	FILE *fout = fopen("work/res/textures/output.png","wb");
	struct TinyPngOut pngout;
	if (fout == NULL || TinyPngOut_init(&pngout, fout, width, height) != TINYPNGOUT_OK)
		cout << "error" << endl;
	
	// Write image data
	if (TinyPngOut_write(&pngout, dMap, width*height) != TINYPNGOUT_OK)
		cout << "error" << endl;
	
	// Check for proper completion
	if (TinyPngOut_write(&pngout, NULL, 0) != TINYPNGOUT_DONE)
		cout << "error" << endl;
	else{
		cout << "Texture generated\n";
	}
	fclose(fout);
}

float Texture::smoothNoise(int x, int y){
	float corner = heatMap[x-1][y-1] + heatMap[x+1][y-1] +  heatMap[x-1][y+1] + heatMap[x+1][y+1] / 16.0;
	float side = heatMap[x-1][y] + heatMap[x+1][y] + heatMap[x][y-1] + heatMap[x][y+1] / 8.0;
	float centre = heatMap[x][y] / 4.0 ;
	return corner + side+ centre;
}	

void Texture::makeHeatmap(){
	for(unsigned int i=0; i<width*2; i++){
		for(unsigned int j=0; j<height*2; j++){
			heatMap[i][j] = abs(noiseMap(i, j));
		}
	}
}

void Texture::generateGradiant(){
	for(unsigned int i=0; i<width; i++){
		for(unsigned int j=0; j<height; j++){
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
	float f = t * pi();
	f = (1 - cos(f)) *0.5;
	return a*(1-f) + b*f;
}

float Texture::cubicLerp(float t, float v0, float v1, float v2, float v3){
	float p = (v3 - v2) - (v0 -v1);
	float q = (v0 - v1) - p;
	float r = v2 - v0;
	float s = v1;

	return (p*pow(t,3) + q*pow(t, 2) + r*t + s);
}

float Texture::dotGradient(int xi, int yi, float xf, float yf){
	float f = Texture::gradient[xi][yi];

	float dx = xf - (double)xi;
	float dy = yf - (double)yi;
	return dx*f + dy*f;
}

float Texture::noiseMap(float x, float y){
	int x0 = (x >= 0.0 ? (int)x : (int)x-1);
	int x1 = x0+1;
	int y0 = (y >= 0.0 ? (int)y : (int)y-1);
	int y1 = y0+1;

	float sx = x - (double)x0;
	float sy = y - (double)y0;

	float n0, n1, n2, n3, ix0, ix1, value;

	n0 = dotGradient(x0, y0, x, y);
	n1 = dotGradient(x1, y0, x, y);
	n2 = dotGradient(x0, y1, x, y);
	n3 = dotGradient(x1, y1, x, y);
	ix0 = cubicLerp(sx, n0, n1,n2,n3);
	n0 = dotGradient(x1, y1+1, x, y);
	n1 = dotGradient(x1+1, y1+1, x, y);
	n2 = dotGradient(x1, y1, x, y);
	n3 = dotGradient(x1+1, y1, x, y);
	ix1 = cubicLerp(sx, n0, n1,n2,n3);
	return lerp(sy, ix1, ix0);
}

image* Texture::getImage(){
	return Texture::Image;
}