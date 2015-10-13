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
#include <string>
#include <string.h>
#include <cstring>

#include "comp308.hpp"
#include "skeleton.hpp"
#include "geometry.hpp"
#include "splines.hpp"
#include "quat.hpp"

using namespace std;
using namespace comp308;


// Global variables
// 
GLuint g_winWidth = 640;
GLuint g_winHeight = 480;
GLuint g_winSecWidth = 320;
GLuint g_winSecHeight = 240;
GLuint g_mainWindow = 0;


// Projection values
// 
float g_fovy = 20.0;
float g_znear = -5;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_mouseDown = false;
vec2 g_mousePos;
float g_yRotation = 0;
float g_xRotation = 0;
float g_zoomFactor = 1.0;

int maxX, maxY;
// Geometry loader and drawer
//

Geometry *g_geometry = nullptr;
Geometry *g_reference = nullptr;

//Used for Splins and stuff
//
Splines *spline;
Splines *Speed;

// Sets up where and what the light is
// Called once on start up
// 
void initLight() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float diffintensity[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	//glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHT0);
}

enum MENU_TYPE
{
	MENU_PLAY,
	MENU_CLEAR,
	MENU_RESTART,
	MENU_SAVE,
};

// Menu handling function definition
void menu(int item)
{
	switch (item)
	{
	case MENU_PLAY:
	{
		if (g_geometry != nullptr) {
			if (spline->ready()) {
				g_geometry->translateList(spline->getNavPoints());
			}
		}
		break;
	}
	break;
	case MENU_CLEAR:
	{
		if (glutGetWindow() == g_mainWindow) {
			spline->clearSpline();
			if (g_geometry != nullptr) {
				g_geometry->clearTransList();
			}
		}
		else {
			Speed->clearSpline();
		}

		break;
	}
	case MENU_RESTART:
	{
		if (g_geometry != nullptr) {
			g_geometry->clearTransList();
		}

	}break;
	}
	glutPostRedisplay();

	return;
}

// Sets up where the camera is in the scene
// Called every frame
// 
void setUpCamera() {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*if (g_geometry != nullptr) {
		g_znear = -5;
		glOrtho(-2, g_winWidth / 10, -2, g_winHeight / 10, g_znear, g_zfar);
	} else {*/
		gluPerspective(g_fovy, float(g_winWidth) / float(g_winHeight),
			g_znear, g_zfar);
	//}

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Load camera transforms
	glTranslatef(0, 0, -5 * g_zoomFactor);
	glRotatef(g_xRotation, 1, 0, 0);
	glRotatef(g_yRotation, 0, 1, 0);
	//	gluLookAt(g_winWidth / 2, g_winHeight / 2, 0, g_winWidth / 2, g_winHeight / 2,0, 0, 1, 0);
}


// Draw function
//
void draw() {

	// Set up camera every frame
	setUpCamera();

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex3f(0.0f, 0.0f,0.0f);  
	glVertex3f(15.0f, 0.0f,0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f); // Blue
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 15.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f); // Green
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 15.0f);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	// Set the current material (for all objects) to red
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.0f, 0.0f, 0.0f);

	if(g_reference!=nullptr){
		g_reference->renderGeometry(false);	
	}

	if(g_geometry!=nullptr){
		g_geometry->renderGeometry(false);
	}

	spline->renderSpline();
	// Disable flags for cleanup (optional)
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);

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


//-------------------------------------------------------------
// [Assignment 2] :
// Modify the keyboardCallback function and additional files,
// to make your priman pose when the 'p' key is pressed.
//-------------------------------------------------------------

// Keyboard callback
// Called once per button state change
//
void keyboardCallback(unsigned char key, int x, int y) {
	cout << "Keyboard Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...



}


// Special Keyboard callback
// Called once per button state change
//
void specialCallback(int key, int x, int y) {
	// cout << "Special Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// Not needed for this assignment, but useful to have later on
}


// Mouse Button Callback function
// (x,y) :: (0,0) is top left and (g_winWidth, g_winHeight) is bottom right
// state :: 0 is down, 1 is up
// 
// Called once per button state change
// 
void mouseCallback(int button, int state, int x, int y) {
	//cout << "Mouse Callback :: button=" << button << ", state=" << state << ", (" << x << "," << y << ")" << endl;
	switch (button) {
	case 0:
	{
		if (state == 0) {

		//	if (g_geometry != nullptr) {
		//		cout << "create splines\n";
		//		spline->addPoint(vec3(x / 10, (g_winHeight - y) / 10, -4));
		//	}
		//	else
		//		;
		}
		break;
	}
	case 1:
	{//left mouse button
		cout << "Pan";
		if (state == 0) {
			g_mouseDown = (state == 0);
			g_mousePos = vec2(x / 10, y / 10);
		}

		//if clicking make point in world

		break;
	}
	case 3: { //scroll foward/up
		g_zoomFactor /= 1.1;
		break;
	}
	case 4: { //scroll back/down
		g_zoomFactor *= 1.1;
		break;
	}
	}
}

// Mouse Button Callback function
// (x,y) :: (0,0) is top left and (g_winWidth, g_winHeight) is bottom right
// state :: 0 is down, 1 is up
// 
// Called once per button state change
// 
void secondMouseCallback(int button, int state, int x, int y) {
	//cout << "Mouse Callback :: button=" << button << ", state=" << state << ", (" << x << "," << y << ")" << endl;
	switch (button) {
	case 0:
		if (state == 0) {
			cout << "create splines\n";
			Speed->addPoint(vec3(x / 5, (g_winSecHeight - y) / 5, -4));
		}
		break;
	case 1: //left mouse button
		g_mouseDown = (state == 0);
		g_mousePos = vec2(x / 10, y / 10);
		//if clicking make point in world

		break;

	case 3: //scroll foward/up
		g_zoomFactor /= 1.1;
		break;

	case 4: //scroll back/down
		g_zoomFactor *= 1.1;
		break;
	};
}


// Mouse Motion Callback function
// Called once per frame if the mouse has moved and
// at least one mouse button has an active state
// 
void mouseMotionCallback(int x, int y) {
	//cout << "Mouse Motion Callback :: (" << x << "," << y << ")" << endl;
	if (g_geometry != nullptr) {
		return;
	}
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

	//if (argc < 2) {
	//	cout << "ASF filename expected" << endl << "    ./a2 priman.asf" << endl;
	//	abort(); // Unrecoverable error
	//}
	//else if (argc > 3) {
	//	cout << "Too many arguments, expected only ASF and AMC filenames" << endl;
	//	cout << "    ./a2 priman.asf priman.amc" << endl;
	//	abort(); // Unrecoverable error
	//}

	if (strstr(argv[1], ".obj") != nullptr) {
		g_geometry = new Geometry(argv[1]);
	}

	if (argc > 2 && strstr(argv[2], ".obj") != nullptr) {
		g_reference = new Geometry(argv[2]);
	}
	
	

	spline = new Splines();

	// Initialise GL, GLU and GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Initialise window size and create window
	glutInitWindowSize(g_winWidth, g_winHeight);
	g_mainWindow = glutCreateWindow("COMP308 Project");

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

	glutCreateMenu(menu);

	glutAddMenuEntry("Play", MENU_PLAY);
	glutAddMenuEntry("Clear Splines", MENU_CLEAR);
	glutAddMenuEntry("Restart", MENU_RESTART);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Create a light on the camera
	initLight();

	// Loop required by OpenGL
	// This will not return until we tell OpenGL to finish
	glutMainLoop();

	// Don't forget to delete all pointers that we made
	
	delete g_geometry;
	return 0;
}