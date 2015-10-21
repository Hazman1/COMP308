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
#include <cstdlib>
#include <iostream>
#include <cstring>
#include "texture.hpp"

#include "comp308.hpp"
//#include "imageLoader.hpp"

#include "geometry.hpp"

using namespace std;
using namespace comp308;


// Global variables
//
GLuint g_winWidth = 640;
GLuint g_winHeight = 480;
GLuint g_mainWindow = 0;


// Projection values
//
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_mouseDown = false;
vec2 g_mousePos;
float g_yRotation = 0;
float g_xRotation = 0;
float g_zoomFactor = 1.0;


// Scene information
////
//GLuint g_texture = 0;
GLuint g_texture1 = 0;
GLuint g_shader = 0;
bool g_useShader = false;
vec3 scale(1.0f,1.0f,1.0f);
int point =3;
std::vector<Geometry *> Items;
#ifdef _WIN32
string _Item[] = { "./res/assets/bunny.obj" ,"./res/assets/Boat.obj",	"./res/assets/dragon.obj", "./res/assets/table.obj" };
	string str = "./res/assets/test.png";

#else
string _Item[] = { "work/res/assets/bunny.obj" ,"work/res/assets/Boat.obj", "work/res/assets/dragon.obj", "work/res/assets/table.obj" };
string str = "work/res/assets/test.png";
#endif

// Objects to be rendered
//
//Geometry *ball;
//Geometry *box;
//Geometry *bunny;
//Geometry *boat;
//Geometry *sphere;
Geometry *active;
//Geometry *teapot;
//Geometry *torus;

bool light0 = true, light1 = true, light2 = true, light3 = true;
float base = 1;
float cutoff = 45.0f;

GLfloat lightPos[] = { 0.0f, 10.0f, 0.0f, 1.0f };
GLfloat spotDir[] = { 0.0f, -1.0f, 0.0f, 0.0 };


// Sets up where and what the light is
// Called once on start up
//
void initLight() {


	float direction[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff[] = { 1.5f, 1.5f, 1.5f, 1.0f };
	float ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat specular[] = { 0.05f, 0.05f, 0.05f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//GLfloat specular[] = { 5.0f, 5.0f, 10.0f, 1.0f};
	GLfloat ambientLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spotDiff[] = { 5.0f,5.0f,10.0f,1.0f };

	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spotDiff);
	//glLightfv(GL_LIGHT1,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);

	// Specific spot effects
	// Cut off angle is 60 degrees
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);

	base = (tan(radians(cutoff)) * 2);

	// Fairly shiny spot
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 5.0f);

	float diffintensity2[] = { 0.4f, 0.2f, 0.2f, 1.0f };
	float ambient2[] = { 0.2f, 0.1f, 0.1f, 1.0f };
	float direction2[] = { -10.0f, 2.5f,0.0f, 0.0f };

	glLightfv(GL_LIGHT2, GL_POSITION, direction2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffintensity2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 6.0f);


	float diffintensity3[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float ambient3[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffintensity3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);


	// Enable this light in particular
	if (!light0) {
		glDisable(GL_LIGHT0);
	}
	else {
		glEnable(GL_LIGHT0);
	}
	if (!light1) {
		glDisable(GL_LIGHT1);

	}
	else {
		glEnable(GL_LIGHT1);
	}if (!light2) {
		glDisable(GL_LIGHT2);

	}
	else {
		glEnable(GL_LIGHT2);

	}if (!light3) {
		glDisable(GL_LIGHT3);

	}
	else {
		glEnable(GL_LIGHT3);

	}

}

void geometrySetUp() {
	if (Items.size() < 4) {
		active = new Geometry(_Item[point]);
		Items.push_back(active);
	}
	else {
		active = Items.at(point);
	}
#ifdef _WIN32
	active->loadTexture("./res/textures/output.png");
#else
	active->loadTexture("work/res/textures/output.png");
#endif
	active->changeScale(scale);
	if (point == 1) {
		active->translate(vec3(-0.2, 0, 0));
		active->rotate(vec4(90,0,1,0));
	}
	else {
		active->translate(vec3(0, 0.4, 0));
	}
	active->setAmbient(vec3(0.2125, 0.1275, 0.054));
	active->setDiffuse(vec3(0.714, 0.4284, 0.18144));
	active->setSpecular(vec3(0.0, 0.0, 0.0));
	active->setShine(0.0);
}


// Sets up where the camera is in the scene
// Called every frame
//
void setUpCamera() {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, float(g_winWidth) / float(g_winHeight), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -50 * g_zoomFactor);
	glRotatef(g_xRotation, 1, 0, 0);
	glRotatef(g_yRotation, 0, 1, 0);
}


// Draw function
//
void draw() {
	initLight();
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	setUpCamera();
	if (light1) {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);

		glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
		glDisable(GL_LIGHTING);
		glColor3f(0.0, 1.0, 1.0);
		glRotatef(90, -1.0f, 0.0f, 0.0f);
		//glRotatef(1,spotDir[0],spotDir[1],spotDir[2]);

		glutWireCone(base, 2, 100, 100);


		glEnable(GL_LIGHTING);
		glPopMatrix();


	}


	// Without shaders
	//
	if (!g_useShader) {



		glPushMatrix();
		active->renderGeometry(false);
		glPopMatrix();



		glFlush();

		// With shaders (no lighting)
		//
	}
	else {

		//// Texture setup
		////

		//Enable Drawing texures
		//glEnable(GL_TEXTURE_2D);
		//// Set the location for binding the texture
		//glActiveTexture(GL_TEXTURE0);
		//// Bind the texture
		//glBindTexture(GL_TEXTURE_2D, g_texture);
		// glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		//// Use the shader we made
		//glUseProgram(g_shader);

		//// Set our sampler (texture0) to use GL_TEXTURE0 as the source
		//glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);

		glPushMatrix();
		active->renderGeometry(true);
		glPopMatrix();


		glFlush();


		// Unbind our shader
		glUseProgram(0);

	}


	// Disable flags for cleanup (optional)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	// Move the buffer we just drew to the front
	glutSwapBuffers();

	// Queue the next frame to be drawn straight away
	glutPostRedisplay();
}


// Reshape function
//
void reshape(int w, int h) {
	if (h == 0) h = 1;

	g_winWidth = w;
	g_winHeight = h;

	// Sets the openGL rendering window to match the window size
	glViewport(0, 0, g_winWidth, g_winHeight);
}


// Keyboard callback
// Called once per button state change
//
void keyboardCallback(unsigned char key, int x, int y) {
	cout << "Keyboard Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;

	if (key == '1') {
		if (light0) {
			light0 = false;
		}
		else {
			light0 = true;
		}
	}
	else if (key == '2') {
		if (light1) {
			light1 = false;
		}
		else {
			light1 = true;
		}
	}
	else if (key == '3') {
		if (light2) {
			light2 = false;
		}
		else {
			light2 = true;
		}
	}
	else if (key == '4') {
		if (light3) {
			light3 = false;
		}
		else {
			light3 = true;
		}
	}
	//GLfloat lightPos[] = { 0.0f, 5.0f, 0.0f, 0.0f };
	//GLfloat spotDir[] = { 0.0f, -1.0f, 0.0f, 1.0f };
	if (key == 'w') {

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	else if (key == 's') {

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == '+') {
		vec3 temp(0.1f, 0.1f, 0.1f);
		scale = scale + temp;
		active->changeScale(scale);
		cout << "Scale = " << scale << endl;

	}
	else if (key == '-'&&cutoff > 0) {
		vec3 temp(0.1f, 0.1f, 0.1f);
		scale = scale - temp;
		if (scale.x < 0)scale = vec3(0.0f, 0.0f, 0.0f);
		active->changeScale(scale);
		cout << "Scale = " << scale << endl;
	}

	cout << (int)key << "\n";


	if (key == ' ') {
		//bunny->laplaceSmooth();
		active->laplaceSmooth();
	}

	if (key == 'n') {
		Texture* t = new Texture(str);
#ifdef _WIN32
		active->loadTexture("./res/textures/output.png");
#else
		active->loadTexture("work/res/textures/output.png");
#endif
	}

	if (key == 'v') {
		point = (point + 1) % 4;
		geometrySetUp();
	}


	if (key == 27)exit(0);

}


// Special Keybhttp://www.faculty.jacobs-university.de/llinsen/teaching/320491/Lecture13.pdfoard callback
// Called once per button state change
//
void specialCallback(int key, int x, int y) {
	cout << "Special Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
}


// Mouse Button Callback function
// Called once per button state change
//
void mouseCallback(int button, int state, int x, int y) {
	cout << "Mouse Callback :: button=" << button << ", state=" << state << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	switch (button) {

	case 0: // left mouse button
		g_mouseDown = (state == 0);
		g_mousePos = vec2(x, y);
		break;

	case 2: // right mouse button
		if (state == 0)
			g_useShader = !g_useShader;
		break;

	case 3: // scroll foward/up
		g_zoomFactor /= 1.1;
		break;

	case 4: // scroll back/down
		g_zoomFactor *= 1.1;
		break;
	}
}


// Mouse Motion Callback function
// Called once per frame if the mouse has moved and
// at least one mouse button has an active state
//
void mouseMotionCallback(int x, int y) {
	cout << "Mouse Motion Callback :: x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	if (g_mouseDown) {
		vec2 dif = vec2(x, y) - g_mousePos;
		g_mousePos = vec2(x, y);
		g_yRotation += 0.3 * dif.x;
		g_xRotation += 0.3 * dif.y;
	}
}



//Main program
//
int main(int argc, char **argv) {



	if (argc != 1) {
		cout << "No arguments expected" << endl;
		exit(EXIT_FAILURE);
	}

	// Initialise GL, GLU and GLUT
	glutInit(&argc, argv);

	// Setting up the display
	// - RGB color model + Alpha Channel = GLUT_RGBA
	// - Double buffered = GLUT_DOUBLE
	// - Depth buffer = GLUT_DEPTH
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Initialise window size and create window
	glutInitWindowSize(g_winWidth, g_winHeight);
	g_mainWindow = glutCreateWindow("Final Project");


	// Initilise GLEW
	// must be done after creating GL context (glutCreateWindow in this case)
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}

	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;



	// Register functions for callback
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);

	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);

	initLight();
	//initShader();



	Texture* t = new Texture(str);


	geometrySetUp();


	// Loop required by GLUT
	// This will not return until we tell GLUT to finish
	glutMainLoop();
	for (int i = 0; i < Items.size(); i++) {
		delete (Items[i]);
	}
	delete active;



	// Don't forget to delete all pointers that we made
	return 0;
}
