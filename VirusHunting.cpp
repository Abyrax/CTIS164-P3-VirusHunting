/*********
 CTIS164 - Project
----------
STUDENT : Ahmet Burak Yuksel
SECTION : 3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
1.Spiral Background
2.Pause - Restart Menu
3.Score is changing with every successfull shot
4.Timer - Player has 20 seconds
5.Player can restart game after time finishes
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD1  15 // Period for the animations.
#define TIMER_PERIOD2  1000 // Period for the timers.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define TIMESEC 20  

#define D2R 0.0174532

#define RUN 0
#define GAMEOVER 1
#define PAUSED 2
#define RESTART 3

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int mode = RUN;
int duration = TIMESEC;
int timer = 1;
int shoot = 1;
int vaccineX = -500;
int vaccineY = 0;
float medicineX = -510;
float medicineY = 0;
int score;
int restartSec = 4;
int r[5], g[5], b[5];

//virus locations
int v1x = 470, v1y = -350;
int v2x = 300, v2y = -550;
int v3x = 360, v3y = -650;
int v4x = 430, v4y = -750;
int v5x = 250, v5y = -450;

//redcell locations
int redcellX[10] = { -800,-500,-1100,-710,-550,-850,-1190,-930,-1300,-650 };
int redcellY[10] = { 260,200,140,80,20,-40,-100,-160,-220,-240 };

float A = 100;
float fq = 1;
float C = 0;
float B = 0;
float angle = 0;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}


//drawing vaccine
void vaccine(float x, float y) {
	glColor3f(1, 1, 1);
	glRectf(x, y + 35, x + 5, y + 10);
	glRectf(x, y + 28, x + 40, y + 18);
	glRectf(x + 20, y + 40, x + 30, y + 5);
	glRectf(x + 25, y + 30, x + 70, y + 15);
	glRectf(x + 25, y + 25, x + 90, y + 20);
	glEnd();

	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glRectf(x + 45, y + 28, x + 68, y + 15);
	glEnd();

	glColor3f(0.2f, 0.2f, 0.2f);
	glRectf(x + 3, y + 35, x + 6, y + 10);
	glRectf(x + 20, y + 38, x + 28, y + 7);
	glRectf(x + 70, y + 25, x + 90, y + 20);

	glRectf(x + 35, y + 30, x + 38, y + 25);
	glRectf(x + 40, y + 30, x + 43, y + 20);
	glRectf(x + 45, y + 30, x + 48, y + 25);
	glRectf(x + 50, y + 30, x + 53, y + 20);
	glRectf(x + 55, y + 30, x + 58, y + 25);
	glRectf(x + 60, y + 30, x + 63, y + 20);
	glEnd();
}

//drawing medicine
void medicine(int x, int y) {
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glRectf(x + 80, y + 24, x + 90, y + 22);
	glEnd();
}

//drawing virus
void virus(float x, float y) {
	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y, 30);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + 50);
	glVertex2f(x - 10, y + 20);
	glVertex2f(x + 10, y + 20);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y + 45, 10);
	glEnd();

	glColor3f(0, 1, 0);
	circle(x, y + 45, 5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 10, y - 20);
	glVertex2f(x - 10, y - 20);
	glVertex2f(x, y - 50);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y - 45, 10);
	glEnd();

	glColor3f(0, 1, 0);
	circle(x, y - 45, 5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 50, y);
	glVertex2f(x + 10, y + 10);
	glVertex2f(x + 10, y - 10);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x + 45, y, 10);
	glEnd();

	glColor3f(0, 1, 0);
	circle(x + 45, y, 5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 50, y);
	glVertex2f(x - 10, y + 10);
	glVertex2f(x - 10, y - 10);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x - 45, y, 10);
	glEnd();

	glColor3f(0, 1, 0);
	circle(x - 45, y, 5);
	glEnd();

	glColor3f(0, 1, 0);
	circle(x, y, 25);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y, 23);
	glEnd();

	glColor3f(0, 1, 0);
	circle(x, y, 15);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y, 10);
	glEnd();
}

//drawing small virus
void smallvirus(float x, float y, int r,int g,int b) {
	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y, 15);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + 25);
	glVertex2f(x - 5, y + 10);
	glVertex2f(x + 5, y + 10);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y + 22, 5);
	glEnd();

	glColor3f(r,g,b);
	circle(x, y + 22, 2.5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 5, y - 10);
	glVertex2f(x - 5, y - 10);
	glVertex2f(x, y - 25);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y - 22, 5);
	glEnd();

	glColor3f(r, g, b);
	circle(x, y - 22, 2.5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 25, y);
	glVertex2f(x + 5, y + 5);
	glVertex2f(x + 5, y - 5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x + 22, y, 5);
	glEnd();

	glColor3f(r, g, b);
	circle(x + 22, y, 2.5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 25, y);
	glVertex2f(x - 5, y + 5);
	glVertex2f(x - 5, y - 5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x - 22, y, 5);
	glEnd();

	glColor3f(r, g, b);
	circle(x - 22, y, 2.5);
	glEnd();

	glColor3f(r, g, b);
	circle(x, y, 12.5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y, 11);
	glEnd();

	glColor3f(r, g, b);
	circle(x, y, 7.5);
	glEnd();

	glColor3f(0.1f, 0.5f, 0.1f);
	circle(x, y, 5);
	glEnd();
}

//drawing redcell
void drawredcell(float x, float y) {
	glColor3f(1, 0, 0);
	circle(x, y, 20);
	glEnd();

	glColor4f(1.0f, 0.5f, 0.6f, 0.9f);
	circle(x, y, 18);
	glEnd();

	glColor3f(1, 0, 0);
	circle(x, y, 16);
	glEnd();

	glColor3f(1, 0, 0);
	circle(x + 15, y, 20);
	glEnd();

	glColor4f(1.0f, 0.5f, 0.6f, 0.9f);
	circle(x + 15, y, 18);
	glEnd();

	glColor3f(1, 0, 0);
	circle(x, y, 16);
	glEnd();

	glColor3f(1, 0, 0);
	circle(x + 15, y, 16);
	glEnd();

}

float f(float x) {
	return A * sin((fq * x + C) * D2R) + B;
}

int randomcolor() {
	int num;
	srand(time(0));
	num = rand() % 256;	
	return num;
}

//displaying game
void displayrun() {
	//background
	glColor4f(1.0f, 0.5f, 0.6f, 0.9f);
	circle(0, 0, 1000);
	glEnd();

	//vein design
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (float x = -500; x <= 550; x += 6) {
		for (float y = -350; y <= 300; y += 10)
			glVertex2f(x - 50, f(x) + y);
	}
	glEnd();

	//redcells
	for (int i = 0; i <= 10; i++)
		drawredcell(redcellX[i], redcellY[i]);	
	
	smallvirus(v1x, v1y, r[0], g[0], b[0]);
	smallvirus(v2x, v2y, r[1], g[1], b[1]);
	smallvirus(v3x, v3y, r[2], g[2], b[2]);
	smallvirus(v4x, v4y, r[3], g[3], b[3]);
	smallvirus(v5x, v5y, r[4], g[4], b[4]);

	//smallvirus(v1x, v1y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v2x, v2y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v3x, v3y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v4x, v4y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v5x, v5y, r = randomcolor(), g = randomcolor(), b = randomcolor());

	if (!shoot)
		medicine(medicineX, medicineY);

	medicine(medicineX, medicineY);

	vaccine(vaccineX, vaccineY);


	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, -300, 500, -270);
	glEnd();

	//Mode Status
	glColor3f(1, 1, 1); 
	vprint(-490, -290, GLUT_BITMAP_8_BY_13, "                    Spacebar : Activate Vaccine    F1 : Pause                                            Score:  ^%d^", score);

	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, 270, 500, 300);
	glEnd();

	//Information
	glColor3f(1, 1, 1);
	vprint(-490, 280, GLUT_BITMAP_8_BY_13, "Ahmet Burak Yuksel   Homework #2                                                                       Time:  ^0:%d^", duration);
	 
}

//pausing game
void displaypause() {
	//background
	glColor4f(1.0f, 0.5f, 0.6f, 0.9f);
	circle(0, 0, 1000);
	glEnd();

	//vein design
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (float x = -500; x <= 550; x += 6) {
		for (float y = -350; y <= 300; y += 10)
			glVertex2f(x - 50, f(x) + y);
	}
	glEnd();

	//redcells
	for (int i = 0; i <= 10; i++)
		drawredcell(redcellX[i], redcellY[i]);	

	smallvirus(v1x, v1y, 1, 1, 1);
	smallvirus(v2x, v2y, 0, 0, 1);
	smallvirus(v3x, v3y, 1, 1, 0);
	smallvirus(v4x, v4y, 0, 0, 0);
	smallvirus(v5x, v5y, 0, 1, 0);

	//smallvirus(v1x, v1y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v2x, v2y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v3x, v3y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v4x, v4y, r = randomcolor(), g = randomcolor(), b = randomcolor());
	//smallvirus(v5x, v5y, r = randomcolor(), g = randomcolor(), b = randomcolor());

	if (!shoot)
		medicine(medicineX, medicineY);

	medicine(medicineX, medicineY);

	vaccine(vaccineX, vaccineY);


	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, -300, 500, -270);
	glEnd();

	//Mode Status
	glColor3f(1, 1, 1);
	vprint(-490, -290, GLUT_BITMAP_8_BY_13, "                PRESS F1 TO CONTINUE       PRESS F2 TO RESTART                                           Score:  ^%d^", score);
	
	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, 270, 500, 300);
	glEnd();

	//Information
	glColor3f(1, 1, 1);
	vprint(-490, 280, GLUT_BITMAP_8_BY_13, "Ahmet Burak Yuksel   Homework #2                                                                       Time:  ^0:%d^", duration);
	glColor3f(0.3, 0.3, 0.3);

	glBegin(GL_POLYGON);
	glVertex2f(-100, 20);
	glVertex2f(-100, -30);
	glVertex2f(120, -30);
	glVertex2f(120, 20);
	glEnd();

	glColor3f(0, 1, 0);
	vprint(-20, 5, GLUT_BITMAP_8_BY_13, " PAUSED");
	vprint(-75, -10, GLUT_BITMAP_8_BY_13, " PRESS F1 TO CONTINUE");
	vprint(-70, -25, GLUT_BITMAP_8_BY_13, " PRESS F2 TO RESTART ");
}

void displayreset() {
	//background
	glColor4f(1.0f, 0.5f, 0.6f, 0.9f);
	circle(0, 0, 1000);
	glEnd();

	//vein design
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (float x = -500; x <= 550; x += 6) {
		for (float y = -350; y <= 300; y += 10)
			glVertex2f(x - 50, f(x) + y);
	}
	glEnd();

	//redcells
	for (int i = 0; i <= 10; i++)
		drawredcell(redcellX[i], redcellY[i]);

	smallvirus(v1x, v1y, 1, 1, 1);
	smallvirus(v2x, v2y, 0, 0, 1);
	smallvirus(v3x, v3y, 1, 1, 0);
	smallvirus(v4x, v4y, 0, 0, 0);
	smallvirus(v5x, v5y, 0, 1, 0);

	if (!shoot)
		medicine(medicineX, medicineY);

	medicine(medicineX, medicineY);

	vaccine(vaccineX, vaccineY);


	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, -300, 500, -270);
	glEnd();

	//Mode Status
	glColor3f(1, 1, 1);
	vprint(-490, -290, GLUT_BITMAP_8_BY_13, "                    Spacebar : Activate Vaccine    F1 : Pause                                            Score:  ^%d^", score);

	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, 270, 500, 300);
	glEnd();

	//Information
	glColor3f(1, 1, 1);
	vprint(-490, 280, GLUT_BITMAP_8_BY_13, "Ahmet Burak Yuksel   Homework #2                                                                       Time:  ^0:%d^", duration);

	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POLYGON);
	glVertex2f(-100, 20);
	glVertex2f(-100, -30);
	glVertex2f(120, -30);
	glVertex2f(120, 20);
	glEnd();

	glColor3f(0, 1, 0);
	vprint(-40, 0, GLUT_BITMAP_8_BY_13, " RESTARTING");
	vprint(-20, -20, GLUT_BITMAP_8_BY_13, "00 : 0%d", restartSec);
}

void displayover() {
	//background
	glColor4f(1.0f, 0.5f, 0.6f, 0.9f);
	circle(0, 0, 1000);
	glEnd();

	//vein design
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (float x = -500; x <= 550; x += 6) {
		for (float y = -350; y <= 300; y += 10)
			glVertex2f(x - 50, f(x) + y);
	}
	glEnd();

	//redcells
	for (int i = 0; i <= 10; i++)
		drawredcell(redcellX[i], redcellY[i]);

	smallvirus(v1x, v1y, 1, 1, 1);
	smallvirus(v2x, v2y, 0, 0, 1);
	smallvirus(v3x, v3y, 1, 1, 0);
	smallvirus(v4x, v4y, 0, 0, 0);
	smallvirus(v5x, v5y, 0, 1, 0);

	if (!shoot)
		medicine(medicineX, medicineY);

	medicine(medicineX, medicineY);

	vaccine(vaccineX, vaccineY);


	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, -300, 500, -270);
	glEnd();

	//Mode Status
	glColor3f(1, 1, 1);
	vprint(-490, -290, GLUT_BITMAP_8_BY_13, "                    Spacebar : Activate Vaccine    F1 : Pause                                            Score:  ^%d^", score);
	
	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, 270, 500, 300);
	glEnd();

	//Information
	glColor3f(1, 1, 1);
	vprint(-490, 280, GLUT_BITMAP_8_BY_13, "Ahmet Burak Yuksel   Homework #2                                                                       Time:  ^0:%d^", duration);

	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POLYGON);
	glVertex2f(-100, 20);
	glVertex2f(-100, -30);
	glVertex2f(120, -30);
	glVertex2f(120, 20);
	glEnd();

	glColor3f(0, 1, 0);
	vprint(-30, 5, GLUT_BITMAP_8_BY_13, " GAME OVER");
	vprint(-45, -10, GLUT_BITMAP_8_BY_13, " YOUR SCORE : %d ", score);
	vprint(-70, -23, GLUT_BITMAP_8_BY_13, " PRESS F1 TO RESTART ");
}

//checking hit
int ifHit(int *x, int *y) {
	if (medicineY< *y + 5 && medicineY>*y - 50) {
		if (medicineX < *x + 20 && medicineX > *x - 10) {
			score++;
			*x = (485 + 1 - 250) + 250;
			*y = -400;
			medicineX = 900;
			for (int i = 0; i < 5; i++) {
				r[i] = randomcolor();
				g[i] = randomcolor();
				b[i] = randomcolor();
			}

			return 1;
		}
	}
	return 0;
}

void reset() {
	mode = RUN;
	duration = TIMESEC;
	vaccineX = -500;
	vaccineY = 0;
	medicineX = -510;
	medicineY = 0;
	restartSec = 3;
	timer = 1;
	shoot = 1;
	score = 0;
	v1x = 470, v1y = -350;
	v2x = 400, v2y = -550;
	v3x = 360, v3y = -650;
	v4x = 430, v4y = -750;
	v5x = 250, v5y = -450;
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	switch (mode) {
	case RUN: displayrun();
		break;
	case PAUSED: displaypause();
		break;
	case RESTART: displayreset();
		break;
	case GAMEOVER: displayover();
		break;
	}

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == 32 && shoot && mode == RUN)
		shoot = !shoot;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; {
		if (vaccineY + 50 < WINDOW_HEIGHT / 2) {
			vaccineY += 10;
			medicineY += 10;
		};
	} break;
	case GLUT_KEY_DOWN: down = true; {
		if (vaccineY - 40 > -WINDOW_HEIGHT / 2) {
			vaccineY -= 10;
			medicineY -= 10;
		};
	} break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;

		//pausing game
	case GLUT_KEY_F1: {
		if (mode == RUN) {
			mode = PAUSED;
			timer = 0;
		}
		else if (mode == PAUSED) {
			mode = RUN;
			timer = 1;
		}
		if (mode == GAMEOVER) {
			mode = RESTART;
			timer = 1;
		}
		break;
	}
	case GLUT_KEY_F2: {
		if (mode == PAUSED)
			mode = RESTART;
		break;
	}
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer1(int v) {
	glutTimerFunc(TIMER_PERIOD1, onTimer1, 0);

	if (timer) {
		//redcells
		for (int i = 0; i < 10; ++i) {
			if (redcellX[i] < 520) {
				redcellX[i]++;
			}
			else {
				redcellX[i] = -570;
			}
		}

		//shoot
		if (shoot) {
			medicineY = vaccineY;
		}

		else if (!shoot){
			if (medicineY - 6 < WINDOW_HEIGHT / 2)
				medicineX += 10;
			else
				shoot = !shoot;
		}

		if (medicineX > 505) {
			medicineY = vaccineY;
			medicineX = vaccineX;
			shoot = !shoot;
		}

		//virusses
		if (v1y - 20 < 300) {
			ifHit(&v1x, &v1y);
			v1y += 1;
		}
		else {
			v1x = rand() % ((485 + 1) - 250) + 250;
			v1y = -310;
			r[0] = randomcolor();
			g[0] = randomcolor();
			b[0] = randomcolor();
		}

		if (v2y - 20 < 300) {
			ifHit(&v2x, &v2y);
			v2y += 3;

		}
		else {			
			v2x = rand() % ((485 + 1) - 250) + 250;
			v2y = -310;
			r[1] = randomcolor();
			g[1] = randomcolor();
			b[1] = randomcolor();
		}

		if (v3y - 20 < 300) {
			ifHit(&v3x, &v3y);
			v3y += 2;
		}
		else {			
			v3x = rand() % ((485 + 1) - 250) + 250;
			v3y = -310;
			r[2] = randomcolor();
			g[2] = randomcolor();
			b[2] = randomcolor();
		}

		if (v4y - 20 < 300) {
			ifHit(&v4x, &v4y);
			v4y += 3;
		}
		else {			
			v4x = rand() % ((485 + 1) - 250) + 250;
			v4y = -310;
			r[3] = randomcolor();
			g[3] = randomcolor();
			b[3] = randomcolor();
		}

		if (v5y - 20 < 300) {
			ifHit(&v5x, &v5y);
			v5y += 2;
		}
		else {			
			v5x = rand() % ((485 + 1) - 250) + 250;
			v5y = -310;
			r[4] = randomcolor();
			g[4] = randomcolor();
			b[4] = randomcolor();
		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

#if TIMER_ON == 1
void onTimer2(int v) {
	glutTimerFunc(TIMER_PERIOD2, onTimer2, 0);
	//timer for remaining time
	if (mode == RUN) {
		if (duration > 0)
			duration--;
		else
			mode = GAMEOVER;
	}
	if (mode == RESTART) {
		if (restartSec > 0)
			restartSec--;
		else
			reset();
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Virus Hunting by Ahmet Burak Yuksel");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD1, onTimer1, 0);
	glutTimerFunc(TIMER_PERIOD2, onTimer2, 0);
#endif

	Init();

	glutMainLoop();
}
