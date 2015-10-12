//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "imageLoader.hpp"
#include "comp308.hpp"


struct vertex {
	int p = 0; // index for point in m_points
	int t = 0; // index for uv in m_uvs
	int n = 0; // index for normal in m_normals
};

struct triangle {
	vertex v[3]; //requires 3 verticies
};

class Geometry {
private:
	image *texture = nullptr;
	comp308::vec3 Scale;
	comp308::vec4 Rotation;
	comp308::vec3 Translation;

	GLuint g_shader = 0;
	float mat_ambient[3];
	float mat_diffuse[3];
	float mat_specular[3];
	float shine = 1.0f;

	GLuint g_texture;
	// Feilds for storing raw obj information
	std::string m_filename;
	std::vector<comp308::vec3> m_points;	// Point list
	std::vector<comp308::vec2> m_uvs;		// Texture Coordinate list
	std::vector<comp308::vec3> m_normals;	// Normal list
	std::vector<triangle> m_triangles;		// Triangle/Face list

	bool m_wireFrameOn = false;

	// IDs for the display list to render
	GLuint m_displayListPoly = 0; // DisplayList for Polygon
	GLuint m_displayListWire = 0; // DisplayList for Wireframe

	void readOBJ(std::string);
	void initShader();
	void createNormals();
	void setTexture();
	void createDisplayListPoly();
	void createDisplayListWire();

public:
	Geometry(std::string);
	// ~GeometryManager();
	void changeScale(comp308::vec3 s);
	void renderGeometry(bool shade);
	void toggleWireFrame();
	void loadTexture(std::string s);
	void rotate(comp308::vec4 r);
	void translate(comp308::vec3 t);
	void setAmbient(comp308::vec3 a);
	void setDiffuse(comp308::vec3 d);
	void setSpecular(comp308::vec3 s);
	void setShine(float s);
};
