#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "comp308.hpp"
#include "splines.hpp"

using namespace std;
using namespace comp308;


vec4 Splines::phantomSpine(vec4 one, vec4 two)
{
	return endOfLine(one, two);
}

comp308::vec4 Splines::endOfLine(comp308::vec4 A, comp308::vec4 B)
{
	float x = (2 * B.x) - A.x;
	float y = (2 * B.y) - A.y;
	float z = (2 * B.z) - A.z;
	return comp308::vec4(x, y, z,0);
}

comp308::vec4 Splines::compPoint(float u, vec4 zero, vec4 one, vec4 two, vec4 three)
{
	float u3 = u*u*u;
	float u2 = u*u;

	return (0.5*vec4(u3, u2, u, 1.0f)*M)*mat4(zero.x, one.x, two.x, three.x, 
		zero.y, one.y, two.y, three.y, 
		zero.z, one.z, two.z, three.z,
		0, 0, 0, 0);
}

void Splines::readySplines()
{

}


void Splines::setUStep(int temp)
{
	UStep = 1 / temp;

}

void Splines::addPoint(comp308::vec3 add)
{
	cout << add << "\n";
	sPoints.push_back(vec4(add.x, add.y, add.z,0));
	if (sPoints.size() == 2) {
		vector<vec4> temp;
		vec4 one = vec4(sPoints.at(1).x, sPoints.at(1).y, sPoints.at(1).z, 0);
		vec4 zero = vec4(sPoints.at(0).x, sPoints.at(0).y, sPoints.at(0).z, 0);

		temp.push_back(endOfLine(one,zero));
		temp.push_back(sPoints.at(0));
		temp.push_back(sPoints.at(1));
		sPoints = temp;
	}
}

void Splines::renderSpline()
{
	if (sPoints.empty()) return;
	glPushMatrix();
	int i;
	glColor3f(0, 0, 1);
	for (i = 0; i < sPoints.size(); i++) {
		vec4 s = sPoints.at(i);
		if (i == 0) {
			glColor3f(0, 1, 1);
		}else{
			glColor3f(1, 0, 0);
		}
	
		glPushMatrix();
		glTranslatef(s.x, s.y, s.z);
		glutSolidSphere(0.2, 100, 100);
		
		glPopMatrix();
	}
	if (sPoints.size() > 2) {
		glPushMatrix();
		glColor3f(0, 1, 1);
		vec4 s = endOfLine(sPoints.at(sPoints.size() - 2), sPoints.at(sPoints.size()-1));
		glTranslatef(s.x, s.y, s.z);
		glutSolidSphere(0.2, 100, 100);
		glPopMatrix();
	}

	glPopMatrix();
	if (sPoints.size() >= 3) {
		
		glPushMatrix();
		glBegin(GL_LINE_STRIP);
		
		glColor3f(0, 1, 0);
		for (i = 0; i< sPoints.size() - 2; i++) {

			float locU = 0.0;
			vec4 temp;
			vec4 zero, one, two, three;

			if (i == 0  || i == sPoints.size() - 3) {
				glColor3f(0, 1, 1);
			}
			else {
				glColor3f(1, 0, 0);
			}

			zero = vec4(sPoints.at(i).x, sPoints.at(i).y, sPoints.at(i).z,0);
			one = vec4(sPoints.at(i+1).x, sPoints.at(i+1).y, sPoints.at(i+1).z, 0);
			two = vec4(sPoints.at(i + 2).x, sPoints.at(i + 2).y, sPoints.at(i + 2).z, 0);
			if (i == sPoints.size() - 3) {
				three = endOfLine(one,two);

			}
			else {
			three = vec4(sPoints.at(i + 3).x, sPoints.at(i + 3).y, sPoints.at(i + 3).z, 0);
		}
			for (locU = 0.00f; locU <= 1.05f; locU = locU + UStep) {

				temp = compPoint(locU, zero, one, two, three);
				glVertex3f(temp.x, temp.y, temp.z);

			}
			
		}
		glEnd();
		glPopMatrix();
	}
}


void Splines::clearSpline()
{
	sPoints.clear();

}

std::vector<comp308::vec3> Splines::getNavPoints()
{
	
		int i = 0;
		vector<vec3> list;
		if (!sPoints.empty()) {
		for (i = 0; i < sPoints.size() - 2; i++) {

			float locU = 0.0;
			vec4 temp;
			vec4 zero, one, two, three;

			zero = vec4(sPoints.at(i).x, sPoints.at(i).y, sPoints.at(i).z, 0);
			one = vec4(sPoints.at(i + 1).x, sPoints.at(i + 1).y, sPoints.at(i + 1).z, 0);
			two = vec4(sPoints.at(i + 2).x, sPoints.at(i + 2).y, sPoints.at(i + 2).z, 0);
			if (i == sPoints.size() - 3) {
				three = endOfLine(one, two);
			}
			else {
				three = vec4(sPoints.at(i + 3).x, sPoints.at(i + 3).y, sPoints.at(i + 3).z, 0);
			}
			for (locU = 0.00f; locU <= 1.05f; locU = locU + UStep) {
				temp = compPoint(locU, zero, one, two, three);
				list.push_back(vec3(temp.x, temp.y, temp.z));
			}

		}
	}
		return list;


}

bool Splines::ready()
{
	return sPoints.size()>=3;
}

bool Splines::empty()
{
	return sPoints.empty();
}


