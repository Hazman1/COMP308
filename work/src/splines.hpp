#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "comp308.hpp"


class Splines {
private:
	float UStep = 0.1;
	comp308::mat4 M=comp308::mat4(-1, 2, -1, 0, 3, -5, 0, 2, -3, 4, 1, 0, 1, -1, 0, 0);
	std::vector<comp308::vec4> sPoints;

	comp308::vec4 phantomSpine(comp308::vec4, comp308::vec4);
	comp308::vec4 endOfLine(comp308::vec4 A, comp308::vec4 B);
	comp308::vec4 compPoint(float, comp308::vec4, comp308::vec4, comp308::vec4, comp308::vec4);
	void readySplines();

public:
	void setUStep(int);
	void addPoint(comp308::vec3);
	void renderSpline();
	void clearSpline();
	std::vector<comp308::vec3> getNavPoints();
	bool ready();
	bool empty();
	std::vector<comp308::vec3> getNavPointsWithSpeed(std::vector<comp308::vec3>);
};