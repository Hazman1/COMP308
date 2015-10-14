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

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>
#include <map>

#include "shaderLoader.hpp"
#include "imageLoader.hpp"
#include "comp308.hpp"
#include "geometry.hpp"

using namespace std;
using namespace comp308;




Geometry::Geometry(string filename)
{
	Scale = vec3(1.0, 1.0, 1.0);
	mat_ambient[0] = 1;
	mat_ambient[1] = 1;
	mat_ambient[2] = 1;

	mat_diffuse[0] = 1;
	mat_diffuse[1] = 1;
	mat_diffuse[2] = 1;

	mat_specular[0] = 1;
	mat_specular[1] = 1;
	mat_specular[2] = 1;

	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0)
	{
		createDisplayListPoly();
		createDisplayListWire();
		generateNabours();
	}
}


void Geometry::generateNabours(){
	long count = 0;
	//std::map<unsigned int, std::vector<int>> nabours;
	for (int i = 0; i < m_points.size(); i++) {
		std::vector<int> n;
		for (triangle t : m_triangles) {
			for(vertex k: t.v){
				if (k.p == i) {
					for (vertex k : t.v) {
						if (k.p != i) {
							n.push_back(k.p);
						}
						
					}
				}
				count++;
			}
			nabours[i] = n;
		}
	}
	cout << count << endl;
}

void Geometry::readOBJ(string filename)
{

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0, 0, 0));
	m_uvs.push_back(vec2(0, 0));
	m_normals.push_back(vec3(0, 0, 1));


	ifstream objFile(filename);

	if (!objFile.is_open())
	{
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;

	// good() means that failbit, badbit and eofbit are all not set
	while (objFile.good())
	{

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail())
		{

			if (mode == "v")
			{
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back((v*10));

			}
			else if (mode == "vn")
			{
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			}
			else if (mode == "vt")
			{
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			}
			else if (mode == "f")
			{

				vector<vertex> verts;
				while (objLine.good())
				{


					//-------------------------------------------------------------
					// [Assignment 1] :
					// Modify the following to parse the bunny.obj. It has no uv
					// coordinates so each vertex for each face is in the format
					// v//vn instead of the usual v/vt/vn.
					//
					// Modify the following to parse the dragon.obj. It has no
					// normals or uv coordinates so the format for each vertex is
					// v instead of v/vt/vn or v//vn.
					//
					// Hint : Check if there is more than one uv or normal in
					// the uv or normal vector and then parse appropriately.
					//-------------------------------------------------------------

					// Assignment code (assumes you have all of v/vt/vn for each vertex)
					if (m_uvs.size() > 1)
					{
						vertex v;
						objLine >> v.p;		// Scan in position index
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.t;		// Scan in uv (texture coord) index
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.n;		// Scan in normal index

						verts.push_back(v);
					}
					else if (m_normals.size() > 1)
					{
						vertex v;
						objLine >> v.p;		// Scan in position index
						objLine.ignore(1);	// Ignore the '/' character
						//objLine >> v.t;	// Scan in uv (texture coord) index
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.n;		// Scan in normal index

						verts.push_back(v);
					}
					else
					{
						for (int i = 0; i < 3; i++)
						{
							vertex v;
							objLine >> v.p;
							objLine.ignore(1);

							verts.push_back(v);
						}
					}
				}

				// IFF we have 3 verticies, construct a triangle
				if (verts.size() == 3)
				{
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);

				}
			}
		}
	}
	if (m_normals.size() < 1)
	{
		createNormals();
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size() - 1 << " points" << endl;
	cout << m_uvs.size() - 1 << " uv coords" << endl;
	cout << m_normals.size() - 1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;

	/*for (int i = 1; i < m_uvs.size(); i++) {
		cout << m_uvs[i]<<"\n";
	}*/

	// If we didn't have any normals, create them
	if (m_normals.size() <= 1)
	{
		createNormals();
		cout << m_normals.size() - 1 << " normals" << endl;
	}


}

struct pNormals
{
	int point;
	std::vector<int> normals;
	std::vector<int> triangle;
	std::vector<int> vertex;
};

//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to populate the normals for
// the model currently loaded. Compute per face normals
// first and get that working before moving onto calculating
// per vertex normals.
//-------------------------------------------------------------
void Geometry::createNormals()
{
	cout << "Creating Normals" << endl;
	unsigned int i;
	std::vector<vec3> normals;
	std::map<unsigned int, pNormals> Pnorm;

	for (i = 0; i < m_triangles.size() - 1; i++)
	{
		triangle face = m_triangles.at(i);

		vec3 A = m_points.at(face.v[0].p);
		vec3 B = m_points.at(face.v[1].p);
		vec3 C = m_points.at(face.v[2].p);
		
		vec3 U = B - C;
		vec3 V = B - A;

		vec3 faceNormal = cross(U, V);
		normals.push_back(faceNormal);

		for (int k = 0; k < 3; k++)
		{
			m_triangles.at(i).v[k].n = normals.size() - 1; // assignment happens here
		}
		pNormals a;
		if (Pnorm.find(face.v[0].p) != Pnorm.end()) {
			a = Pnorm[face.v[0].p];
		}
		a.point = face.v[0].p;
		a.vertex.push_back(0);
		a.triangle.push_back(i);
		a.normals.push_back(normals.size() - 1);
		Pnorm[face.v[0].p] = a;

		pNormals b;
		if (Pnorm.find(face.v[1].p) != Pnorm.end()) {
			b = Pnorm[face.v[1].p];
		}
		b.point = face.v[1].p;
		b.vertex.push_back(1);
		b.triangle.push_back(i);
		b.normals.push_back(normals.size() - 1);
		Pnorm[face.v[1].p] = b;

		pNormals c;
		if (Pnorm.find(face.v[2].p) != Pnorm.end()) {
			c = Pnorm[face.v[2].p];
		}
		c.point = face.v[2].p;
		c.vertex.push_back(2);
		c.triangle.push_back(i);
		c.normals.push_back(normals.size() - 1);
		Pnorm[face.v[2].p] = c;

		// Pnorm is up dating
	}

	cout << "Point Normals " << Pnorm.size() << endl;
	for (i = 0; i < Pnorm.size(); i++)
	{
		pNormals p;
		p = Pnorm[i];
		//    cout << endl<< " Point "<< p.point << endl;
		vec3 normal;

		for (unsigned int k = 0; k < p.normals.size(); k++)
		{
			normal += normalize(normals.at(p.normals.at(k)));
		}
		if (p.normals.size() > 0) {
			normal /= vec3(p.normals.size());
		}
		m_normals.push_back(normal);//vec3((float)t));

		for (unsigned int z = 0; z < p.triangle.size(); z++)
		{
			m_triangles.at(p.triangle.at(z)).v[p.vertex.at(z)].n = 0;

			m_triangles.at(p.triangle.at(z)).v[p.vertex.at(z)].n = m_normals.size() - 1;
		}

	}

	cout << "Complete Normals" << endl;

}

void Geometry::setTexture()
{
	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_texture); // Generate texture ID

	glBindTexture(GL_TEXTURE_2D, g_texture); // Bind it as a 2D texture


										 // Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->dataPointer());
	// Finnaly, actually fill the data into our texture
	image* im = texture->getImage();
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, im->w, im->h, im->glFormat(), GL_UNSIGNED_BYTE, im->dataPointer());

}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as wireframe model
//-------------------------------------------------------------
void Geometry::createDisplayListPoly()
{
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glScalef(Scale.x, Scale.y, Scale.z);


	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);
	unsigned int i;
	for (i = 0; i < m_triangles.size(); i++)
	{
		glBegin(GL_TRIANGLES);

		triangle points = m_triangles.at(i);

		for (int k = 0; k < 3; k++)
		{
			// cout << points.v[k].n << "|";

			vec3 norm = m_normals.at(points.v[k].n);
			vec2 uvs = m_uvs.at(points.v[k].t);
			vec3 vect = m_points.at(points.v[k].p);

			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(uvs.x, uvs.y);
			glVertex3f(vect.x, vect.y, vect.z);

		}

		glEnd();
	}

	glPopMatrix();
	// YOUR CODE GOES HERE
	// ...


	glEndList();
	cout << "Finished creating Poly Geometry" << endl;
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as polygon model
//-------------------------------------------------------------
void Geometry::createDisplayListWire()
{
	// Delete old list if there is one
	if (m_displayListWire) glDeleteLists(m_displayListWire, 1);


	// Create a new list
	cout << "Creating Wire Geometry" << endl;
	m_displayListWire = glGenLists(1);
	glNewList(m_displayListWire, GL_COMPILE);
	unsigned int i;
	for (i = 1; i < m_triangles.size() - 1; i++)
	{
		glBegin(GL_LINE_LOOP);

		triangle points = m_triangles.at(i);

		for (int k = 0; k < 3; k++)
		{
			vec3 norm = m_normals.at(points.v[k].n);
			vec2 uvs = m_uvs.at(points.v[k].t);
			vec3 vect = m_points.at(points.v[k].p);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(uvs.x, uvs.y);
			glVertex3f(vect.x, vect.y, vect.z);
		}
		glEnd();
	}
	// YOUR CODE GOES HERE
	// ...


	glEndList();
	cout << "Finished creating Wire Geometry" << endl;
}

void Geometry::changeScale(comp308::vec3 s)
{
	Scale = s;
	createDisplayListPoly();
}

void Geometry::initShader() {
#ifdef _WIN32
	cout << "This is Windows. Through visual studios. Shader" << endl;
	g_shader = makeShaderProgram("./res/shaders/shaderDemo.vert", "./res/shaders/shaderDemo.frag");
#else
	cout << "This is Not Windows. Shader" << endl;
	g_shader = makeShaderProgram("work/res/shaders/shaderDemo.vert", "work/res/shaders/shaderDemo.frag");
#endif
	
}


void Geometry::renderGeometry(bool shade)
{
	//-------------------------------------------------------------
	// [Assignment 1] :
	// When moving on to displaying your obj, comment out the
	// glutWireTeapot function & uncomment the glCallList function
	//-------------------------------------------------------------
	glPushMatrix();
	if (thereIsTexture == 1) {
		// Enable Drawing texures
		glEnable(GL_TEXTURE_2D);
		// Use Texture as the color
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		// Set the location for binding the texture
		glActiveTexture(GL_TEXTURE0);
		// Bind the texture

		glBindTexture(GL_TEXTURE_2D, g_texture);
		if (shade) {
			//// Use the shader we made
			glUseProgram(g_shader);

			//// Set our sampler (texture0) to use GL_TEXTURE0 as the source
			glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);

		}

	}
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);



	glMatrixMode(GL_MODELVIEW);

	glRotatef(Rotation.w, Rotation.x, Rotation.y, Rotation.z);

	glTranslatef(Translation.x, Translation.y, Translation.z);
	glShadeModel(GL_SMOOTH);


	//glutSolidTeapot(5.0);
	glCallList(m_displayListPoly);


	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}


void Geometry::toggleWireFrame()
{
	m_wireFrameOn = !m_wireFrameOn;
}

void Geometry::loadTexture(std::string s)
{
	texture = new Texture(s);
	setTexture();
	initShader();
	thereIsTexture = 1;
}

void Geometry::rotate(comp308::vec4 r)
{
	Rotation = r;
	createDisplayListPoly();
}

void Geometry::translateList(std::vector<comp308::vec3> t)
{
	TranslationList = t;
}

void Geometry::translate(comp308::vec3 t)
{
	Translation = t;
}

void Geometry::setAmbient(comp308::vec3 a) {
	mat_ambient[0] = a.x;
	mat_ambient[1] = a.y;
	mat_ambient[2] = a.z;
}

void Geometry::setDiffuse(comp308::vec3 d) {
	mat_diffuse[0] = d.x;
	mat_diffuse[1] = d.y;
	mat_diffuse[2] = d.z;
}

void Geometry::setSpecular(comp308::vec3 s) {
	mat_specular[0] = s.x;
	mat_specular[1] = s.y;
	mat_specular[2] = s.z;
}

void Geometry::setShine(float s) {
	shine = s;
}

void Geometry::clearTransList()
{
	TranslationList.clear();
}



/*	
	http://graphics.stanford.edu/courses/cs468-12-spring/LectureSlides/06_smoothing.pdf

	http://www.faculty.jacobs-university.de/llinsen/teaching/320491/Lecture13.pdf
	
	https://en.wikipedia.org/wiki/Additive_smoothing

		https://en.wikipedia.org/wiki/Laplacian_smoothing
		From Wikipedia, the free encyclopedia
		This article is about the mesh smoothing algorithm.For the multinomial shrinkage estimator, also called Laplace smoothing or add - one smoothing, see additive smoothing.
		Laplacian smoothing is an algorithm to smooth a polygonal mesh.[1][2] For each vertex in a mesh, a new position is chosen based on local 
		information(such as the position of neighbors) and the vertex is moved there.In the case that a mesh is topologically a rectangular
		grid(that is, each internal vertex is connected to four neighbors) then this operation produces the Laplacian of the mesh.

		More formally, the smoothing operation may be described per - vertex as :

		\bar{ x }_{ i } = \frac{ 1 }{N} \sum_{ j = 1 }^{N}\bar{ x }_j
		Where N is the number of adjacent vertices to node i, \bar{ x }_{ j } is the position of the j - th adjacent vertex and \bar{ x }_{ i } is the new position for node i.[3]
		*/
void Geometry::laplaceSmooth()
{
	std::vector<vec3> points;

	for (int i = 0; i < nabours.size(); i++) {
		vec3 a(0, 0, 0);
		vector<int> nab = nabours[i];
		for (int i : nab) {
			a = a + m_points.at(i);
		}
		if (nab.size() != 0) {
		a = a / nab.size();
		points.push_back(a);
		}
	}
 	m_points = points;
	createDisplayListPoly();
}

