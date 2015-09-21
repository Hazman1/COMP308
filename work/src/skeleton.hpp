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
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "comp308.hpp"



// Needed for Completion/Challenge
// We use bitmasking to work out the Degrees of Freedom
// To work out if a bone b has a y-axis dof, simply:
//     if (b.freedom & dof_ry) {...}
//
// To add and subtract degrees of freedom, respectively:
//     b.freedom |= dof_rx
//     b.freedom ^= dof_rx
using dof_set = unsigned int;

enum dof {
	dof_none = 0,
	dof_rx = 1,
	dof_ry = 2,
	dof_rz = 4,
	dof_root = 8 // Root has 6, 3 translation and 3 rotation
};


// Type to represent a bone
struct bone {
	int ID;
	std::string name;
	float length = 0;             // Length of the bone
	comp308::vec3 boneDir;        // Direction of the bone
	comp308::vec3 basisRot;       // Euler angle rotations for the bone basis
	dof_set freedom = dof_none;   // Degrees of freedom for the joint rotation
	std::vector<bone *> children; // Pointers to bone children

	// Completion and Challenge
	comp308::vec3 rotation;       // Rotation of joint in the basis (degrees)

	// Challenge
	comp308::vec3 translation;    // Translation (Only for the Root)
	comp308::vec3 rotation_max;   // Maximum value for rotation for this joint (degrees)
	comp308::vec3 rotation_min;   // Minimum value for rotation for this joint (degrees)
};


class Skeleton {

private:
	std::vector<bone> m_bones;

	// Helper method
	int findBone(std::string);
	
	// Reading code TODO change
	void readASF(std::string);
	void readHeading(std::string, std::ifstream&);
	void readBone(std::ifstream&);
	void readHierarchy(std::ifstream&);

	void renderBone(bone *, GLUquadric*);
	void loadPoses();

public:
	Skeleton(std::string);
	void renderSkeleton();
	void readAMC(std::string);
	void switchPose();
	void increaseSpeed();
	void decreaseSpeed();
	void reverseSpeed();
	void pauseSpeed();
	void IncreeaseFrame();
	bool canPlay();
};
