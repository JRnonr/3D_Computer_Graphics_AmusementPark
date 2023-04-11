/* Operations:
Click the left mouse button, the will switch from day to dusk.
Click the right mouse button, the will switch from dusk to day.
Press a, the camera will turn left.
Press d, the camera will turn right.
Press w, the camera will move Up.
Press a, the camera will move down.
Press e, the camera will be zoomed in.
Press f, the camera will be zoomed out.
Press m, the Ferris wheel will accelerate rotation.
Press n, the Ferris wheel will slow down the rotation.
Press r, the rotation of the Ferris wheel will stop.
Press t, the rotation of the Ferris wheel will restart.
Press c, the Ferris wheel will change the rotation direction.
Press x, the rotation of the carousel will stop.
Press y, the rotation of the carousel will restart.
Press i, the cars will stop moving.
Press j, the cars will restart moving.
Press h, the cars will change the moving direction.
Press k, the lights will stop flashing.
Press l, the lights will restart flashing.
Press q, the program will quit.
*/
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;
int intWinWidth = 800; //Default window size
int intWinHeight = 600;
float fltFOV = 70; //Field Of View
float fltZoom = 1.0; //Zoom amount
float fltX0 = 0.0; //Camera position
float fltY0 = 150.0;
float fltZ0 = 500.0;
float fltXRef = 0.0; //Look At reference point
float fltYRef = 0.0;
float fltZRef = 0.0;
float fltXUp = 0.0; //Up vector
float fltYUp = 1.0;
float fltZUp = 0.0;
float fltViewingAngle = 0; //Used for rotating camera
const GLdouble PI = 3.1415926;
double cameraD = 10;
vector<GLubyte*> p;
GLuint texture[12];void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}
float ang = 0;//Ferris wheel rotate
float step = 0.1;
int time_interval = 10; // declare refresh interval in ms
void OnTimer(int value)
{	
	ang = ang + step;
	when_in_mainloop();
	glutTimerFunc(time_interval, OnTimer, 1);
}
float dis1 = 0;
GLfloat stepd = 6;
int time_interval_CarRight = 35;
void OnTimer_CarRight(int value) {
	dis1 += stepd;
	if (dis1 > 600)
		dis1 = -600;
	else if (dis1 < -600)
		dis1 = 600;
	glutTimerFunc(time_interval_CarRight, OnTimer_CarRight, 1);
}
float dis2 = 0;
int time_interval_CarLeft = 35;
void OnTimer_CarLeft(int value) {
	dis2 -= stepd;
	if (dis2 > 600)
		dis2 = -600;
	else if (dis2 < -600)
		dis2 = 600;
	glutTimerFunc(time_interval_CarLeft, OnTimer_CarLeft, 1);
}
int time_interval_Light = 200;
float step3 = 0.02;
float colorLight[] = { 0.2,0.1,0.2 };
void OnTimer_Light(int value) {
	colorLight[0] += step3+0.02;
	colorLight[1] += step3;
	colorLight[2] += step3+0.02;
	if (colorLight[0]>0.8) {
		colorLight[0] = 0.3;
	}
	else if (colorLight[1] > 0.6) {
		colorLight[1] = 0.2;
	}
	else if (colorLight[2]>0.8) {
		colorLight[2] = 0.1;
	}
	glutTimerFunc(time_interval_Light, OnTimer_Light, 1);
}
float angCC = 0;//carousel rotate
float stepCCar = PI / 360;
int time_interval_CC = 80;
void OnTimer_CC(int value)
{
	angCC = angCC + stepCCar;
	when_in_mainloop();
	glutTimerFunc(time_interval_CC, OnTimer_CC, 1);
}
struct image {
	GLint imageWidth;
	GLint imageHeight;
	GLint pixelLength;
};
image loadTexture[10];
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
	GLubyte* pixeldata;
	FILE* pfile;
	fopen_s(&pfile, path, "rb");
	if (pfile == 0) exit(0);
	fseek(pfile, 0x0012, SEEK_SET);
	fread(&imagewidth, sizeof(imagewidth), 1, pfile);
	fread(&imageheight, sizeof(imageheight), 1, pfile);
	pixellength = imagewidth * 3;
	while (pixellength % 4 != 0)
		pixellength++;
	pixellength *= imageheight;
	pixeldata = (GLubyte*)malloc(pixellength);
	if (pixeldata == 0)
		exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(pixeldata, pixellength, 1, pfile);
	p.push_back(pixeldata);
	fclose(pfile);
}
void myinit() {
	const char* pictures[10] = {  // Define a char pointer for the file name to be used 
		"grass.bmp",
		"sky.bmp",
		"Pleft.bmp",
		"Pright.bmp",
		"road.bmp",
		"wallleft.bmp",
		"wallright.bmp",
		"stripe1.bmp",
		"stripe2.bmp",
		"dusk.bmp"
	};
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i <10; i++) {
		ReadImage(pictures[i], loadTexture[i].imageWidth, loadTexture[i].imageHeight, loadTexture[i].pixelLength);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texture[i]);     // Creat texture for the number i texture
		glBindTexture(GL_TEXTURE_2D, texture[i]);    // bind the coresponding texture to the unique ID
		glTexImage2D(GL_TEXTURE_2D, 0, 3, loadTexture[i].imageWidth, loadTexture[i].imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[i]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // Apply GL_MODULATE to combine the texture with material color
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	}
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
float dark = 0.0;
void light() {
	GLfloat lmodel_ambient[] = { 0.7 - dark, 0.7 - dark, 0.7 - dark, 0.3 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}
//two pillars
void DrawPillar1() {
	glPushMatrix();
	glTranslatef(-100.0, 5.0, -150.0);
	glScalef(40.0, 400.0, 40.0);
	glPushMatrix();
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(100.0, 5.0, -150.0);
	glScalef(40.0, 400.0, 40.0);
	glPushMatrix();
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
}
//map two pillars
void DrawPillarPic() {
	glPushMatrix();
	glTranslatef(-100.0, 5.0, -128.0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-20, 200, 0);
	glTexCoord2f(1, 0); glVertex3d(20, 200, 0);
	glTexCoord2f(1, 1); glVertex3d(20, -5, 0);
	glTexCoord2f(0, 1); glVertex3d(-20, -5, 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);


	glPopMatrix();


	glPushMatrix();
	glTranslatef(100.0, 5.0, -128.0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-20, 200, 0);
	glTexCoord2f(1, 0); glVertex3d(20, 200, 0);
	glTexCoord2f(1, 1); glVertex3d(20, -5, 0);
	glTexCoord2f(0, 1); glVertex3d(-20, -5, 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);


	glPopMatrix();


}
//enclosing wall
void DrawPillar2() {
	//front
	glPushMatrix();
	glTranslatef(550.0, 5.0, -150.0);
	glScalef(870.0, 130.0, 40.0);
	glPushMatrix();
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-550.0, 5.0, -150.0);
	glScalef(870.0, 130.0, 40.0);
	glPushMatrix();
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//two sides
	glPushMatrix();
	glTranslatef(-1000.0, 5.0, -1030.0);
	glScalef(40.0, 130.0, 1800.0);
	glPushMatrix();
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1000.0, 5.0, -1030.0);
	glScalef(40.0, 130.0, 1800.0);
	glPushMatrix();
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//behind
	glPushMatrix();
	glTranslatef(0.0, 5.0, -1950.0);
	glScalef(2050.0, 130.0, 40.0);
	glPushMatrix();
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

}
//map the wall
void wallPic0() {
	//sides
	glPushMatrix();
	glTranslatef(0.0, 5.0, -200.0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(0, 63, -1750);
	glTexCoord2f(1, 0); glVertex3d(0, 63, 68);
	glTexCoord2f(1, 1); glVertex3d(0, -5, 68);
	glTexCoord2f(0, 1); glVertex3d(0, -5, -1750);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
//front out
void wallpic1() {
	glPushMatrix();
	glTranslatef(0.0, 5.0, -128.0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-1020, 63, 0);
	glTexCoord2f(1, 0); glVertex3d(-121, 63, 0);
	glTexCoord2f(1, 1); glVertex3d(-121, -5, 0);
	glTexCoord2f(0, 1); glVertex3d(-1020, -5, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 5.0, -128.0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(121, 63, 0);
	glTexCoord2f(1, 0); glVertex3d(1020, 63, 0);
	glTexCoord2f(1, 1); glVertex3d(1020, -5, 0);
	glTexCoord2f(0, 1); glVertex3d(121, -5, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
//behind in
void wallpic2() {
	glPushMatrix();
	glTranslatef(0.0, 5.0, -1925.0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-1020, 63, 0);
	glTexCoord2f(1, 0); glVertex3d(0, 63, 0);
	glTexCoord2f(1, 1); glVertex3d(0, -5, 0);
	glTexCoord2f(0, 1); glVertex3d(-1020, -5, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(0, 63, 0);
	glTexCoord2f(1, 0); glVertex3d(1020, 63, 0);
	glTexCoord2f(1, 1); glVertex3d(1020, -5, 0);
	glTexCoord2f(0, 1); glVertex3d(0, -5, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void wallPic() {
	//front out
	wallpic1();
	//front in
	glPushMatrix();
	glTranslatef(0.0, 0.0, -44.0);
	wallpic1();
	glPopMatrix();

	//behind in
	wallpic2();
	//behind out
	glPushMatrix();	
	glTranslatef(0.0, 0.0, -44.0);
	wallpic2();
	glPopMatrix();

	//side
	glPushMatrix();
	glTranslatef(-1022.0, 0.0, 0.0);
	wallPic0();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-978.0, 0.0, 0.0);
	wallPic0();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1022.0, 0.0, 0.0);
	wallPic0();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(978.0, 0.0, 0.0);
	wallPic0();
	glPopMatrix();
}
//small pillar
void DrawPillar3()
{
	for (int i = 0; i <= 800; i = i + 100) {
		//left front
		glPushMatrix();
		glTranslatef(-200.0 - i, 20.0, -150.0);
		glScalef(10.0, 400.0, 10.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();

		//left front top
		glPushMatrix();
		glTranslatef(-200.0 - i, 215.0, -150.0);
		glRotatef(45, 0, 1, 0);
		glScalef(8.0, 30.0, 8.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidOctahedron();
		glColor3f(0.0, 0.0, 0.0);
		glutWireOctahedron();
		glPopMatrix();
		glPopMatrix();

	}
	for (int i = 0; i <= 800; i = i + 100) {
		//rignt front 
		glPushMatrix();
		glTranslatef(200.0 + i, 20.0, -150.0);
		glScalef(10.0, 400.0, 10.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();

		//right front top
		glPushMatrix();
		glTranslatef(200.0 + i, 215.0, -150.0);
		glRotatef(45, 0, 1, 0);
		glScalef(8.0, 30.0, 8.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidOctahedron();
		glColor3f(0.0, 0.0, 0.0);
		glutWireOctahedron();
		glPopMatrix();
		glPopMatrix();
	}


	for (int i = 0; i <2000; i = i + 100) {
		//left
		glPushMatrix();
		glTranslatef(-1000.0, 20.0, -150.0-i);
		glScalef(10.0, 400.0, 10.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();

		//left top
		glPushMatrix();
		glTranslatef(-1000.0, 215.0, -150.0-i);
		glRotatef(45, 0, 1, 0);
		glScalef(8.0, 30.0, 8.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidOctahedron();
		glColor3f(0.0, 0.0, 0.0);
		glutWireOctahedron();
		glPopMatrix();
		glPopMatrix();

	}

	for (int i = 0; i < 2000; i = i + 100) {
		//right
		glPushMatrix();
		glTranslatef(1000.0, 20.0, -150.0 - i);
		glScalef(10.0, 400.0, 10.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();

		//right top
		glPushMatrix();
		glTranslatef(1000.0, 215.0, -150.0 - i);
		glRotatef(45, 0, 1, 0);
		glScalef(8.0, 30.0, 8.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidOctahedron();
		glColor3f(0.0, 0.0, 0.0);
		glutWireOctahedron();
		glPopMatrix();
		glPopMatrix();

	}

	for (int i = 0; i < 2000; i = i + 100) {
		//behind 
		glPushMatrix();
		glTranslatef(1000.0-i, 20.0, -150.0-1800);
		glScalef(10.0, 400.0, 10.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();

		//behind top
		glPushMatrix();
		glTranslatef(1000.0-i, 215.0, -150.0 - 1800);
		glRotatef(45, 0, 1, 0);
		glScalef(8.0, 30.0, 8.0);
		glPushMatrix();
		glColor3f(0.57, 0.54, 0.46);
		glutSolidOctahedron();
		glColor3f(0.0, 0.0, 0.0);
		glutWireOctahedron();
		glPopMatrix();
		glPopMatrix();

	}


}
void DrawRail() {
	//right front
	for (int i = 0; i <= 100; i = i + 30) {
		glColor3ub(138, 89, 105);
		glPushMatrix();
		glTranslatef(110, 100+i, -150);
		glRotatef(90, 0, 1, 0);
		glScalef(3, 3.3, 11);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		GLUquadricObj* cyliner = gluNewQuadric(); 
		gluQuadricDrawStyle(cyliner, GLU_FILL);
		gluQuadricNormals(cyliner, GLU_SMOOTH);
		gluQuadricTexture(cyliner, GL_TRUE);
		gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
		gluDeleteQuadric(cyliner);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	//left front
	for (int i = 0; i <= 100; i = i + 30) {
		glColor3ub(138, 89, 105);
		glPushMatrix();
		glTranslatef(-110, 100 + i, -150);
		glRotatef(-90, 0, 1, 0);
		glScalef(3, 3.3, 11);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		GLUquadricObj* cyliner = gluNewQuadric();
		gluQuadricDrawStyle(cyliner, GLU_FILL);
		gluQuadricNormals(cyliner, GLU_SMOOTH);
		gluQuadricTexture(cyliner, GL_TRUE);
		gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
		gluDeleteQuadric(cyliner);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	//right 
	for (int i = 0; i <= 100; i = i + 30) {
		glColor3ub(138, 89, 105);
		glPushMatrix();
		glTranslatef(1000, 100 + i, -1950);
		glScalef(3, 3.3, 22.5);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		GLUquadricObj* cyliner = gluNewQuadric(); 
		gluQuadricDrawStyle(cyliner, GLU_FILL);
		gluQuadricNormals(cyliner, GLU_SMOOTH);
		gluQuadricTexture(cyliner, GL_TRUE);
		gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
		gluDeleteQuadric(cyliner);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	//left
	for (int i = 0; i <= 100; i = i + 30) {
		glColor3ub(138, 89, 105);
		glPushMatrix();
		glTranslatef(-1000, 100 + i, -1950);
		glScalef(3, 3.3, 22.5);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		GLUquadricObj* cyliner = gluNewQuadric();
		gluQuadricDrawStyle(cyliner, GLU_FILL);
		gluQuadricNormals(cyliner, GLU_SMOOTH);
		gluQuadricTexture(cyliner, GL_TRUE);
		gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
		gluDeleteQuadric(cyliner);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	//behind
	for (int i = 0; i <= 100; i = i + 30) {
		glColor3ub(138, 89, 105);
		glPushMatrix();
		glTranslatef(1000, 100 + i, -150-1800);
		glRotatef(-90, 0, 1, 0);
		glScalef(3, 3.3, 25);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		GLUquadricObj* cyliner = gluNewQuadric(); 
		gluQuadricDrawStyle(cyliner, GLU_FILL);
		gluQuadricNormals(cyliner, GLU_SMOOTH);
		gluQuadricTexture(cyliner, GL_TRUE);
		gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
		gluDeleteQuadric(cyliner);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

}
double ch1 = 0;
double ch2 = 0;
//map sky and grass
void DrawBackground()
{
	//grass
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(3000, 0, 3000);
	glTexCoord2f(1, 0); glVertex3d(-3000, 0, 3000);
	glTexCoord2f(1, 1); glVertex3d(-3000, 0, -2000);
	glTexCoord2f(0, 1); glVertex3d(3000, 0, -2000);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//day
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-2000, -400, -2000+ch1);
	glTexCoord2f(1, 0); glVertex3d(2000, -400, -2000+ch1);
	glTexCoord2f(1, 1); glVertex3d(2000, 1600, -2000+ch1);
	glTexCoord2f(0, 1); glVertex3d(-2000, 1600, -2000+ch1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//dusk
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-2000, -400, -2010+ch2);
	glTexCoord2f(1, 0); glVertex3d(2000, -400, -2010+ch2);
	glTexCoord2f(1, 1); glVertex3d(2000, 1600, -2010+ch2);
	glTexCoord2f(0, 1); glVertex3d(-2000, 1600, -2010+ch2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//ferris wheel zhijia
void DrawZhijia()//two pairs of pillars and one pillar in the middle
{
	float r1[4] = { 1.97f, 0.0f, 0.0f ,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, r1);
	
	glPushMatrix();
	glTranslatef(0, 0, -1650+600);
	
	//front right
	glPushMatrix();
	glTranslatef(250.0, 0, 50.0);
	glRotatef(90, 1, 0, 0);
	glRotatef(30, 0, 1, 0);
	glScalef(40, 40, 1000);
	glColor3f(0.5,0.6,0.82);
	glutSolidCube(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	//front left
	glPushMatrix();
	glTranslatef(-250.0, 0, 50.0);
	glRotatef(90, 1, 0, 0);
	glRotatef(-30, 0, 1, 0);
	glScalef(40, 40, 1000);
	glColor3f(0.5, 0.6, 0.82);
	glutSolidCube(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	//behind right
	glPushMatrix();
	glTranslatef(250.0, 0, -50.0);
	glRotatef(90, 1, 0, 0);
	glRotatef(30, 0, 1, 0);
	glScalef(40, 40, 1000);
	glColor3f(0.5, 0.6, 0.82);
	glutSolidCube(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	//behind left
	glPushMatrix();
	glTranslatef(-250.0, 0, -50.0);
	glRotatef(90, 1, 0, 0);
	glRotatef(-30, 0, 1, 0);
	glScalef(40, 40, 1000);
	glColor3f(0.5, 0.6, 0.82);
	glutSolidCube(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	//pillar in the middle
	glPushMatrix();
	glTranslatef(0.0, 420.0, 0.0);
	glRotatef(90, 1, 0, 0);
	glScalef(20.0, 60.0, 20.0);
	glPushMatrix();
	glColor3f(0.5,0.6,0.82);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
//front lights
void DrawLight1()
{
	glPushMatrix();
	glTranslatef(0, 420, -1650+600);
	glRotatef(ang, 0, 0, 1);
	
	for (int i = 0; i < 12; i++) {//the number of the light bars
		
		glRotatef(30, 0, 0, 1);
		for (int j = 0; j < 15; j++) {//the number of the light balls
			glPushMatrix();
			glRotatef(5, 1, 0, 0);
			glTranslatef(0, j * 20, 0);
			glScalef(1, 1, 1);
			glPushMatrix();
			//glColor3f(1,1,0);
			glColor3f(colorLight[0]+ (j / 3) * 0.1, colorLight[1]+ (j / 3) * 0.1, colorLight[2] + (j / 3) * 0.1);
			glutSolidSphere(10, 10, 10);
			glColor3f(0.0, 0.0, 0.0);
			glutWireSphere(5, 10, 10);
			glPopMatrix();
			glPopMatrix();
		}

	}
	glPopMatrix();
}
//behind lights
void DrawLight2()
{
	glPushMatrix();
	glTranslatef(0, 420, -1650+600);
	glRotatef(ang, 0, 0, 1);
	for (int i = 0; i < 12; i++) {//the number of the light bars
		glRotatef(30, 0, 0, 1);
		for (int j = 0; j < 15; j++) {//the number of the light balls
			glPushMatrix();
			glRotatef(-5, 1, 0, 0);
			glTranslatef(0, j * 20, 0);
			glScalef(1, 1, 1);
			glPushMatrix();
			/*glColor3f(1,1,0);*/
			glColor3f(colorLight[0] + (j / 5) * 0.1, colorLight[1] + (j / 5) * 0.1, colorLight[2] + (j / 5) * 0.1);
			glutSolidSphere(10, 10, 10);
			glColor3f(0.0, 0.0, 0.0);
			glutWireSphere(5, 10, 10);
			glPopMatrix();
			glPopMatrix();
		}
	}
	glPopMatrix();
}
//pillar between light bars
void DrawSlash() {
	glPushMatrix();

	glTranslatef(0, 420, -1650+600);
	glRotatef(ang, 0, 0, 1);


	for (int i = 0; i < 12; i++) {//the number of light bars

		glRotatef(30, 0, 0, 1);
		glPushMatrix();
		glTranslatef(0.0, 290.0, 0.0);
		glRotatef(90, 1, 0, 0);
		glScalef(8.0, 45.0, 8.0);
		glPushMatrix();
		glColor3f(0.39, 0.49, 0.72);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
	}
	
	

	
	glPopMatrix();
}
float colorCabin[3];
//ferris wheel cars
void DrawCabin(int i) {
	//body
	glPushMatrix();
	glTranslatef(0,5-55,0);
	glScalef(40.0, 35.0, 40.0);
	glPushMatrix();

	if (i % 3 == 0) {
		colorCabin[0] = 0.39,
		colorCabin[1] = 0.49,
		colorCabin[2] = 0.72;
	}
	else if (i % 3 == 1) {
		colorCabin[0] = 0.3,
		colorCabin[1] = 0.3,
		colorCabin[2] = 0.8;
	}
	else if (i % 3 == 2) {
		colorCabin[0] = 0.6,
		colorCabin[1] = 0.6,
		colorCabin[2] = 0.5;
	}
	glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
	//four small pillar
		//front left
	glPushMatrix();
	glTranslatef(-17.5, 32.5-55, 17.5);
	glScalef(5.0, 25.0, 5.0);
	glPushMatrix();
	glColor3f(0.39, 0.49, 0.72);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
		//front right
	glPushMatrix();
	glTranslatef(17.5, 32.5-55, 17.5);
	glScalef(5.0, 25.0, 5.0);
	glPushMatrix();
	glColor3f(0.39, 0.49, 0.72);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
		//behind left
	glPushMatrix();
	glTranslatef(-17.5, 32.5-55, -17.5);
	glScalef(5.0, 25.0, 5.0);
	glPushMatrix();
	glColor3f(0.39, 0.49, 0.72);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
		//behind right
	glPushMatrix();
	glTranslatef(17.5, 32.5-55, -17.5);
	glScalef(5.0, 25.0, 5.0);
	glPushMatrix();
	glColor3f(0.39, 0.49, 0.72);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
	

	//car top
		//left
	glPushMatrix();
	glTranslatef(-16.1, 48-55, 0);
	glRotatef(-43.0, 0, 0, 1);
	glScalef(5.0, 52, 40);
	glPushMatrix();
	glColor3f(0.39, 0.49, 0.72);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
		//right
	glPushMatrix();
	glTranslatef(16.1, 48-55, 0);
	glRotatef(43.0, 0, 0, 1);
	glScalef(5.0, 52, 40);
	glPushMatrix();
	glColor3f(0.39, 0.49, 0.72);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();



}
void DrawCabinRotate() {
	double b = PI / 6;

	for (int i = 0; i < 12; i++) {//the number of light bars
		glPushMatrix();
		glTranslatef(0, 420.0, -1650+600);
		glRotatef(ang, 0, 0, 1);
		glTranslatef(0 - 290 * sin(b*i), 290 * cos(b*i), 0);
		glRotatef(-ang, 0, 0, 1);
		glScalef(0.4, 0.4, 0.4);
		DrawCabin(i);
		glPopMatrix();
	}
}
//the wheel
void DrawWheel() {
	glPushMatrix();
	glTranslatef(0, 420, -1650+600);
	glRotatef(ang, 0, 0, 1);

	for (int i = 0; i < 12; i++) {//the number of light bars

		if (i % 3 == 0) {
			colorCabin[0] = 0.39,
				colorCabin[1] = 0.49,
				colorCabin[2] = 0.72;
		}
		else if (i % 3 == 1) {
			colorCabin[0] = 0.3,
				colorCabin[1] = 0.3,
				colorCabin[2] = 0.8;
		}
		else if (i % 3 == 2) {
			colorCabin[0] = 0.6,
				colorCabin[1] = 0.6,
				colorCabin[2] = 0.5;
		}

		glRotatef(30, 0, 0, 1);
		glPushMatrix();
		glTranslatef(0, -20, 0);
		glScalef(0.3, 0.3, 0.3);
		//body
		glPushMatrix();
		glTranslatef(0, 5, 0);
		glScalef(40.0, 35.0, 40.0);
		glPushMatrix();
		glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
		//four pillars
			//front left
		glPushMatrix();
		glTranslatef(-17.5, 32.5, 17.5);
		glScalef(5.0, 25.0, 5.0);
		glPushMatrix();
		glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
		//front right
		glPushMatrix();
		glTranslatef(17.5, 32.5, 17.5);
		glScalef(5.0, 25.0, 5.0);
		glPushMatrix();
		glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
		//behind left
		glPushMatrix();
		glTranslatef(-17.5, 32.5, -17.5);
		glScalef(5.0, 25.0, 5.0);
		glPushMatrix();
		glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
		//behind right
		glPushMatrix();
		glTranslatef(17.5, 32.5, -17.5);
		glScalef(5.0, 25.0, 5.0);
		glPushMatrix();
		glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
		//top
			//left
		glPushMatrix();
		glTranslatef(-16.1, 48, 0);
		glRotatef(-43.0, 0, 0, 1);
		glScalef(5.0, 52, 40);
		glPushMatrix();


		glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
		//right
		glPushMatrix();
		glTranslatef(16.1, 48, 0);
		glRotatef(43.0, 0, 0, 1);
		glScalef(5.0, 52, 40);
		glPushMatrix();
		glColor3f(colorCabin[0], colorCabin[1], colorCabin[2]);
		glutSolidCube(1);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1);
		glPopMatrix();
		glPopMatrix();


	}


	glPopMatrix();
}
//map the road
void roadPic() {
	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTranslatef(0, 10, 100);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(1000, 0, -150);
	glTexCoord2f(1, 0); glVertex3d(-1000, 0, -150);
	glTexCoord2f(1, 1); glVertex3d(-1000, 0, 0);
	glTexCoord2f(0, 1); glVertex3d(1000, 0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawTyre() {
	glBegin(GL_POLYGON);
	glLineWidth(5);
	for (int i = 0; i < 360; i++)
	{
		glVertex3f(8 * cos(2 * PI * i / 360), 8 * sin(2 * PI * i / 360), 0);
	}
	glEnd();
}
//close to the screen-right direction
void DrawTyresRight() {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(20, 20, 75);
	drawTyre();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(20, 20, 50);
	drawTyre();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0, 20, 50);
	drawTyre();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0, 20, 75);
	drawTyre();
	glPopMatrix();
}
//far away from the screen-left direction
void DrawTyresLeft() {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(-40, 20, -15);
	drawTyre();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(-40, 20, 10);
	drawTyre();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(-60, 20, -15);
	drawTyre();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(-60, 20, 10);
	drawTyre();
	glPopMatrix();
}
void carRight1() {
	glPushMatrix();
	glTranslatef(dis1, 0, 10);

	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10,30,62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(254.0 / 255.0, 235.0 / 255.0, 105.0 / 255.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 45, 62.5);
	glScalef(19.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(1, 97/255, 0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carRight2() {
	glPushMatrix();
	glTranslatef(dis1-130, 0, 10);

	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10, 30, 62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(0.4, 0.59, 1);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carRight3() {
	glPushMatrix();
	glTranslatef(dis1+100, 0, 10);

	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10, 30, 62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(1, 0.6117, 0.506);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 45, 62.5);
	glScalef(19.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(dis1 + 120, 0, 10);

	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10, 30, 62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(1, 0.6117, 0.506);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 45, 62.5);
	glScalef(19.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carRight4() {
	glPushMatrix();
	glTranslatef(dis1 +230, 0, 10);
	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10, 30, 62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(0.4, 0.59, 1);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carRight5() {
	glPushMatrix();
	glTranslatef(dis1+320, 0, 10);

	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10, 30, 62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(254.0 / 255.0, 235.0 / 255.0, 105.0 / 255.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 45, 62.5);
	glScalef(19.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(1, 97 / 255, 0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carRight6() {
	glPushMatrix();
	glTranslatef(dis1 - 260, 0, 10);

	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10, 30, 62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(1, 0.6117, 0.506);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 45, 62.5);
	glScalef(19.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(dis1 + 120, 0, 10);

	DrawTyresRight();
	glPushMatrix();
	glTranslatef(10, 30, 62.5);
	glScalef(40.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(1, 0.6117, 0.506);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 45, 62.5);
	glScalef(19.0, 15.0, 30.0);
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carLeft1() {
	glPushMatrix();
	glTranslatef(dis2, 0, -10);

	DrawTyresLeft();
	glPushMatrix();
	glTranslatef(-50, 35, -2.5);
	glScalef(40.0, 25.0, 35.0);
	glPushMatrix();
	glColor3f(0.9, 0.6, 0.3);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carLeft2() {
	glPushMatrix();
	glTranslatef(dis2+260, 0, -10);

	DrawTyresLeft();
	glPushMatrix();
	glTranslatef(-50, 35, -2.5);
	glScalef(40.0, 25.0, 35.0);
	glPushMatrix();
	glColor3f(0.4, 0.8, 0.4);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();


	glPopMatrix();
}
void carLeft3() {
	glPushMatrix();
	glTranslatef(dis2 -160, 0, -10);

	DrawTyresLeft();
	glPushMatrix();
	glTranslatef(-50, 35, -2.5);
	glScalef(40.0, 20.0, 35.0);
	glPushMatrix();
	glColor3f(0.4, 0.8, 0.4);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-41, 55, -2.5);
	glScalef(20.0, 20.0, 30.0);
	glPushMatrix();
	glColor3f(0.5, 0.8, 0.8);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carLeft4() {
	glPushMatrix();
	glTranslatef(dis2+360, 0, -10);

	DrawTyresLeft();
	glPushMatrix();
	glTranslatef(-50, 35, -2.5);
	glScalef(40.0, 25.0, 35.0);
	glPushMatrix();
	glColor3f(0.4, 0.3, 0.8);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void carLeft5() {
	glPushMatrix();
	glTranslatef(dis2 -260, 0, -10);

	DrawTyresLeft();
	glPushMatrix();
	glTranslatef(-50, 35, -2.5);
	glScalef(40.0, 25.0, 35.0);
	glPushMatrix();
	glColor3f(0.2, 0.8, 0.6);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();


	glPopMatrix();
}
void carLeft6() {
	glPushMatrix();
	glTranslatef(dis2 +190, 0, -10);

	DrawTyresLeft();
	glPushMatrix();
	glTranslatef(-50, 35, -2.5);
	glScalef(40.0, 20.0, 35.0);
	glPushMatrix();
	glColor3f(0.3, 0.5, 0.4);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-41, 55, -2.5);
	glScalef(20.0, 20.0, 30.0);
	glPushMatrix();
	glColor3f(0.5, 0.6, 0.7);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void CarouselBottom() {
	glColor3f(0.55, 0.5, 0.5);
	glPushMatrix();
	glTranslatef(0, 15, 300);
	glRotatef(90, 1, 0, 0);
	glScalef(170, 170, 0.3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	GLUquadricObj* cyliner = gluNewQuadric(); 
	gluQuadricDrawStyle(cyliner, GLU_FILL);
	gluQuadricNormals(cyliner, GLU_SMOOTH);
	gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
	gluDeleteQuadric(cyliner);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	GLint r1 = 169;
	GLint x1 = 0;
	GLint y1 = 100;
	glPushMatrix();
	glTranslatef(0, -290+4, 400);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0.55, 0.5, 0.5);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * PI; i = i + PI / 180)
	{
		glVertex3f(r1 * sin(i) + x1, r1 * cos(i) + y1, 300);
	}
	glEnd();
	glPopMatrix();
}
void CarouselPolar() {
	//middle Cylinder
	
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 15, 300);
	glRotatef(-90, 1, 0, 0);
	glScalef(33,33, 3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	GLUquadricObj* cyliner = gluNewQuadric();
	gluQuadricDrawStyle(cyliner, GLU_FILL);
	gluQuadricNormals(cyliner, GLU_SMOOTH);
	gluQuadricTexture(cyliner, GL_TRUE);
	gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
	gluDeleteQuadric(cyliner);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


}
void CarouselTop() {
	//top 
	glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(0, 180, 300);
	glRotatef(-90, 1, 0, 0);
	glScalef(160, 160, 0.5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	GLUquadricObj* cyliner = gluNewQuadric(); 
	gluQuadricDrawStyle(cyliner, GLU_FILL);
	gluQuadricNormals(cyliner, GLU_SMOOTH);
	gluQuadricTexture(cyliner, GL_TRUE);
	gluCylinder(cyliner, 1, 0,250.0, 36, 1);
	gluDeleteQuadric(cyliner);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
float xc = -120;
float zc = 0;
void CarouselBody() {
	glPushMatrix();
	glTranslatef(0, 15, 300);
	glColor3f(0.55,0.5,0.5);

	double b = PI / 6;
	for (int i = 0; i < 12; i++) {
		glPushMatrix();
		glTranslatef(xc*cos(b*i+angCC), 0, zc+xc*sin(b*i+angCC));
		
		glRotatef(-90, 1, 0, 0);
		glScalef(5.5, 5.5, 2.2);
	/*	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[7]);*/
		GLUquadricObj* cyliner = gluNewQuadric(); 
		gluQuadricDrawStyle(cyliner, GLU_FILL);
		gluQuadricNormals(cyliner, GLU_SMOOTH);
		//gluQuadricTexture(cyliner, GL_TRUE);
		gluCylinder(cyliner, 1, 1, 80.0, 36, 1);
		gluDeleteQuadric(cyliner);
		//glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}

	glPopMatrix();

}
//car on Carousel
void carrr() {
	//battom
	glPushMatrix();
	glTranslatef(-13 - 15, 30 - 20-2, 180 - 120);
	glScalef(32, 5, 35);
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	//left
	glPushMatrix();
	glTranslatef(-31 - 15, 43 - 20-2, 180 - 120);
	glScalef(5, 30, 35);
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	//right
	glPushMatrix();
	glTranslatef(5 - 15, 43 - 20-2, 180 - 120);
	glScalef(5, 30, 35);
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	//seats
	glPushMatrix();
	glTranslatef(-20, 37 - 20-2, 180 - 120);
	glScalef(13, 10, 34);
	glColor3f(0.37, 0.4, 0.6);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	//behind
	glPushMatrix();
	glTranslatef(-13 - 15, 40 - 20-2, 160 - 120);
	glScalef(30, 20, 5);
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	//front
	glPushMatrix();
	glTranslatef(-13 - 15, 38 - 20-2, 195 - 120+2);
	glScalef(32, 20, 5);
	glColor3f(0.57, 0.54, 0.46);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
}
void CarouselCar() {

	glPushMatrix();
	glTranslatef(30, 15, 250);

	double b = PI / 6;
	for (int i = 0; i < 12; i++) {
		glPushMatrix();
		glTranslatef(xc * cos(b * i+angCC), 0, zc + xc * sin(b * i+angCC));
		glScalef(0.85, 0.85, 0.85);
		carrr();
		glPopMatrix();
	}

	glPopMatrix();
}
void display()
{
	DrawBackground();
	DrawRail();
	//DrawBoard();
	//boardPic();
	//DrawWord();
	//DrawTree();
	DrawPillar3();
	DrawPillar2();
	DrawPillar1();
	
	//ferris wheel
	glPushMatrix();
	glScalef(1.4, 1.4, 1.4);
	glTranslatef(-350, 0, 300);
	DrawZhijia();
	DrawLight1();
	DrawLight2();
	DrawSlash();
	DrawCabinRotate();
	DrawWheel();
	glPopMatrix();

	//car
	carRight1();
	carRight2();
	carRight3();
	carRight4();
	carRight5();
	carRight6();
	carLeft1();
	carLeft2();
	carLeft3(); 
	carLeft4();
	carLeft5();
	carLeft6();


	roadPic();
	wallPic();
	DrawPillarPic();
	
	//Carousel
	glPushMatrix();
    glScalef(1.6,1.4,1.6);
	glTranslatef(350, 0, -900);
	CarouselBottom();
	CarouselPolar();
	CarouselTop();
	CarouselBody();
	CarouselCar();
	glPopMatrix();

	//light model
	light();
}
void displayObject()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fltFOV, 1, 0.1, 5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(fltX0 * fltZoom, fltY0 * fltZoom, fltZ0 * fltZoom+100, fltXRef, fltYRef, fltZRef, fltXUp, fltYUp, fltZUp);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.2, 0.8, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display();
	glutSwapBuffers();
	glLoadIdentity();
	glFlush();
}
void reshapeWindow(GLint intNewWidth, GLint intNewHeight)
{
	glViewport(0, 0, intNewWidth, intNewHeight);//reshape
}
void keyboard_input(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		fltX0 -= cameraD;
		break;
	case 'd':
		fltX0 += cameraD;
		break;
	case 'w':
		fltY0 += cameraD;
		break;
	case 's':
		fltY0 -= cameraD;
		break;
	case 'e':
		fltZoom -= 0.005;
		break;
	case 'f':
		fltZoom += 0.005;
		break;
	//ferris wheel
	case 'm':
		step += 0.1;// accelerate
		break;
	case 'n':
		step -= 0.1;//decelerate
		break;
	case 'r':
		step = 0;//stop
		break;
	case 't':
		step = 0.1;//restart
		break;
	case 'c':
		step = -step;//change direction
		break;
	//Carousel
	case 'x': //stop
		stepCCar = 0;
		break;
	case 'y'://restart
		stepCCar = PI / 360;
		break;
	//car
	case 'i'://stop
		stepd = 0;
		break;
	case 'j'://restart
		stepd = 6;
		break;
	case 'h'://change the direction
		stepd = -6;
		break;
	//lights
	case 'k'://stop
		step3=0;
		break;
	case 'l'://restart
		step3 = 0.2;
		break;
	case 'q':
		exit(0);
	}
	
	
	glutPostRedisplay();
}
void mouse_input(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON&&dark==0)//->dusk
		ch1 = -15,
		ch2 = 15,
		dark = 0.25;
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && dark == 0.25)//->day
		ch1 = 0,
		ch2 = 0,
		dark = 0.0;

	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(intWinWidth, intWinHeight);
	glutCreateWindow("AMUSEMENT PARK");
	myinit();
	glutKeyboardFunc(keyboard_input);
	glutMouseFunc(mouse_input);
	glutTimerFunc(time_interval, OnTimer, 1);
	glutTimerFunc(time_interval_CarRight, OnTimer_CarRight, 1);
	glutTimerFunc(time_interval_CarLeft, OnTimer_CarLeft, 1);
	glutTimerFunc(time_interval_Light, OnTimer_Light, 1);
	glutTimerFunc(time_interval_CC, OnTimer_CC, 1);
	glutDisplayFunc(displayObject);
	glutReshapeFunc(reshapeWindow);
	glutMainLoop();
}

