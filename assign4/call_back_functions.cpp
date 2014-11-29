


#include "call_back_functions.h"
#include "camera.h"

/**************************************************************/
// DEFINE

#define NORMAL_SPEED 3
#define MAX_SPEED 5

extern camera cam;
extern float angle;
extern int refractFlag;

// Any time the window is resized, this function gets called.  It's setup to the
// "glutReshapeFunc" in main.
void changeViewport(int w, int h)
{
	glViewport(0, 0, w, h);
}


// This one is the keyboard func (clearly) which moves the camera using WASD
// Note: 033 is an "octal" number and is the same thing as Escape, so you could call "exit" on that
void keyboardFun(unsigned char key, int x, int y)
{
	switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;
		case 'w':
			cam.moveForward(NORMAL_SPEED);
			break;
		case 'W':
			cam.moveForward(MAX_SPEED);
			break;
		case 's':
			cam.moveForward(-NORMAL_SPEED);
			break;
		case 'S':
			cam.moveForward(-MAX_SPEED);
			break;
		case 'a':
			cam.yaw((float) 2);
			break;
		case 'A':
			cam.yaw(1);
			break;
		case 'D':
			cam.yaw(-1);
			break;
		case 'd':
			cam.yaw((float) -2);
			break;
		case 'z':
			cam.zoomIn();
			break;
		case 'Z':
			cam.zoomOut();
			break;
		case 'r':
			angle += 2.0;
			break;
		case 'R':
			angle -= 2.0;
			break;
		case 't':
			refractFlag = (refractFlag == 0) ? 1 : 0;
			break;
	}
}

void specialKeyboardFun(int key, int x, int y)
{
	switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;
		case GLUT_KEY_LEFT:
			cam.roll((float) .2);
			break;
		case GLUT_KEY_UP:
			cam.pitch((float) .2);
			break;
		case GLUT_KEY_RIGHT:
			cam.roll((float) -.2);
			break;
		case GLUT_KEY_DOWN:
			cam.pitch((float) -.2);
			break;
	}
}





void setCallbackFun()
{
	//glutMouseFunc(mouseReader);
	glutKeyboardFunc(keyboardFun);
	glutSpecialFunc(specialKeyboardFun);
	//glutPassiveMotionFunc(look);
	//glutMotionFunc(look);
}
