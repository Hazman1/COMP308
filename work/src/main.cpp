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
#include <xstring>

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


// Objects to be rendered
//
//Geometry *ball;
//Geometry *box;
//Geometry *bunny;
Geometry *boat;
//Geometry *sphere;
//Geometry *table;
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
	float diff[] = { 0.5f, 0.8f, 0.5f, 1.0f };
	float ambient[] = { 0.05f, 0.1f, 0.05f, 1.0f };
	GLfloat specular[] = { 0.05f, 0.2f, 0.05f, 1.0f };

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
	float diffintensity3[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float ambient3[] = { 0.6f, 0.6f, 0.6f, 1.0f };

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


		//glPushMatrix();
		//bunny->renderGeometry(false);
		//glPopMatrix();

		glPushMatrix();
		glRotatef(120, 1, 0, 0);
		//glRotatef(15, 0, 0, 1);
		glRotatef(180, 0, 1, 0);
		glTranslatef(-5, 0, 0);
		
		boat->renderGeometry(false);
		glPopMatrix();
		//glPushMatrix();
		//torus->renderGeometry(false);
		//glPopMatrix();

		//glPushMatrix();
		//ball->renderGeometry(false);
		//glPopMatrix();

		//glPushMatrix();
		//teapot->renderGeometry(false);
		//glPopMatrix();

		//glPushMatrix();
		//table->renderGeometry(false);
		//glPopMatrix();

		//glPushMatrix();
		//box->renderGeometry(false);
		//glPopMatrix();


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

		/*glPushMatrix();
		table->renderGeometry(true);
		glPopMatrix();*/

	/*	glPushMatrix();
		box->renderGeometry(true);
		glPopMatrix();*/

		/*glPushMatrix();
		bunny->renderGeometry(true);
		glPopMatrix();*/

		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glRotatef(45, 0, 0, 1);
		boat->renderGeometry(true);
		glPopMatrix();

		/*glPushMatrix();
		torus->renderGeometry(true);
		glPopMatrix();

		glPushMatrix();
		ball->renderGeometry(true);
		glPopMatrix();
*/
		//glPushMatrix();
		//teapot->renderGeometry(true);
		//glPopMatrix();

		glFlush();

		//// Render a single square as our geometry
		//// You would normally render your geometry here
		//glBegin(GL_QUADS);
		//glNormal3f(0.0, 0.0, 1.0);
		//glTexCoord2f(0.0, 0.0);
		//glVertex3f(-5.0, -5.0, 0.0);
		//glTexCoord2f(0.0, 1.0);
		//glVertex3f(-5.0, 5.0, 0.0);
		//glTexCoord2f(1.0, 1.0);
		//glVertex3f(5.0, 5.0, 0.0);
		//glTexCoord2f(1.0, 0.0);
		//glVertex3f(5.0, -5.0, 0.0);
		//glEnd();
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

		lightPos[2] = lightPos[2] - 0.5f;
		cout << lightPos[2] << "\n";
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	}
	else if (key == 's') {

		lightPos[2] = lightPos[2] + 0.5f;
		cout << lightPos[2] << "\n";
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	}

	if (key == 'a') {

		lightPos[0] = lightPos[0] - 0.5f;
		cout << lightPos[0] << "\n";
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	}
	else if (key == 'd') {

		lightPos[0] = lightPos[0] + 0.5f;
		cout << lightPos[0] << "\n";
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	}

	if (key == '+') {
		cutoff++;
		if (cutoff<75.0f) {
			base = (tan(radians(cutoff)) * 2);
			cout << base << "\t" << cutoff << "\n";
		}
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	}
	else if (key == '-'&&cutoff>0) {
		cutoff--;
		if (cutoff<75.0f) {
			base = (tan(radians(cutoff)) * 2);
			cout << base << "\t" << cutoff << "\n";
		}
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	}
	cout << (int)key << "\n";
	if (key == 'o') {
		//move um in z 

		spotDir[2] = spotDir[2] - 0.1f;
	}
	else if (key == 'l') {
		//move down in z 
		spotDir[2] = spotDir[2] + 0.1f;
	}

	if (key == 'k') {
		//move up in x
		spotDir[0] = spotDir[0] - 0.1f;
	}
	else if (key == ';') {
		//move down in x 
		spotDir[0] = spotDir[0] + 0.1f;
	}


	if (key == 27)exit(0);

	//101, 103
	//100, 102
	// YOUR CODE GOES HERE
	// ...
}


// Special Keyboard callback
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
	g_mainWindow = glutCreateWindow("COMP308 Assignment 3");


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
#ifdef _WIN32
	cout << "This is Windows." << endl;
	string _bunny = "./res/assets/bunny.obj";
	string _Boat = "./res/assets/Boat.obj";
#else
	cout << "This is Not Windows." << endl;
	string _bunny = "work/res/assets/bunny.obj";
	string _Boat = "work/res/assets/Boat.obj";
#endif
	
	/*bunny = new Geometry(_bunny);
	bunny->translate(vec3(0, 0.95, 0));
	bunny->setAmbient(vec3(0.25, 0.20725, 0.20725));
	bunny->setDiffuse(vec3(1, 0.829, 0.829));
	bunny->setSpecular(vec3(0.296648, 0.296648, 0.296648));
	bunny->setShine(0.088);*/
	
	boat = new Geometry(_Boat);

	/*string _teapot = "./res/assets/teapot.obj";
	teapot = new Geometry(_teapot);
	teapot->translate(vec3(-5.5, 0.9, -5.5));
	teapot->setAmbient(vec3(0.054, 0.127, 0.2125));
	teapot->setDiffuse(vec3(0.18144, 0.4284, 0.714));
	teapot->setSpecular(vec3(0.166721, 0.271906, 0.393548));
	teapot->setShine(0.2);*/
	
	//string _ball = "./res/assets/sphere.obj";
	//ball = new Geometry(_ball);
	//ball->translate(vec3(-5.5, 2.3, 5.5));
	//ball->setAmbient(vec3(0.2125, 0.1275, 0.054));
	//ball->setDiffuse(vec3(0.714, 0.4284, 0.18144));
	//ball->setSpecular(vec3(0.393548, 0.271906, 0.166721));
	//ball->setShine(0.2);

	//string _torus = "./res/assets/torus.obj";
	//torus = new Geometry(_torus);
	//torus->translate(vec3(5.5, 1.4, 5.5));
	//torus->setAmbient(vec3(0.0, 0.0, 0.0));
	//torus->setDiffuse(vec3(0.5, 0.0, 0.0));
	//torus->setSpecular(vec3(0.7, 0.6, 0.6));
	//torus->setShine(0.25);
	//
	//string _table = "./res/assets/table.obj";
	//table = new Geometry(_table);
	//table->loadTexture("work/res/textures/wood.jpg");
	//table->changeScale(vec3(1.2, 1.2, 1.2));
	////table->translate(vec3(0, 0.4, 0));
	////table->setAmbient(vec3(0.21, 0.1275, 0.054));
	////table->setDiffuse(vec3(0.715, 0.4284, 0.18144));
	////table->setSpecular(vec3(0.393548,0.271906,0.166721));
	////table->setShine(0.78125f);
	//
	//string _box = "./res/assets/box.obj";
	//box = new Geometry(_box);
	//box->loadTexture("work/res/textures/brick.jpg");
	//box->changeScale(vec3(2, 2, 2));
	//box->rotate(vec4(0, 1, 0, 180));
	//box->translate(vec3(-5.5, 2.9, 5.5));

	// Loop required by GLUT
	// This will not return until we tell GLUT to finish
	glutMainLoop();

	//delete table;
	//delete bunny;
	delete boat;
	//delete teapot;
	//delete box;
	//delete torus;



	// Don't forget to delete all pointers that we made
	return 0;
}
