#include "visuals.h"

model myModel;
int numOfVerts, numOfNormals, numOfFaces;
float g = 10;
float dt = 0.2;
float dts = 0.1;
float RotateXSelf = 0.0f;
float RotateY = 0.0f;
float RotateZ = 0.0f;
float angleX = 0.0f;
float angleY = 0.0f;
static int howManyStars = 1000;
static float roty = 0.0;
static bool animate = false;
static bool redisplay = false;
GLfloat outerAlpha = 0.0f;
GLfloat starOuterAlpha = 0.0f;
GLdouble maxOuterRadius = 24.0f;
GLdouble maxStarOuterRadius = 0.8f;
GLdouble outerRadius = 24.0f;
GLdouble starOuterRadius = 0.8f;
Positions myPositions[1000];
vector<point> verticesVect;
vector<point> edgesVect;
vector<face> facesVect;
ifstream obj_file;
string GARBAGE;


void Init_Structs()
{
	//Initialize needed structs
	for (int i = 0; i < howManyStars; i++)
	{
		myPositions[i].x_pos = -500 + rand() % howManyStars;
		myPositions[i].y_pos = -500 + rand() % howManyStars;
		myPositions[i].z_pos = -500 + rand() % howManyStars;
	}
}


void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);

	//Stars Creation
	StarsCreation(howManyStars);

	//Sun Creation
	SunCreation();

	//Red planet
	glPushMatrix();
	glTranslatef(0, 0, -100);
	glRotatef(RotateY, 0, 1, 0);
	glTranslatef(0, 0, -50);
	glRotatef(RotateY, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	glColor3f(1.0f, 0.0f, 0.0f);
	DisplayModel(myModel);
	glPopMatrix();

	//Blue little satellite planet
	glPushMatrix();
	glTranslatef(0.0, 0, -50);
	glRotatef(RotateY, 0, 1, 0);
	glTranslatef(0, 0, -30);
	glScalef(0.001, 0.001, 0.001);
	glColor3f(0.0f, 0.0f, 1.0f);
	DisplayModel(myModel);
	glPopMatrix();

	glutSwapBuffers();
}


void Resize(int w, int h)
{
	//Define the visible area of the window in pixels
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);

	//Setup viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);

}


void Idle()
{
	if (animate)
	{
		RotateY += 15;
		RotateXSelf += 15;
		RotateZ += 0.4;
	}

	if (starOuterRadius < 0.0001)
	{
		dts = -dts;
		starOuterRadius = starOuterRadius - dts;
	}
	else
	{
		starOuterRadius = starOuterRadius - dts;
		if (starOuterRadius == maxStarOuterRadius)
		{
			dts = -dts;
		}
	}

	if (outerRadius == 0)
	{
		g = -10;
		outerRadius = outerRadius - g*dt;
	}
	else
	{
		outerRadius = outerRadius - g*dt;

		if (outerRadius == maxOuterRadius)
		{
			g = 10;
		}
	}

	outerAlpha = outerRadius / maxOuterRadius;
	starOuterAlpha = starOuterRadius / maxStarOuterRadius;

	glutPostRedisplay();
}


void Setup()
{

	ReadFile(&myModel);

	//Parameter handling
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  
	glClearDepth(1);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//Set up light source
	GLfloat light_position[] = { 0.0, 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

	//For Gold
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	GLfloat specref[4];
	specref[0] = 0.247; specref[1] = 0.225; specref[2] = 0.065; specref[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
	specref[0] = 0.346; specref[1] = 0.314; specref[2] = 0.090; specref[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
	specref[0] = 0.797; specref[1] = 0.724; specref[2] = 0.208; specref[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMaterialf(GL_FRONT, GL_SHININESS, 83.2);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//BLENDING
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void StarsCreation(int howManyStars)
{
	for (int i = 0; i < howManyStars; i++)
	{
		glPushMatrix();
		glTranslatef(myPositions[i].x_pos, myPositions[i].y_pos, myPositions[i].z_pos);
		glColor3f(1.0, 1.0, 1.0);
		glutSolidSphere(0.50, 10, 5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(myPositions[i].x_pos, myPositions[i].y_pos, myPositions[i].z_pos);
		glColor4f(1, 1, 1, starOuterAlpha);
		glutSolidSphere(starOuterRadius, 10, 5);
		glPopMatrix();
	}
}


void SunCreation()
{
	glPushMatrix();
	glTranslatef(0, 0, -100);
	glColor3f(0.7, 0.6, 0);
	glutSolidSphere(15.0, 30, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -100);
	glColor4f(0.4, 0.3, 0, outerAlpha);
	glutSolidSphere(outerRadius, 30, 24);
	glPopMatrix();

	//glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p': 
		animate = !animate;
		break;
	case 'w' :
		angleX += 1.0f;
		break;
	case 's':
		angleX -= 1.0f;
		break;
	case 'a':
		angleY += 1.0f;
		break;
	case 'd':
		angleY -= 1.0f;
		break;
	default: break;
	}

	glutPostRedisplay();

}


void ReadFile(model *md)
{
	int vx1, vx2, vx3, vn1, vn2, vn3;
	ifstream file;
	string line;
	point vec, norm;
	face tempface;

	file.open("planet.obj");
	if (file.is_open() == true)
	{
		while (getline(file, line))
		{
			if (line.substr(0, 2) == " ")
				continue;
			else if (line.substr(0, 2) == "v ")
			{
				istringstream numbers(line.substr(2));
				numbers >> vec.x >> vec.y >> vec.z;
				verticesVect.push_back(vec);
			}
			else if (line.substr(0, 2) == "vn")
			{
				istringstream numbers(line.substr(2));
				numbers >> norm.x >> norm.y >> norm.z;
				edgesVect.push_back(norm);
			}
			else if (line.substr(0, 2) == "f ")
			{

				face f;
				line = line.substr(2, line.length());
				for (string::iterator it = line.begin(); it != line.end(); ++it)
				{
					if (*it == '/')
					{
						it = line.erase(it);
						*it = ' ';
					}
				}
				istringstream inp(line);
				inp >> f.vtx[0] >> f.nrm[0] >> f.vtx[1] >> f.nrm[1] >> f.vtx[2] >> f.nrm[2];
				facesVect.push_back(f);
			}
			else
				continue;
		}
		file.close();
	}
}


void DisplayModel(model md)
{
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for (vector<face>::const_iterator it = facesVect.begin(); it != facesVect.end(); ++it)
	{
		//glNormal3f(edgesVect[it->nrm[0] - 1].x, edgesVect[it->nrm[0] - 1].y, edgesVect[it->nrm[0] - 1].z);
		glVertex3f(verticesVect[it->vtx[0] - 1].x, verticesVect[it->vtx[0] - 1].y, verticesVect[it->vtx[0] - 1].z);
		//glNormal3f(edgesVect[it->nrm[1] - 1].x, edgesVect[it->nrm[1] - 1].y, edgesVect[it->nrm[1] - 1].z);
		glVertex3f(verticesVect[it->vtx[1] - 1].x, verticesVect[it->vtx[1] - 1].y, verticesVect[it->vtx[1] - 1].z);
		//glNormal3f(edgesVect[it->nrm[2] - 1].x, edgesVect[it->nrm[2] - 1].y, edgesVect[it->nrm[2] - 1].z);
		glVertex3f(verticesVect[it->vtx[2] - 1].x, verticesVect[it->vtx[2] - 1].y, verticesVect[it->vtx[2] - 1].z);
	}
	glEnd();
	glPopMatrix();
}