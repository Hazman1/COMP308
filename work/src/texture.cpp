#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#include "imageLoader.hpp"
#include "comp308.hpp"
#include "texture.hpp"
#include "imageLoader.hpp"

using namespace std;
using namespace comp308;

int width = 256, height =256;
png_bytep *row_pointers;

Texture::Texture(std::string s){
	//Image = new image(s);
	generateGradiant();
	makeHeatmap();
	uint8_t *dMap;
	dMap = (uint8_t*) calloc((width*height)*512, sizeof(uint8_t));
	unsigned int x = 0;
	for(int i=0; i<width;i++){
		for(int j=0; j<height;j++){
			cout <<"yes\n";
			dMap[x] = (uint8_t) (heatMap[i][j] * 256);
			x++;
		}
	}
	printf("x %d\n", x);
	 FILE *fout = fopen("work/res/textures/output.png","wb");
	 struct TinyPngOut pngout;
	 if (fout == NULL || TinyPngOut_init(&pngout, fout, width, height) != TINYPNGOUT_OK)
		cout << "Oh fuck" << endl;
	
	// Write image data
	if (TinyPngOut_write(&pngout, dMap, width * height) != TINYPNGOUT_OK)
		cout << "Oh fuck" << endl;
	
	// Check for proper completion
	if (TinyPngOut_write(&pngout, NULL, 0) != TINYPNGOUT_DONE)
		cout << "Oh fuck no" << endl;
	else{
		cout << "Oh fuck yes\n";
	}
	fclose(fout);
}

void Texture::makeHeatmap(){
	for(unsigned int i=0; i<width; i++){
		for(unsigned int j=0; j<height; j++){
			float x= randomValue(i*j+i);
			float y= randomValue(i*j+j);
			heatMap[i][j] = abs(noiseMap(x, y));
		}
	}
}

void Texture::generateGradiant(){
	cout << "generate\n";
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
	return (1.0 - t)*a + t*b;
}

float Texture::dotGradient(int xi, int yi, float xf, float yf){
	float f = Texture::gradient[xi][yi];

	float dx = xf - (double)xi;
	float dy = yf - (double)yi;
	return dx*f + dy*f;
}

float Texture::noiseMap(float x, float y){
	cout << "Noise\n";
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
	//cout << "work?\n";
	return lerp(sy, ix1, ix0);
}

image* Texture::getImage(){
	return Texture::Image;
}

void setRGB(png_byte *ptr, float val)
{
	int v = (int)(val * 767);
	if (v < 0) v = 0;
	if (v > 767) v = 767;
	int offset = v % 256;

	if (v<256) {
		ptr[0] = 0; ptr[1] = 0; ptr[2] = offset;
	}
	else if (v<512) {
		ptr[0] = 0; ptr[1] = offset; ptr[2] = 255-offset;
	}
	else {
		ptr[0] = offset; ptr[1] = 255-offset; ptr[2] = 0;
	}
}