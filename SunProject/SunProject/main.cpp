#include "visuals.h"


//**********  STATE VARIABLES  **********
float t = 0.0;

//**********  MAIN PROGRAM  **********
int main(int argc, char* argv[])
{
	//Initialize GLUT library state
	glutInit(&argc, argv);
	cout << "GLUT library initialized successfully!" << endl;
	
	//Setup display
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	
	//Setup window
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Solar System");

	//Configure rendering properties
	Setup();  
	Init_Structs();

	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);

	//Main event handling loop
	glutMainLoop();

	return 0;
}