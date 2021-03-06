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
float factor = 0.9f;

Texture::Texture(std::string s){
	//Image = new image(s);
	generateGradiant();
	makeHeatmap();
	uint8_t *dMap;
	uint8_t *gMap;
	dMap = (uint8_t*) calloc(width*height*3, sizeof(uint8_t));
	gMap = (uint8_t*) calloc(width*height*3, sizeof(uint8_t));
	unsigned int x = 0;
	int iters = 1024;
	for(int l=1; l<5; l++){
		for(int i=0; i<width; i=i+l){
			for(int j=0; j<height; j++){
				heatMap[i][j] = smoothNoise(iters, gradient[i][j], gradient[i][j], 0.5, 0.007, 0, 255);
			}
		}
		iters/2;
	}

	for(int i=0; i<width;i++){
		for(int j=0; j<height;j++){
			dMap[x * 3 + 0] = (uint8_t) ((int)(heatMap[i][j] * 255) >> 14);
			dMap[x * 3 + 1] = (uint8_t) ((int)(heatMap[i+1][j+1] * 255) >> 14);
			dMap[x * 3 + 2] = (uint8_t) ((int)(heatMap[i+2][j+2] * 255) >> 14);
			x++;
		}
	}
	x=0;
	for(int i=0; i<width;i++){
		for(int j=0; j<height;j++){
			gMap[x * 3 + 0] = (uint8_t) gradient[i][j] *255;
			gMap[x * 3 + 1] = (uint8_t) gradient[i+1][j+1]*255;
			gMap[x * 3 + 2] = (uint8_t) gradient[i+2][j+2]*255;
			x++;
		}
	}
#ifdef _WIN32
	FILE *fout = fopen("./res/textures/output.png", "wb");
#else
	FILE *fout = fopen("work/res/textures/output.png","wb");
#endif
	struct TinyPngOut pngout;
	if (fout == NULL || TinyPngOut_init(&pngout, fout, width, height) != TINYPNGOUT_OK)
		cout << "error init" << endl;
	
	// Write image data
	if (TinyPngOut_write(&pngout, dMap, width*height) != TINYPNGOUT_OK)
		cout << "error write" << endl;
	
	// Check for proper completion
	if (TinyPngOut_write(&pngout, NULL, 0) != TINYPNGOUT_DONE)
		cout << "error complete" << endl;
	else{
		cout << "Texture generated\n";
	}
	fclose(fout);
#ifdef _WIN32
	fout = fopen("./res/textures/grad.png", "wb");
#else
	fout = fopen("work/res/textures/grad.png", "wb");
#endif

	struct TinyPngOut pngout2;
	if (fout == NULL || TinyPngOut_init(&pngout2, fout, width, height) != TINYPNGOUT_OK)
		cout << "error init" << endl;
	
	// Write image data
	if (TinyPngOut_write(&pngout2, gMap, width*height) != TINYPNGOUT_OK)
		cout << "error write" << endl;
	
	// Check for proper completion
	if (TinyPngOut_write(&pngout2, NULL, 0) != TINYPNGOUT_DONE)
		cout << "error complete" << endl;
	else{
		cout << "Texture generated\n";
	}
	fclose(fout);
}

float Texture::smoothNoise(int iters, float x, float y, float pers, float scale, int low, int high){
	int maxAmp = 0;
	int amp = 1;
	float freq = scale;
	float noise = 1;

	for(int i=0; i<iters;i++){
		noise += noise2d(x*freq+1, y*freq+1);
		maxAmp += amp;
		amp *= pers;
		freq *= 2;
	}

	noise /= maxAmp;

	noise = noise * (high - low) / 2 + (high + low) / 2;
	
	return noise;
}	

void Texture::makeHeatmap(){
	for(unsigned int i=0; i<width*3-1; i++){
		for(unsigned int j=0; j<height*3-1; j++){
			heatMap[i][j] = abs(noiseMap(i, j));
		}
	}
}

void Texture::setGrad(int i, int j){
	float z, x, y;
	if(gradient[i-1][j-1] == 0){
		gradient[i-1][j-1] = factor * gradient[i][j];
	}
	if(gradient[i][j-1] == 0){
		gradient[i][j-1] = factor * gradient[i][j];
	}
	if(gradient[i+1][j-1] == 0){
		gradient[i+1][j-1] = factor * gradient[i][j];
	}
	if(gradient[i-1][j] == 0){
		gradient[i-1][j] = factor * gradient[i][j];
	}
	if(gradient[i+1][j] == 0){
		gradient[i+1][j] = factor * gradient[i][j];	
	}
	if(gradient[i-1][j+1] == 0){
		gradient[i-1][j+1] = factor * gradient[i][j];
	}
	if(gradient[i][j+1] == 0){
		gradient[i][j+1] = factor * gradient[i][j];
	}
	if(gradient[i+1][j] == 0){
		gradient[i+1][j] = factor * gradient[i][j];
	}
}

void Texture::generateOnes(){
	for(unsigned int i=1; i<height-1; i++){
		int x = ((int) (rand() % width-1)) + 1;
		int y = ((int) (rand() % height-1)) + 1;
		gradient[x][y] = 1.0;
	}
}

bool Texture::noZeros(){
	for(unsigned int i=1; i<width-1; i++){
		for(unsigned int j=1; j<height-1; j++){
			if(gradient[i][j] == 0){
				return true;
			}
		}
	}
	return false;
}

void Texture::generateGradiant(){
	for(unsigned int i=0; i<width; i++){
		for(unsigned int j=0; j<height; j++){
			gradient[i][j] = 0;
		}
	}
	generateOnes();
	while(noZeros()){
		if(factor <= 0){
			factor = 0.9;
		}
		for(unsigned int i=1; i<width-1; i++){
			for(unsigned int j=1; j<height-1; j++){
				if(gradient[i][j] != 0){
					setGrad(i, j);
				}
			}		
		}
		factor -= 0.01;
	}
}

float Texture::randomValue(int val){
	val = val*13;
	int x = (val<<13) ^ val;
	x = 1.0-((x*(x*x*15731 + 789221) + 1376312589) & 2147483647);
	float z = x / 1073741824.0;
	return z;
}

float Texture::noise2d(int x, int y){
	int z = x+y*53;
	z = (z<<13) ^ z;
	z = 1.0-((z*(z*z*15731 + 789221) + 1376312589) & 2147483647);
	float w = z / 1073741824.0;
	return w;

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
	float zr =0, zi=0;
	int i=0;
	for(;i<width;i++){
		if(zr*zr + zi*zi > 4){
			break;
		}
		float temp = zr *zr - zi*zi +x;
		zi = 2*zr*zi+y;
		zr = temp;
	}

	float j = (double)i / width;
	return (int)(pow(j, 0.6) * 255) << 14 | (int)(pow(j, 0.3) * 255) << 14 | (int)(pow(j, 0.1) *255) << 14;
}

image* Texture::getImage(){
	return Texture::Image;
}