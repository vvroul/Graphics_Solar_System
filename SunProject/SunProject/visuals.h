#ifndef VISUALS_H
#define VISUALS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include <math.h>
#include <climits>
#include <limits>
#include "gl/glut.h"
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4

using namespace std;

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
}


struct point
{
	float x;
	float y;
	float z;
};

struct face
{
	int vtx[3];
	int nrm[3];
};

struct model
{
	point obj_points[10];
	face obj_faces[4];
	int vertices;
	int faces;
};



struct Info
{
	int starNumber;
};

struct Positions
{
	GLfloat x_pos;
	GLfloat y_pos;
	GLfloat z_pos;
};


//-------- Functions --------------------------------

// The function responsible for drawing everything in the OpenGL context associated to a window. 
void Render();

// Handle the window size changes and define the world coordinate system and projection type
void Resize(int w, int h);

// Set up the OpenGL state machine and create a light source
void Setup();

void Idle();

//Function for reading a model file
void ReadFile(model*);

// Function for displaying a model
void DisplayModel(model);

// Function for handling keyboard events.
void Keyboard(unsigned char key, int x, int y);

void StarsCreation(int howManyStars);

void SunCreation();

void Init_Structs();


#endif // !VISUALS_H