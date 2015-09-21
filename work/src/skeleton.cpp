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
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "comp308.hpp"
#include "skeleton.hpp"

using namespace std;
using namespace comp308;

struct BonMotion {
	string name;
	std::vector<float> info;
};

struct Frame {
	int number = -1;
	std::vector<BonMotion> data;
};

struct Clip {
	std::vector<Frame> Frames;
};

int FrameNumber=0;
int speed = 1;
Clip LoadedMoves;
Frame current;
int poseIndex = 3;
Frame Pose[3];

Skeleton::Skeleton(string filename) {
	bone b = bone(); 
	b.name = "root";
	b.freedom |= dof_rx;
	b.freedom |= dof_ry;
	b.freedom |= dof_rz;
	b.freedom |= dof_root;
	m_bones.push_back(b);
	readASF(filename);
	loadPoses();
	current.number = -1;
}

// [Assignment2] you may need to revise this function
void Skeleton::renderSkeleton() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//Create a new GLUquadric object; to allow you to draw cylinders
	GLUquadric *quad = gluNewQuadric();
	if (quad == 0) {
		cerr << "Not enough memory to allocate space to draw" << endl;
		exit(EXIT_FAILURE);
	}

	//Actually draw the skeleton
	renderBone(&m_bones[0], quad);

	// Clean up
	gluDeleteQuadric(quad);
	glPopMatrix();
}



//-------------------------------------------------------------
// [Assignment 2] :
// Should render each bone by drawing the axes, and the bone
// then translating to the end of the bone and drawing each
// of it's children. Remember to use the basis rotations
// before drawing the axes (and for Completion, rotation).
// The actual bone is NOT drawn with the basis rotation.
//
// Should not draw the root bone (because it has zero length)
// but should go on to draw it's children
//-------------------------------------------------------------
void Skeleton::renderBone(bone *b, GLUquadric* q) {
	glLoadName(b->ID);
	int scale = 100;
	double R = 0.015*scale;
	std::vector<float> motion;
	if (current.number!=-1) {
		//std::vector<float> motion;
			for (int k = 0; k < current.data.size(); k++) {
				string na = current.data.at(k).name;

				if (na== b->name) {

					motion = current.data.at(k).info;
					break;
				}
			}
		}
	

	//cout << b->name<<"  "<< b->freedom<<endl;

	if (b->length > 0) {

		glRotatef(b->basisRot.z, 0, 0, 1);
		glRotatef(b->basisRot.y, 0, 1, 0);
		glRotatef(b->basisRot.x, 1, 0, 0);

		// find the rotation for the bone
		// applie that rotation and continue on your way.
		if (motion.size() > 0) {
			int i = 0;
			if (b->freedom & dof_rx) {
				if (motion.at(i) < b->rotation_max.x && motion.at(i) > b->rotation_min.x) {
					glRotatef(motion.at(i), 1, 0, 0);
					i++;
				}
			}
			if (b->freedom & dof_ry) {
				if (motion.at(i) < b->rotation_max.y && motion.at(i) > b->rotation_min.y) {
					glRotatef(motion.at(i), 0, 1, 0);
					i++;
				}
			}


			if (b->freedom & dof_rz) {
				if (motion.at(i) < b->rotation_max.z && motion.at(i) > b->rotation_min.z) {
					glRotatef(motion.at(i), 0, 0, 1);
					i++;
				}
			}


		}
		//Blue Ball
		glColor3f(0, 1, 1);
		glutSolidSphere(1.2*R, 100, 100);

		// Z-axis BLUE
		glPushMatrix();
		glColor3f(0, 0, 1);
		//glRotatef(90, 0, 1, 0);
		gluCylinder(q, 0.3*R, 0.3*R, 4 * R, 100, 100);
		glTranslatef(0, 0, 4 * R);
		glutSolidCone(0.6*R, 0.6*R, 100, 100);
		glPopMatrix();

		// Y-axis GREEN
		glPushMatrix();
		glColor3f(0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 0.3*R, 0.3*R, 4 * R, 100, 100);
		glTranslatef(0, 0, 4 * R);
		glutSolidCone(0.6*R, 0.6*R, 100, 100);
		glPopMatrix();

		// X-axis RED
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(90, 0, 1, 0);
		gluCylinder(q, 0.3*R, 0.3*R, 4 * R, 100, 100);
		glTranslatef(0, 0, 4 * R);
		glutSolidCone(0.6*R, 0.6*R, 100, 100);
		glPopMatrix();

		glRotatef(-b->basisRot.x, 1, 0, 0);
		glRotatef(-b->basisRot.y, 0, 1, 0);
		glRotatef(-b->basisRot.z, 0, 0, 1);

		//Bone Segment
		glPushMatrix();
		glColor3f(0.8, 0.8, 0.8);

		// dot product for angle for on the vector
		float zRotate = degrees(acos(dot(vec3(0, 0, 1), b->boneDir)));//Z axis

		//cross for the vector to rotate on
		vec3 zVect = cross(vec3(0, 0, 1), b->boneDir);

		glRotatef(zRotate, zVect.x, zVect.y, zVect.z);
		gluCylinder(q, R, R / 3, b->length*scale, 100, 100);

		glPopMatrix();


	}
	else {
		glTranslatef(b->translation.x, b->translation.y, b->translation.z);
		if (motion.size() == 6) {
			glTranslatef(motion.at(0), motion.at(1), motion.at(2));
			int i = 3;

			if (b->freedom & dof_rz) {
				glRotatef(motion.at(i), 0, 0, 1);
				i++;
			}
			if (b->freedom & dof_ry) {
				glRotatef(motion.at(i), 0, 1, 0);
				i++;
			}

			if (b->freedom & dof_rx) {
				glRotatef(motion.at(i), 1, 0, 0);
				i++;
			}
		}
		
	}

	for (bone * temp : b->children) {
		glPushMatrix();
		vec3 newLoc = (b->boneDir) *b->length*scale;
		glTranslatef(newLoc.x, newLoc.y, newLoc.z);

		renderBone(temp, q);
		glPopMatrix();
	}
}


// Helper method for retreiving and trimming the next line in a file.
// You should not need to modify this method.
namespace {
	string nextLineTrimmed(istream &file) {
		// Pull out line from file
		string line;
		getline(file, line);
		// Remove leading and trailing whitespace and comments
		size_t i = line.find_first_not_of(" \t\r\n");
		if (i != string::npos) {
			if (line[i] != '#') {
				return line.substr(i, line.find_last_not_of(" \t\r\n") - i + 1);
			}
		}
		return "";
	}
}


int Skeleton::findBone(string name) {
	for (size_t i = 0; i < m_bones.size(); i++)
		if (m_bones[i].name == name)
			return i;
	return -1;
}


void Skeleton::readASF(string filename) {

	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Failed to open file " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file" << filename << endl;

	// good() means that failbit, badbit and eofbit are all not set
	while (file.good()) {

		// Pull out line from file
		string line = nextLineTrimmed(file);

		// Check if it is a comment or just empty
		if (line.empty() || line[0] == '#')
			continue;
		else if (line[0] == ':') {
			// Line starts with a ':' character so it must be a header
			readHeading(line, file);
		}
		else {
			// Would normally error here, but becuase we don't parse
			// every header entirely we will leave this blank.
		}
	}

	cout << "Completed reading skeleton file" << endl;
}


void Skeleton::readHeading(string headerline, ifstream &file) {

	string head;
	istringstream lineStream(headerline);
	lineStream >> head; // get the first token from the stream

	// remove the ':' from the header name
	if (head[0] == ':')
		head = head.substr(1);

	if (lineStream.fail() || head.empty()) {
		cerr << "Could not get heading name from\"" << headerline << "\", all is lost" << endl;
		throw runtime_error("Error :: could not parse .asf file.");
	}

	if (head == "version") {
		//version string - must be 1.10
		string version;
		lineStream >> version;
		if (lineStream.fail() || version != "1.10") {
			cerr << "Invalid version: \"" << version << "\" must be 1.10" << endl;
			throw runtime_error("Error :: invalid .asf version.");
		}
	}
	else if (head == "name") {
		// This allows the skeleton to be called something
		// other than the file name. We don't actually care
		// what the name is, so can ignore this whole section
	}
	else if (head == "documentation") {
		// Documentation section has no meaningful information
		// only of use if you want to copy the file. So we skip it
	}
	else if (head == "units") {
		// Has factors for the units to be able to model the
		// real person, these must be parsed correctly. Only
		// really need to check if deg or rad, but that is 
		// not needed for this assignment.

		// We are going to assume that the units:length feild
		// is 0.45, and that the angles are in degrees
	}
	else if (head == "root") {
		// Read in information about root. Let's just assume
		// it'll be at the origin for this assignment.
	}
	else if (head == "bonedata") {
		// Read in each bone until we get to the
		// end of the file or a new header
		string line = nextLineTrimmed(file);
		while (file.good() && !line.empty()) {
			if (line[0] == ':') {
				// finished our reading of bones
				// read next header and return
				return readHeading(line, file);
			}
			else if (line == "begin") {
				// Read the bone data
				readBone(file);
			}
			else {
				cerr << "Expected 'begin' in bone data, found \"" << line << "\"";
				throw runtime_error("Error :: could not parse .asf file.");
			}
			line = nextLineTrimmed(file);
		}
	}
	else if (head == "hierarchy") {
		// Description of how the bones fit together
		// Read in each line until we get to the
		// end of the file or a new header
		string line = nextLineTrimmed(file);
		while (file.good() && !line.empty()) {
			if (line[0] == ':') {
				// finished our reading of bones
				// read next header and return
				return readHeading(line, file);
			}
			else if (line == "begin") {
				// Read the entire hierarchy
				readHierarchy(file);
			}
			else {
				cerr << "Expected 'begin' in hierarchy, found \"" << line << "\"";
				throw runtime_error("Error :: could not parse .asf file.");
			}
			line = nextLineTrimmed(file);
		}
	}
	else {
		// Would normally error here, but becuase we don't parse
		// every header entirely we will leave this blank.
	}
}


void Skeleton::readBone(ifstream &file) {
	// Create the bone to add the data to
	bone b;

	string line = nextLineTrimmed(file);
	while (file.good()) {
		if (line == "end") {
			// End of the data for this bone
			// Push the bone into the vector
			b.ID = m_bones.size();
			m_bones.push_back(b);
			return;
		}
		else {

			string head;
			istringstream lineStream(line);
			lineStream >> head; // Get the first token

			if (head == "name") {
				// Name of the bone
				lineStream >> b.name;
			}
			else if (head == "direction") {
				// Direction of the bone
				lineStream >> b.boneDir.x >> b.boneDir.y >> b.boneDir.z;
				b.boneDir = normalize(b.boneDir); // Normalize here for consistency
				//cout << b.boneDir << endl;
			}
			else if (head == "length") {
				// Length of the bone
				float length;
				lineStream >> length;
				length *= (1.0 / 0.45);  // scale by 1/0.45 to get actual measurements
				length *= 0.0254;      // convert from inches to meters
				b.length = length;
			}
			else if (head == "dof") {
				// Degrees of Freedom of the joint (rotation)
				while (lineStream.good()) {
					string dofString;
					lineStream >> dofString;
					if (!dofString.empty()) {
						// Parse each dof string
						if (dofString == "rx") b.freedom |= dof_rx;
						else if (dofString == "ry") b.freedom |= dof_ry;
						else if (dofString == "rz") b.freedom |= dof_rz;
						else throw runtime_error("Error :: could not parse .asf file.");
					}
				}
			}
			else if (head == "axis") {
				// Basis rotations 
				lineStream >> b.basisRot.x >> b.basisRot.y >> b.basisRot.z;
			}
			else if (head == "limits") {
				// Limits for each of the DOF
				// Assumes dof has been read first
				// You can optionally fill this method out
			}

			// Because we've tried to parse numerical values
			// check if we've failed at any point
			if (lineStream.fail()) {
				cerr << "Unable to parse \"" << line << "\"";
				throw runtime_error("Error :: could not parse .asf file.");
			}
		}

		// Get the next line
		line = nextLineTrimmed(file);
	}

	cerr << "Expected end in bonedata, found \"" << line << "\"";
	throw runtime_error("Error :: could not parse .asf file.");
}


void Skeleton::readHierarchy(ifstream &file) {
	string line = nextLineTrimmed(file);
	while (file.good()) {
		if (line == "end") {
			// End of hierarchy
			return;
		}
		else if (!line.empty()) {
			// Read the parent node
			string parentName;
			istringstream lineStream(line);
			lineStream >> parentName;

			// Find the parent bone and have a pointer to it
			int parentIndex = findBone(parentName);

			if (parentIndex < 0) {
				cerr << "Expected a valid parent bone name, found \"" << parentName << "\"" << endl;
				throw runtime_error("Error :: could not parse .asf file.");
			}

			//Read the connections
			string childName;
			lineStream >> childName;
			while (!lineStream.fail() && !childName.empty()) {

				int childIndex = findBone(childName);

				if (childIndex < 0) {
					cerr << "Expected a valid child bone name, found \"" << childName << "\"" << endl;
					throw runtime_error("Error :: could not parse .asf file.");
				}

				// Set a POINTER to the child to be recorded in the parents
				m_bones[parentIndex].children.push_back(&m_bones[childIndex]);

				// Get the next child
				lineStream >> childName;
			}
		}
		line = nextLineTrimmed(file);
	}
	cerr << "Expected end in bonedata, found \"" << line << "\"";
	throw runtime_error("Error :: could not parse .asf file.");
}

//-------------------------------------------------------------
// [Assignment 2] :
// Complete the following method to load data from an *.amc file
//-------------------------------------------------------------
void Skeleton::readAMC(string filename) {

	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Failed to open file " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file" << filename << endl;
	string line = nextLineTrimmed(file);
	string head;

	//lineStream >> head; // get the first token from the stream


	Clip clips;
	Frame frame;
	cout << "Starting the Load this will take some time" << endl;
	// good() means that failbit, badbit and eofbit are all not set
	while (file.good()) {
		istringstream lineStream(line);

			//cout << lineStream.peek() << endl;
		 if (lineStream.peek() == -1||lineStream.peek() == ':') {
			 ;
		}
		else {
			if (isdigit(lineStream.peek())) {
				if (frame.number != -1) {
					//Frame f1;
					//f1.number = frame.number;
					//f1.data = frame.data.c;
					clips.Frames.push_back(frame);
					frame.data.clear();
				}
				//frame = new Frame;
				lineStream >> frame.number;
				
			}
			else {
				lineStream >> head; // get the first token from the stream
				BonMotion Bm;
				Bm.name = head;
				while (lineStream.good()) {
					float num;
					lineStream >> num;
					Bm.info.push_back(num);
				}
				frame.data.push_back(Bm);
			}
		}
		line = nextLineTrimmed(file);

	}

	LoadedMoves = clips;
	cout << "Completed reading skeleton file" << endl;
}

void Skeleton::switchPose()
{
	cout << "switch pose" << endl;
	poseIndex = (poseIndex + 1) % 4;
	if (poseIndex >2) {
		current.number = -1;
		current.data.clear();
	}
	else {
		current = Pose[poseIndex];
	}
}

void Skeleton::increaseSpeed() { 
	speed++;
}
void Skeleton::decreaseSpeed() { 
	if(speed>1)
		speed--;
}
void Skeleton::reverseSpeed() {
	if(speed >=0 )
		speed = -1;
	else 
		speed = 1;
}
void Skeleton::pauseSpeed() { 
	if(speed!=0)
		speed = 0;
	else 
		speed = 1;
}

void Skeleton::IncreeaseFrame() {
	FrameNumber = (FrameNumber + speed) % (LoadedMoves.Frames.size()-1);
	current = LoadedMoves.Frames.at(FrameNumber);
}

bool Skeleton::canPlay()
{
	return LoadedMoves.Frames.size() > 0;
}

string splitter(string line) {
	size_t k = line.find_first_not_of(" \t\r\n");
	if (k != string::npos) {
		if (line[k] != '#') {
			return line.substr(k, line.find_last_not_of(" \t\r\n") - k + 1);
		}
	}
}
Frame parseFrame(std::vector<string>& act) {

	Frame temp;
	temp.number = 1;
	for (int i = 0; i < act.size(); i++) {
		string line = splitter(act.at(i));

		istringstream lineStream(line);

		BonMotion bm;
		lineStream >> bm.name;
		while (lineStream.good()) {
			float num;
			lineStream >> num;
			bm.info.push_back(num);
		}
		temp.data.push_back(bm);

	}
	return temp;
}

void Skeleton::loadPoses() {
	std::vector<string> One {"root 0 0 0 7.89575 -1.23166 -0.0566893",
		"lowerback -5.26482 1.22223 -1.36732",
		"upperback -3.86173 1.68698 1.26241",
		"thorax -1.09963 0.887799 2.13322",
		"lowerneck -17.9397 -4.82358 -13.2942",
		"upperneck 23.2383 -5.99768 11.7292",
		"head 11.2831 -2.52518 5.67463",
		"rclavicle -6.72141e-015 9.54166e-015",
		"rhumerus -46.1107 11.3479 -90.9794",
		"rradius 30.8378",
		"rwrist -13.4619",
		"rhand -33.2522 -24.6513",
		"rfingers 7.12502",
		"rthumb -6.4577 -54.5328",
		"lclavicle -6.72141e-015 9.54166e-015",
		"lhumerus -38.0785 -4.69743 89.4066",
		"lradius 19.1919",
		"lwrist -5.42006",
		"lhand -11.9343 -23.666",
		"lfingers 7.12502",
		"lthumb 14.1203 5.83417",
		"rfemur -10.0917 -1.92979 19.5507",
		"rtibia 61.6548",
		"rfoot 7.89252 -7.88495",
		"rtoes -7.98059",
		"lfemur -30.2611 4.21075 -25.5945",
		"ltibia 29.708",
		"lfoot -7.2351 5.89732",
		"ltoes -2.23253"};

	std::vector<string> Two = { "root 0 0 0 -549.864 -41.7659 544.186",
		"lowerback 18.1811 2.56547 -6.29814",
		"upperback 1.07442 2.33024 4.38875",
		"thorax -9.51867 1.42213 7.8661",
		"lowerneck -12.6061 -7.89498 -2.19705",
		"upperneck 19.4398 -10.8563 -0.0530808",
		"head 8.21908 -5.4463 -0.850265",
		"rclavicle 2.34566e-014 1.19271e-014",
		"rhumerus 11.1056 10.6226 -73.3819",
		"rradius 96.055",
		"rwrist -39.8546",
		"rhand -7.29715 20.4312",
		"rfingers 7.12502",
		"rthumb 18.585 -8.70102",
		"lclavicle 2.34566e-014 1.19271e-014",
		"lhumerus 16.1272 -70.922 73.8293",
		"lradius 53.9725",
		"lwrist 81.1055",
		"lhand -4.53761 35.0891",
		"lfingers 7.12502",
		"lthumb 21.2376 63.9456",
		"rfemur -10.7144 7.90539 -4.70273",
		"rtibia 20.6936",
		"rfoot -30.951 -11.4548",
		"rtoes 53.7922",
		"lfemur -31.211 -4.90002 -19.689",
		"ltibia 70.6019",
		"lfoot -26.7766 -7.58725",
		"ltoes 8.02731 "};
	std::vector<string> Three = { "root 0.606783 9.70903 -7.27609 -34.2652 44.3509 1.10213",
		"lowerback 28.8717 0.0555104 -3.26297",
		"upperback 12.0173 -0.276842 -1.05648",
		"thorax -2.38814 0.0285976 0.784246",
		"lowerneck -12.0881 -3.00184 -7.82314",
		"upperneck 12.8542 -3.72462 7.30261",
		"head 6.16584 -1.62077 3.28096",
		"rclavicle 5.28519e-014 4.37326e-015",
		"rhumerus -38.7052 11.4127 -73.6648",
		"rradius 65.3629",
		"rwrist 1.44888",
		"rhand -40.2776 10.4027",
		"rfingers 7.12502",
		"rthumb -13.2353 -19.1571",
		"lclavicle 5.28519e-014 4.37326e-015",
		"lhumerus -39.9374 -19.5191 82.6348",
		"lradius 52.5828",
		"lwrist 18.2619",
		"lhand -11.5636 30.2636",
		"lfingers 7.12502",
		"lthumb 14.4775 59.7385",
		"rfemur -66.8407 -8.67779 6.63314",
		"rtibia 97.8008",
		"rfoot -2.67351 -5.34348",
		"rtoes -3.85532",
		"lfemur -62.4642 18.4873 -3.16277",
		"ltibia 118.948",
		"lfoot -2.81908 -10.7665",
		"ltoes -2.64851" };





	Pose[0] = parseFrame(One);
	Pose[1] = parseFrame(Two);
	Pose[2] = parseFrame(Three);
}
