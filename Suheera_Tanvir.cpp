/*********
 CTIS164
----------
STUDENT : Suheera Tanvir
SECTION : 02
HOMEWORK: 02
----------
PROBLEMS:
Inefficient code. Everything works perfectly but there are separate objects for each apple/object which makes the code long.
----------
ADDITIONAL FEATURES:
1) each object has a different speed. The apples are faster if they have more points. The apples' points
are picked randomly by the program using the random function and the points of each apple influences
the speed of that apple. so apples that are worth 1 move slowest, apples that are worth 8 are the quickest etc.
2) added gamestates. Initial state is the instructions(how to play) and game backstory.
Press s to start or continue game from the how to play gamescreen. press p to pause game and then u to resume.
press h to open instructions/back story game.
3) added LOTS of objects and background to make the game fun and interesting :)
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

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  20 // Setting time period to 20 so that objects move slower by default
#define TIMER_ON      1    // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define HOWTOPLAY 0
#define GAME 1
#define PAUSESCREEN 2

bool showApple = true;  //this is to decide in the drawApple function whether the apples needs to be shown
//the remaining variables are for the other apples
bool showApple2 = true;
bool showApple3 = true;  //these will be set to false when an apple gets hit
bool showApple4 = true;
bool showApple5 = true;

int gameState = HOWTOPLAY;   //initial gamestate is set to Howtoplay and backstory screen

bool up = false, down = false, right = false, left = false, movement = false;

int  winWidth, winHeight; // current Window width and height

int xApple = 145, yApple = 800; //Initial coordinates of the apples
int xApple2 = 278, yApple2 = 1100; //Coordinates for all five of the remaining apples
int xApple3 = 390, yApple3 = 400;
int xApple4 = 270, yApple4 = 550;
int xApple5 = 90, yApple5 = 1000;

int point = 1;   //variables for scores set on apples
int point2 = 2;  //these points will be randomized using the random function
int point3 = 3;
int point4 = 4;
int point5 = 5;

double r = 500, g = 600, b = 300;      //variables for the colors of the apples. apple2, apple3....so on
double r2 = 900, g2 = 100, b2 = 400;   //these variables will be randomized using the random function
double r3 = 770, g3 = 570, b3 = 660;
double r4 = 990, g4 = 330, b4 = 770;
double r5 = 330, g5 = 990, b5 = 660;

int xDoc = -580, yDoc = 250;    //initial coordinates of the doctor

int xBullet = xDoc + 200, yBullet = yDoc;   //initial coordinates of the bullet. It is present right where the pistol is placed

int score = 0;  //variable for calculating the final
double timer = 2000;  //set for 20 seconds. 
bool gameover = false;

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

void bullet()  //function that draws the bullet. It is a black circle.
{
	glColor3f(0, 0, 0);
	circle(xBullet, yBullet, 6);
}

void BulletMovement()   //function that moves the bullet when spacebar is pressed
{
	if (movement == true)
	{
		xBullet += 18;

		if (xBullet >= 700)  //if statement that if bullet goes out of bounds, it is sent back to the pistol
		{
			movement = false;  //bullet movement stopped when bullet goes out of bounds
			xBullet = xDoc + 200;
			yBullet = yDoc;
		}

		//this if statement is to check if the bullet hits an apple. then the apple will disappear(go out of bounds), 
		//bullet will stop and go to the pistol and the score will add
		if (xBullet > xApple - 85 && xBullet < xApple + 85 && yBullet > yApple - 50 && yBullet < yApple + 50)
		{
			movement = false;
			showApple = false;
			yApple = 460;
			score = score + point;
			xBullet = xDoc + 200;
			yBullet = yDoc;
		}

		//same if statement for the remaining apples
		if (xBullet > xApple2 - 85 && xBullet < xApple2 + 85 && yBullet > yApple2 - 50 && yBullet < yApple2 + 50)
		{
			movement = false;
			showApple2 = false;
			yApple2 = 460;
			score = score + point2;
			xBullet = xDoc + 200;
			yBullet = yDoc;
		}

		if (xBullet > xApple3 - 85 && xBullet < xApple3 + 85 && yBullet > yApple3 - 50 && yBullet < yApple3 + 50)
		{
			movement = false;
			showApple3 = false;
			yApple3 = 460;
			score = score + point3;
			xBullet = xDoc + 200;
			yBullet = yDoc;
		}

		if (xBullet > xApple4 - 85 && xBullet < xApple4 + 85 && yBullet > yApple4 - 50 && yBullet < yApple4 + 50)
		{
			movement = false;
			showApple4 = false;
			yApple4 = 460;
			score = score + point4;
			xBullet = xDoc + 200;
			yBullet = yDoc;
		}

		if (xBullet > xApple5 - 85 && xBullet < xApple5 + 85 && yBullet > yApple5 - 50 && yBullet < yApple5 + 50)
		{
			movement = false;
			showApple5 = false;
			yApple5 = 460;
			score = score + point5;
			xBullet = xDoc + 200;
			yBullet = yDoc;
		}

		//to show apples again from the top
		showApple = true;
		showApple2 = true;
		showApple3 = true;
		showApple4 = true;
		showApple5 = true;
	}
}

void drawApples()   //function for making all five apples
{
	srand(time(NULL));   //for the random function

	if (showApple == true)    //if the apple has not been hit by the bullet
	{

		if (yApple < -400 || yApple > 450) //if apple goes out of bounds, randomize the position, points and color variables
		{
			r = (rand() % 1000);
			g = (rand() % 1000);
			b = (rand() % 1000);
			xApple = rand() % 620;
			point = (rand() % 8) + 1;  //generating a random number between 1-8
		}

		/*FIRST APPLE*/    //shape making of the first apple
		//leaf of the apple
		glColor3f(0.2, 0.509, 0.129);
		glBegin(GL_QUADS);
		glVertex2f(xApple - 2, yApple + 60);
		glVertex2f(xApple + 15, yApple + 80);
		glVertex2f(xApple + 65, yApple + 90);
		glVertex2f(xApple + 25, yApple + 60);
		glEnd();

		//Outline of leaf
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple - 2, yApple + 60);
		glVertex2f(xApple + 15, yApple + 80);
		glVertex2f(xApple + 65, yApple + 90);
		glVertex2f(xApple + 25, yApple + 60);
		glEnd();

		//Stem of the apple
		glColor3f(0.509, 0.223, 0.129);
		glBegin(GL_TRIANGLES);
		glVertex2f(xApple + 2, yApple + 15);
		glVertex2f(xApple - 25, yApple + 100);
		glVertex2f(xApple - 5, yApple + 115);
		glEnd();

		//Outline of stem
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple + 2, yApple + 15);
		glVertex2f(xApple - 25, yApple + 100);
		glVertex2f(xApple - 5, yApple + 115);
		glEnd();

		//shape of the apple
		glColor3f(r / 1000, g / 1000, b / 1000);
		glBegin(GL_POLYGON);
		glVertex2f(xApple, yApple + 25);
		glVertex2f(xApple + 15, yApple + 40);
		glVertex2f(xApple + 45, yApple + 40);
		glVertex2f(xApple + 65, yApple + 30);
		glVertex2f(xApple + 75, yApple + 10);
		glVertex2f(xApple + 75, yApple - 10);
		glVertex2f(xApple + 65, yApple - 30);
		glVertex2f(xApple + 45, yApple - 40);
		glVertex2f(xApple + 15, yApple - 40);
		glVertex2f(xApple, yApple - 25);
		glVertex2f(xApple - 15, yApple - 40);
		glVertex2f(xApple - 45, yApple - 40);
		glVertex2f(xApple - 65, yApple - 30);
		glVertex2f(xApple - 75, yApple - 10);
		glVertex2f(xApple - 75, yApple + 10);
		glVertex2f(xApple - 65, yApple + 30);
		glVertex2f(xApple - 45, yApple + 40);
		glVertex2f(xApple - 15, yApple + 40);
		glEnd();

		//Outline of apple
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple, yApple + 25);
		glVertex2f(xApple + 15, yApple + 40);
		glVertex2f(xApple + 45, yApple + 40);
		glVertex2f(xApple + 65, yApple + 30);
		glVertex2f(xApple + 75, yApple + 10);
		glVertex2f(xApple + 75, yApple - 10);
		glVertex2f(xApple + 65, yApple - 30);
		glVertex2f(xApple + 45, yApple - 40);
		glVertex2f(xApple + 15, yApple - 40);
		glVertex2f(xApple, yApple - 25);
		glVertex2f(xApple - 15, yApple - 40);
		glVertex2f(xApple - 45, yApple - 40);
		glVertex2f(xApple - 65, yApple - 30);
		glVertex2f(xApple - 75, yApple - 10);
		glVertex2f(xApple - 75, yApple + 10);
		glVertex2f(xApple - 65, yApple + 30);
		glVertex2f(xApple - 45, yApple + 40);
		glVertex2f(xApple - 15, yApple + 40);
		glEnd();

		glColor3f(0.2, 0.1, 0.2);
		vprint(xApple, yApple, GLUT_BITMAP_HELVETICA_18, "%d", point);
	}


	//same functions and code for the remaining apples

	/*SECOND APPLE*/
	if (showApple2 == true)
	{

		if (yApple2 < -400 || yApple2 > 450)
		{
			r2 = (rand() % 1000);
			g2 = (rand() % 1000);
			b2 = (rand() % 1000);
			xApple2 = rand() % 620;
			point2 = (rand() % 8) + 1;
		}

		//leaf of the apple
		glColor3f(0.2, 0.509, 0.129);
		glBegin(GL_QUADS);
		glVertex2f(xApple2 - 2, yApple2 + 60);
		glVertex2f(xApple2 + 15, yApple2 + 80);
		glVertex2f(xApple2 + 65, yApple2 + 90);
		glVertex2f(xApple2 + 25, yApple2 + 60);
		glEnd();

		//Outline of leaf
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple2 - 2, yApple2 + 60);
		glVertex2f(xApple2 + 15, yApple2 + 80);
		glVertex2f(xApple2 + 65, yApple2 + 90);
		glVertex2f(xApple2 + 25, yApple2 + 60);
		glEnd();

		//Stem of the apple
		glColor3f(0.509, 0.223, 0.129);
		glBegin(GL_TRIANGLES);
		glVertex2f(xApple2 + 2, yApple2 + 15);
		glVertex2f(xApple2 - 25, yApple2 + 100);
		glVertex2f(xApple2 - 5, yApple2 + 115);
		glEnd();

		//Outline of stem
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple2 + 2, yApple2 + 15);
		glVertex2f(xApple2 - 25, yApple2 + 100);
		glVertex2f(xApple2 - 5, yApple2 + 115);
		glEnd();

		//shape of the apple
		glColor3f(r2 / 1000, g2 / 1000, b2 / 1000);
		glBegin(GL_POLYGON);
		glVertex2f(xApple2, yApple2 + 25);
		glVertex2f(xApple2 + 15, yApple2 + 40);
		glVertex2f(xApple2 + 45, yApple2 + 40);
		glVertex2f(xApple2 + 65, yApple2 + 30);
		glVertex2f(xApple2 + 75, yApple2 + 10);
		glVertex2f(xApple2 + 75, yApple2 - 10);
		glVertex2f(xApple2 + 65, yApple2 - 30);
		glVertex2f(xApple2 + 45, yApple2 - 40);
		glVertex2f(xApple2 + 15, yApple2 - 40);
		glVertex2f(xApple2, yApple2 - 25);
		glVertex2f(xApple2 - 15, yApple2 - 40);
		glVertex2f(xApple2 - 45, yApple2 - 40);
		glVertex2f(xApple2 - 65, yApple2 - 30);
		glVertex2f(xApple2 - 75, yApple2 - 10);
		glVertex2f(xApple2 - 75, yApple2 + 10);
		glVertex2f(xApple2 - 65, yApple2 + 30);
		glVertex2f(xApple2 - 45, yApple2 + 40);
		glVertex2f(xApple2 - 15, yApple2 + 40);
		glEnd();

		//Outline of apple
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple2, yApple2 + 25);
		glVertex2f(xApple2 + 15, yApple2 + 40);
		glVertex2f(xApple2 + 45, yApple2 + 40);
		glVertex2f(xApple2 + 65, yApple2 + 30);
		glVertex2f(xApple2 + 75, yApple2 + 10);
		glVertex2f(xApple2 + 75, yApple2 - 10);
		glVertex2f(xApple2 + 65, yApple2 - 30);
		glVertex2f(xApple2 + 45, yApple2 - 40);
		glVertex2f(xApple2 + 15, yApple2 - 40);
		glVertex2f(xApple2, yApple2 - 25);
		glVertex2f(xApple2 - 15, yApple2 - 40);
		glVertex2f(xApple2 - 45, yApple2 - 40);
		glVertex2f(xApple2 - 65, yApple2 - 30);
		glVertex2f(xApple2 - 75, yApple2 - 10);
		glVertex2f(xApple2 - 75, yApple2 + 10);
		glVertex2f(xApple2 - 65, yApple2 + 30);
		glVertex2f(xApple2 - 45, yApple2 + 40);
		glVertex2f(xApple2 - 15, yApple2 + 40);
		glEnd();

		glColor3f(0.2, 0.1, 0.2);
		vprint(xApple2, yApple2, GLUT_BITMAP_HELVETICA_18, "%d", point2);
	}

	/*THIRD APPLE*/
	if (showApple3 == true)
	{
		if (yApple3 < -400 || yApple3 > 450)
		{
			r3 = (rand() % 1000);
			g3 = (rand() % 1000);
			b3 = (rand() % 1000);
			xApple3 = rand() % 620;
			point3 = (rand() % 8) + 1;
		}

		//leaf of the apple
		glColor3f(0.2, 0.509, 0.129);
		glBegin(GL_QUADS);
		glVertex2f(xApple3 - 2, yApple3 + 60);
		glVertex2f(xApple3 + 15, yApple3 + 80);
		glVertex2f(xApple3 + 65, yApple3 + 90);
		glVertex2f(xApple3 + 25, yApple3 + 60);
		glEnd();

		//Outline of leaf
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple3 - 2, yApple3 + 60);
		glVertex2f(xApple3 + 15, yApple3 + 80);
		glVertex2f(xApple3 + 65, yApple3 + 90);
		glVertex2f(xApple3 + 25, yApple3 + 60);
		glEnd();

		//Stem of the apple
		glColor3f(0.509, 0.223, 0.129);
		glBegin(GL_TRIANGLES);
		glVertex2f(xApple3 + 2, yApple3 + 15);
		glVertex2f(xApple3 - 25, yApple3 + 100);
		glVertex2f(xApple3 - 5, yApple3 + 115);
		glEnd();

		//Outline of stem
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple3 + 2, yApple3 + 15);
		glVertex2f(xApple3 - 25, yApple3 + 100);
		glVertex2f(xApple3 - 5, yApple3 + 115);
		glEnd();

		//shape of the apple
		glColor3f(r3 / 1000, g3 / 1000, b3 / 1000);
		glBegin(GL_POLYGON);
		glVertex2f(xApple3, yApple3 + 25);
		glVertex2f(xApple3 + 15, yApple3 + 40);
		glVertex2f(xApple3 + 45, yApple3 + 40);
		glVertex2f(xApple3 + 65, yApple3 + 30);
		glVertex2f(xApple3 + 75, yApple3 + 10);
		glVertex2f(xApple3 + 75, yApple3 - 10);
		glVertex2f(xApple3 + 65, yApple3 - 30);
		glVertex2f(xApple3 + 45, yApple3 - 40);
		glVertex2f(xApple3 + 15, yApple3 - 40);
		glVertex2f(xApple3, yApple3 - 25);
		glVertex2f(xApple3 - 15, yApple3 - 40);
		glVertex2f(xApple3 - 45, yApple3 - 40);
		glVertex2f(xApple3 - 65, yApple3 - 30);
		glVertex2f(xApple3 - 75, yApple3 - 10);
		glVertex2f(xApple3 - 75, yApple3 + 10);
		glVertex2f(xApple3 - 65, yApple3 + 30);
		glVertex2f(xApple3 - 45, yApple3 + 40);
		glVertex2f(xApple3 - 15, yApple3 + 40);
		glEnd();

		//Outline of apple
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple3, yApple3 + 25);
		glVertex2f(xApple3 + 15, yApple3 + 40);
		glVertex2f(xApple3 + 45, yApple3 + 40);
		glVertex2f(xApple3 + 65, yApple3 + 30);
		glVertex2f(xApple3 + 75, yApple3 + 10);
		glVertex2f(xApple3 + 75, yApple3 - 10);
		glVertex2f(xApple3 + 65, yApple3 - 30);
		glVertex2f(xApple3 + 45, yApple3 - 40);
		glVertex2f(xApple3 + 15, yApple3 - 40);
		glVertex2f(xApple3, yApple3 - 25);
		glVertex2f(xApple3 - 15, yApple3 - 40);
		glVertex2f(xApple3 - 45, yApple3 - 40);
		glVertex2f(xApple3 - 65, yApple3 - 30);
		glVertex2f(xApple3 - 75, yApple3 - 10);
		glVertex2f(xApple3 - 75, yApple3 + 10);
		glVertex2f(xApple3 - 65, yApple3 + 30);
		glVertex2f(xApple3 - 45, yApple3 + 40);
		glVertex2f(xApple3 - 15, yApple3 + 40);
		glEnd();

		glColor3f(0.2, 0.1, 0.2);
		vprint(xApple3, yApple3, GLUT_BITMAP_HELVETICA_18, "%d", point3);

	}

	/*FOURTH APPLE*/
	if (showApple4 == true)
	{
		if (yApple4 < -400 || yApple4 > 450)
		{
			r4 = (rand() % 1000);
			g4 = (rand() % 1000);
			b4 = (rand() % 1000);
			xApple4 = rand() % 620;
			point4 = (rand() % 8) + 1;
		}

		//leaf of the apple
		glColor3f(0.2, 0.509, 0.129);
		glBegin(GL_QUADS);
		glVertex2f(xApple4 - 2, yApple4 + 60);
		glVertex2f(xApple4 + 15, yApple4 + 80);
		glVertex2f(xApple4 + 65, yApple4 + 90);
		glVertex2f(xApple4 + 25, yApple4 + 60);
		glEnd();

		//Outline of leaf
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple4 - 2, yApple4 + 60);
		glVertex2f(xApple4 + 15, yApple4 + 80);
		glVertex2f(xApple4 + 65, yApple4 + 90);
		glVertex2f(xApple4 + 25, yApple4 + 60);
		glEnd();

		//Stem of the apple
		glColor3f(0.509, 0.223, 0.129);
		glBegin(GL_TRIANGLES);
		glVertex2f(xApple4 + 2, yApple4 + 15);
		glVertex2f(xApple4 - 25, yApple4 + 100);
		glVertex2f(xApple4 - 5, yApple4 + 115);
		glEnd();

		//Outline of stem
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple4 + 2, yApple4 + 15);
		glVertex2f(xApple4 - 25, yApple4 + 100);
		glVertex2f(xApple4 - 5, yApple4 + 115);
		glEnd();

		//shape of the apple
		glColor3f(r4 / 1000, g4 / 1000, b4 / 1000);
		glBegin(GL_POLYGON);
		glVertex2f(xApple4, yApple4 + 25);
		glVertex2f(xApple4 + 15, yApple4 + 40);
		glVertex2f(xApple4 + 45, yApple4 + 40);
		glVertex2f(xApple4 + 65, yApple4 + 30);
		glVertex2f(xApple4 + 75, yApple4 + 10);
		glVertex2f(xApple4 + 75, yApple4 - 10);
		glVertex2f(xApple4 + 65, yApple4 - 30);
		glVertex2f(xApple4 + 45, yApple4 - 40);
		glVertex2f(xApple4 + 15, yApple4 - 40);
		glVertex2f(xApple4, yApple4 - 25);
		glVertex2f(xApple4 - 15, yApple4 - 40);
		glVertex2f(xApple4 - 45, yApple4 - 40);
		glVertex2f(xApple4 - 65, yApple4 - 30);
		glVertex2f(xApple4 - 75, yApple4 - 10);
		glVertex2f(xApple4 - 75, yApple4 + 10);
		glVertex2f(xApple4 - 65, yApple4 + 30);
		glVertex2f(xApple4 - 45, yApple4 + 40);
		glVertex2f(xApple4 - 15, yApple4 + 40);
		glEnd();

		//Outline of apple
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple4, yApple4 + 25);
		glVertex2f(xApple4 + 15, yApple4 + 40);
		glVertex2f(xApple4 + 45, yApple4 + 40);
		glVertex2f(xApple4 + 65, yApple4 + 30);
		glVertex2f(xApple4 + 75, yApple4 + 10);
		glVertex2f(xApple4 + 75, yApple4 - 10);
		glVertex2f(xApple4 + 65, yApple4 - 30);
		glVertex2f(xApple4 + 45, yApple4 - 40);
		glVertex2f(xApple4 + 15, yApple4 - 40);
		glVertex2f(xApple4, yApple4 - 25);
		glVertex2f(xApple4 - 15, yApple4 - 40);
		glVertex2f(xApple4 - 45, yApple4 - 40);
		glVertex2f(xApple4 - 65, yApple4 - 30);
		glVertex2f(xApple4 - 75, yApple4 - 10);
		glVertex2f(xApple4 - 75, yApple4 + 10);
		glVertex2f(xApple4 - 65, yApple4 + 30);
		glVertex2f(xApple4 - 45, yApple4 + 40);
		glVertex2f(xApple4 - 15, yApple4 + 40);
		glEnd();

		glColor3f(0.2, 0.1, 0.2);
		vprint(xApple4, yApple4, GLUT_BITMAP_HELVETICA_18, "%d", point4);

	}

	/*FIFTH APPLE*/
	if (showApple5 == true)
	{
		if (yApple5 < -400 || yApple5 > 450)
		{
			r5 = (rand() % 1000);
			g5 = (rand() % 1000);
			b5 = (rand() % 1000);
			xApple5 = rand() % 620;
			point5 = (rand() % 8) + 1;
		}

		//leaf of the apple
		glColor3f(0.2, 0.509, 0.129);
		glBegin(GL_QUADS);
		glVertex2f(xApple5 - 2, yApple5 + 60);
		glVertex2f(xApple5 + 15, yApple5 + 80);
		glVertex2f(xApple5 + 65, yApple5 + 90);
		glVertex2f(xApple5 + 25, yApple5 + 60);
		glEnd();

		//Outline of leaf
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple5 - 2, yApple5 + 60);
		glVertex2f(xApple5 + 15, yApple5 + 80);
		glVertex2f(xApple5 + 65, yApple5 + 90);
		glVertex2f(xApple5 + 25, yApple5 + 60);
		glEnd();

		//Stem of the apple
		glColor3f(0.509, 0.223, 0.129);
		glBegin(GL_TRIANGLES);
		glVertex2f(xApple5 + 2, yApple5 + 15);
		glVertex2f(xApple5 - 25, yApple5 + 100);
		glVertex2f(xApple5 - 5, yApple5 + 115);
		glEnd();

		//Outline of stem
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple5 + 2, yApple5 + 15);
		glVertex2f(xApple5 - 25, yApple5 + 100);
		glVertex2f(xApple5 - 5, yApple5 + 115);
		glEnd();

		//shape of the apple
		glColor3f(r5 / 1000, g5 / 1000, b5 / 1000);
		glBegin(GL_POLYGON);
		glVertex2f(xApple5, yApple5 + 25);
		glVertex2f(xApple5 + 15, yApple5 + 40);
		glVertex2f(xApple5 + 45, yApple5 + 40);
		glVertex2f(xApple5 + 65, yApple5 + 30);
		glVertex2f(xApple5 + 75, yApple5 + 10);
		glVertex2f(xApple5 + 75, yApple5 - 10);
		glVertex2f(xApple5 + 65, yApple5 - 30);
		glVertex2f(xApple5 + 45, yApple5 - 40);
		glVertex2f(xApple5 + 15, yApple5 - 40);
		glVertex2f(xApple5, yApple5 - 25);
		glVertex2f(xApple5 - 15, yApple5 - 40);
		glVertex2f(xApple5 - 45, yApple5 - 40);
		glVertex2f(xApple5 - 65, yApple5 - 30);
		glVertex2f(xApple5 - 75, yApple5 - 10);
		glVertex2f(xApple5 - 75, yApple5 + 10);
		glVertex2f(xApple5 - 65, yApple5 + 30);
		glVertex2f(xApple5 - 45, yApple5 + 40);
		glVertex2f(xApple5 - 15, yApple5 + 40);
		glEnd();

		//Outline of apple
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xApple5, yApple5 + 25);
		glVertex2f(xApple5 + 15, yApple5 + 40);
		glVertex2f(xApple5 + 45, yApple5 + 40);
		glVertex2f(xApple5 + 65, yApple5 + 30);
		glVertex2f(xApple5 + 75, yApple5 + 10);
		glVertex2f(xApple5 + 75, yApple5 - 10);
		glVertex2f(xApple5 + 65, yApple5 - 30);
		glVertex2f(xApple5 + 45, yApple5 - 40);
		glVertex2f(xApple5 + 15, yApple5 - 40);
		glVertex2f(xApple5, yApple5 - 25);
		glVertex2f(xApple5 - 15, yApple5 - 40);
		glVertex2f(xApple5 - 45, yApple5 - 40);
		glVertex2f(xApple5 - 65, yApple5 - 30);
		glVertex2f(xApple5 - 75, yApple5 - 10);
		glVertex2f(xApple5 - 75, yApple5 + 10);
		glVertex2f(xApple5 - 65, yApple5 + 30);
		glVertex2f(xApple5 - 45, yApple5 + 40);
		glVertex2f(xApple5 - 15, yApple5 + 40);
		glEnd();

		glColor3f(0.2, 0.1, 0.2);
		vprint(xApple5, yApple5, GLUT_BITMAP_HELVETICA_18, "%d", point5);
	}

}

void drawDoctor()   //drawing the doctor in the game
{
	glColor3f(0.223, 0.074, 0.105);   //rectangle hovering base - doctor standing od this
	glRectf(xDoc - 70, yDoc - 280, xDoc + 80, yDoc - 320);

	glColor3f(0, 0, 0);    //outline of hover base
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc - 70, yDoc - 280);
	glVertex2f(xDoc + 80, yDoc - 280);
	glVertex2f(xDoc + 80, yDoc - 320);
	glVertex2f(xDoc - 70, yDoc - 320);
	glEnd();

	glColor3f(1, 0.745, 0.619);   //rectangle doctor's neck 
	glRectf(xDoc - 20, yDoc + 20, xDoc + 20, yDoc - 20);

	//Outline of the doctor's neck
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc - 20, yDoc + 20);
	glVertex2f(xDoc + 20, yDoc + 20);
	glVertex2f(xDoc + 20, yDoc - 20);
	glVertex2f(xDoc - 20, yDoc - 20);
	glEnd();

	// Circle - doctor's right hand
	glColor3f(1, 0.745, 0.619);
	circle(xDoc - 50, yDoc - 140, 20);

	//outline of hand
	glColor3f(0, 0, 0);
	circle_wire(xDoc - 50, yDoc - 140, 20);

	//Doctor's pants
	glColor3f(0.003, 0.003, 0.078);
	glBegin(GL_QUADS);
	glVertex2f(xDoc - 45, yDoc - 100);
	glVertex2f(xDoc - 45, yDoc - 280);
	glVertex2f(xDoc - 20, yDoc - 280);
	glVertex2f(xDoc - 10, yDoc - 140);
	glEnd();

	glColor3f(0.003, 0.003, 0.078);
	glBegin(GL_QUADS);
	glVertex2f(xDoc + 10, yDoc - 140);
	glVertex2f(xDoc + 20, yDoc - 280);
	glVertex2f(xDoc + 45, yDoc - 280);
	glVertex2f(xDoc + 45, yDoc - 100);
	glEnd();

	glColor3f(0.003, 0.003, 0.078);
	glBegin(GL_QUADS);
	glVertex2f(xDoc - 45, yDoc - 100);
	glVertex2f(xDoc + 45, yDoc - 100);
	glVertex2f(xDoc + 10, yDoc - 150);
	glVertex2f(xDoc - 10, yDoc - 150);
	glEnd();

	//Outline of the doctor's pants
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc - 45, yDoc - 100);
	glVertex2f(xDoc - 45, yDoc - 280);
	glVertex2f(xDoc - 20, yDoc - 280);
	glVertex2f(xDoc - 10, yDoc - 140);
	glVertex2f(xDoc + 10, yDoc - 140);
	glVertex2f(xDoc + 20, yDoc - 280);
	glVertex2f(xDoc + 45, yDoc - 280);
	glVertex2f(xDoc + 45, yDoc - 100);
	glEnd();

	//doctor's lab coat
	glColor3f(0.933, 0.937, 0.988);
	glBegin(GL_POLYGON);
	glVertex2f(xDoc - 10, yDoc - 10);
	glVertex2f(xDoc - 25, yDoc - 10);
	glVertex2f(xDoc - 30, yDoc + 10);
	glVertex2f(xDoc - 50, yDoc + 10);
	glVertex2f(xDoc - 70, yDoc - 10);
	glVertex2f(xDoc - 70, yDoc - 130);
	glVertex2f(xDoc - 50, yDoc - 130);
	glVertex2f(xDoc - 50, yDoc - 30);
	glVertex2f(xDoc - 50, yDoc - 180);
	glVertex2f(xDoc - 5, yDoc - 180);
	glVertex2f(xDoc, yDoc - 100);
	glVertex2f(xDoc + 5, yDoc - 180);
	glVertex2f(xDoc + 50, yDoc - 180);
	glVertex2f(xDoc + 50, yDoc - 20);
	glVertex2f(xDoc + 150, yDoc - 20);
	glVertex2f(xDoc + 150, yDoc + 10);
	glVertex2f(xDoc + 30, yDoc + 10);
	glVertex2f(xDoc + 25, yDoc - 5);
	glVertex2f(xDoc + 10, yDoc - 10);
	glEnd();

	//Outline of the doctor's coat
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc - 10, yDoc - 10);
	glVertex2f(xDoc - 25, yDoc - 5);
	glVertex2f(xDoc - 30, yDoc + 10);
	glVertex2f(xDoc - 50, yDoc + 10);
	glVertex2f(xDoc - 70, yDoc - 10);
	glVertex2f(xDoc - 70, yDoc - 130);
	glVertex2f(xDoc - 50, yDoc - 130);
	glVertex2f(xDoc - 50, yDoc - 30);
	glVertex2f(xDoc - 50, yDoc - 180);
	glVertex2f(xDoc - 5, yDoc - 180);
	glVertex2f(xDoc, yDoc - 100);
	glVertex2f(xDoc + 5, yDoc - 180);
	glVertex2f(xDoc + 50, yDoc - 180);
	glVertex2f(xDoc + 50, yDoc - 20);
	glVertex2f(xDoc + 150, yDoc - 20);
	glVertex2f(xDoc + 150, yDoc + 10);
	glVertex2f(xDoc + 30, yDoc + 10);
	glVertex2f(xDoc + 25, yDoc - 5);
	glVertex2f(xDoc + 10, yDoc - 10);
	glEnd();

	//coat buttons - Circle + outline
	glColor3f(0.941, 0.796, 0.6);
	circle(xDoc + 10, yDoc - 60, 5);
	glColor3f(0, 0, 0);
	circle_wire(xDoc + 10, yDoc - 60, 5);

	glColor3f(0.941, 0.796, 0.6);
	circle(xDoc + 10, yDoc - 80, 5);
	glColor3f(0, 0, 0);
	circle_wire(xDoc + 10, yDoc - 80, 5);

	glColor3f(0.941, 0.796, 0.6);
	circle(xDoc + 10, yDoc - 100, 5);
	glColor3f(0, 0, 0);
	circle_wire(xDoc + 10, yDoc - 100, 5);


	//blood spatter on coat - Circle
	glColor3f(0.666, 0.074, 0.101);
	circle(xDoc + 30, yDoc - 40, 10);

	glColor3f(0.666, 0.074, 0.101);
	circle(xDoc + 35, yDoc - 45, 7);

	glColor3f(0.666, 0.074, 0.101);
	circle(xDoc + 25, yDoc - 48, 6);

	glColor3f(0.666, 0.074, 0.101);
	circle(xDoc + 40, yDoc - 60, 8);

	//coat line
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc, yDoc - 10);
	glVertex2f(xDoc, yDoc - 100);

	//Stethoscope - line strip
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc, yDoc - 40);
	glVertex2f(xDoc - 20, yDoc - 30);
	glVertex2f(xDoc - 40, yDoc - 10);
	glVertex2f(xDoc - 40, yDoc + 10);
	glVertex2f(xDoc - 30, yDoc + 20);
	glVertex2f(xDoc + 30, yDoc + 20);
	glVertex2f(xDoc + 40, yDoc + 10);
	glVertex2f(xDoc + 40, yDoc - 10);
	glVertex2f(xDoc + 20, yDoc - 30);
	glVertex2f(xDoc, yDoc - 40);
	glVertex2f(xDoc - 10, yDoc - 45);
	glVertex2f(xDoc - 20, yDoc - 50);
	glVertex2f(xDoc - 25, yDoc - 60);
	glVertex2f(xDoc - 20, yDoc - 70);
	glVertex2f(xDoc - 5, yDoc - 70);
	glEnd();
	//stethoscope circle
	glColor3f(0, 0, 0);
	circle(xDoc - 5, yDoc - 70, 8);


	//doctor's face - circle
	glColor3f(1, 0.745, 0.619);
	circle(xDoc, yDoc + 50, 40);
	glColor3f(0, 0, 0);
	circle_wire(xDoc, yDoc + 50, 40);

	//doctor's glasses
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc - 40, yDoc + 60);
	glVertex2f(xDoc + 40, yDoc + 60);
	glEnd();

	//glasses lens1 + outline
	glColor3f(0.8, 0.976, 0.980);
	glRectf(xDoc - 25, yDoc + 65, xDoc - 5, yDoc + 55);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc - 25, yDoc + 65);
	glVertex2f(xDoc - 25, yDoc + 55);
	glVertex2f(xDoc - 5, yDoc + 55);
	glVertex2f(xDoc - 5, yDoc + 65);
	glEnd();

	//glasses lens2 + outline
	glColor3f(0.8, 0.976, 0.980);
	glRectf(xDoc + 5, yDoc + 65, xDoc + 25, yDoc + 55);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc + 5, yDoc + 65);
	glVertex2f(xDoc + 5, yDoc + 55);
	glVertex2f(xDoc + 25, yDoc + 55);
	glVertex2f(xDoc + 25, yDoc + 65);
	glEnd();

	//doctor's nose - triangle + outline
	glColor3f(0.956, 0.611, 0.443);
	glBegin(GL_TRIANGLES);
	glVertex2f(xDoc, yDoc + 50);
	glVertex2f(xDoc - 5, yDoc + 40);
	glVertex2f(xDoc + 5, yDoc + 40);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc, yDoc + 50);
	glVertex2f(xDoc - 5, yDoc + 40);
	glVertex2f(xDoc + 5, yDoc + 40);
	glEnd();

	//doctor's mouth - line strip
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc - 20, yDoc + 20);
	glVertex2f(xDoc - 10, yDoc + 30);
	glVertex2f(xDoc, yDoc + 20);
	glVertex2f(xDoc + 10, yDoc + 30);
	glVertex2f(xDoc + 20, yDoc + 20);
	glEnd();

	//Doctor's hair - line strips
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc - 30, yDoc + 78);
	glVertex2f(xDoc - 30, yDoc + 70);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc - 20, yDoc + 84);
	glVertex2f(xDoc - 20, yDoc + 70);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc - 10, yDoc + 88);
	glVertex2f(xDoc - 10, yDoc + 70);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc, yDoc + 90);
	glVertex2f(xDoc, yDoc + 70);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc + 10, yDoc + 88);
	glVertex2f(xDoc + 10, yDoc + 70);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc + 20, yDoc + 84);
	glVertex2f(xDoc + 20, yDoc + 70);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xDoc + 30, yDoc + 78);
	glVertex2f(xDoc + 30, yDoc + 70);
	glEnd();


	//Pistol that doctor is holding + each element's outline
	glColor3f(0.388, 0.392, 0.431);
	glBegin(GL_QUADS);
	glVertex2f(xDoc + 150, yDoc + 8);
	glVertex2f(xDoc + 210, yDoc + 8);
	glVertex2f(xDoc + 210, yDoc);
	glVertex2f(xDoc + 150, yDoc);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc + 150, yDoc + 8);
	glVertex2f(xDoc + 210, yDoc + 8);
	glVertex2f(xDoc + 210, yDoc);
	glVertex2f(xDoc + 150, yDoc);
	glEnd();

	glColor3f(0.231, 0.235, 0.247);
	glBegin(GL_QUADS);
	glVertex2f(xDoc + 150, yDoc);
	glVertex2f(xDoc + 150, yDoc - 3);
	glVertex2f(xDoc + 210, yDoc - 3);
	glVertex2f(xDoc + 210, yDoc);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc + 150, yDoc);
	glVertex2f(xDoc + 150, yDoc - 3);
	glVertex2f(xDoc + 210, yDoc - 3);
	glVertex2f(xDoc + 210, yDoc);
	glEnd();

	glColor3f(0.423, 0.427, 0.498);
	glBegin(GL_POLYGON);
	glVertex2f(xDoc + 150, yDoc - 3);
	glVertex2f(xDoc + 200, yDoc - 3);
	glVertex2f(xDoc + 200, yDoc - 10);
	glVertex2f(xDoc + 170, yDoc - 10);
	glVertex2f(xDoc + 170, yDoc - 30);
	glVertex2f(xDoc + 155, yDoc - 30);
	glVertex2f(xDoc + 155, yDoc - 10);
	glVertex2f(xDoc + 150, yDoc - 10);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc + 150, yDoc - 3);
	glVertex2f(xDoc + 200, yDoc - 3);
	glVertex2f(xDoc + 200, yDoc - 10);
	glVertex2f(xDoc + 170, yDoc - 10);
	glVertex2f(xDoc + 170, yDoc - 30);
	glVertex2f(xDoc + 155, yDoc - 30);
	glVertex2f(xDoc + 155, yDoc - 10);
	glVertex2f(xDoc + 150, yDoc - 10);
	glEnd();

	glColor3f(0.290, 0.290, 0.290);
	glBegin(GL_POLYGON);
	glVertex2f(xDoc + 180, yDoc - 10);
	glVertex2f(xDoc + 183, yDoc - 10);
	glVertex2f(xDoc + 183, yDoc - 23);
	glVertex2f(xDoc + 170, yDoc - 23);
	glVertex2f(xDoc + 170, yDoc - 20);
	glVertex2f(xDoc + 180, yDoc - 20);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc + 180, yDoc - 10);
	glVertex2f(xDoc + 183, yDoc - 10);
	glVertex2f(xDoc + 183, yDoc - 23);
	glVertex2f(xDoc + 170, yDoc - 23);
	glVertex2f(xDoc + 170, yDoc - 20);
	glVertex2f(xDoc + 180, yDoc - 20);
	glEnd();

	//trigger
	glColor3f(0, 0, 0);
	glRectf(xDoc + 172, yDoc - 10, xDoc + 176, yDoc - 17);


	//doctor's shoes
	//right
	glColor3f(0.513, 0.231, 0.105);
	glBegin(GL_QUADS);
	glVertex2f(xDoc - 50, yDoc - 270);
	glVertex2f(xDoc - 50, yDoc - 280);
	glVertex2f(xDoc - 15, yDoc - 270);
	glVertex2f(xDoc - 15, yDoc - 280);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc - 50, yDoc - 270);
	glVertex2f(xDoc - 50, yDoc - 280);
	glVertex2f(xDoc - 15, yDoc - 270);
	glVertex2f(xDoc - 15, yDoc - 280);
	glEnd();

	//left
	glColor3f(0.513, 0.231, 0.105);
	glBegin(GL_QUADS);
	glVertex2f(xDoc + 50, yDoc - 270);
	glVertex2f(xDoc + 50, yDoc - 280);
	glVertex2f(xDoc + 15, yDoc - 270);
	glVertex2f(xDoc + 15, yDoc - 280);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xDoc + 50, yDoc - 270);
	glVertex2f(xDoc + 50, yDoc - 280);
	glVertex2f(xDoc + 15, yDoc - 270);
	glVertex2f(xDoc + 15, yDoc - 280);
	glEnd();


}

void drawBackground()    //drawing the background
{
	//Ceiling + outline - Quad & line loop
	glColor3f(0.960, 0.941, 0.760);
	glBegin(GL_QUADS);
	glVertex2f(-700, 400);
	glVertex2f(700, 400);
	glVertex2f(252, 275);
	glVertex2f(-252, 275);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-700, 400);
	glVertex2f(700, 400);
	glVertex2f(252, 275);
	glVertex2f(-252, 275);
	glEnd();

	//middle wall
	glColor3f(0.650, 0.690, 0.678);
	glBegin(GL_QUADS);
	glVertex2f(-252, 275);
	glVertex2f(252, 275);
	glVertex2f(252, 25);
	glVertex2f(-252, 25);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-252, 275);
	glVertex2f(252, 275);
	glVertex2f(252, 25);
	glVertex2f(-252, 25);
	glEnd();

	//left wall
	glColor3f(0.839, 0.862, 0.854);
	glBegin(GL_QUADS);
	glVertex2f(-700, 400);
	glVertex2f(-252, 275);
	glVertex2f(-252, 25);
	glVertex2f(-700, -400);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-700, 400);
	glVertex2f(-252, 275);
	glVertex2f(-252, 25);
	glVertex2f(-700, -400);
	glEnd();

	//right wall
	glColor3f(0.839, 0.862, 0.854);
	glBegin(GL_QUADS);
	glVertex2f(700, 400);
	glVertex2f(252, 275);
	glVertex2f(252, 25);
	glVertex2f(700, -400);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(700, 400);
	glVertex2f(252, 275);
	glVertex2f(252, 25);
	glVertex2f(700, -400);
	glEnd();

	//left door
	glColor3f(0.407, 0.211, 0.133);
	glBegin(GL_QUADS);
	glVertex2f(-420, 50);
	glVertex2f(-336, 125);
	glVertex2f(-336, -175);
	glVertex2f(-420, -175);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-420, 50);
	glVertex2f(-336, 125);
	glVertex2f(-336, -175);
	glVertex2f(-420, -175);
	glEnd();

	//left door knob - CIRCLE
	glColor3f(0.815, 0.690, 0.305);
	circle(-364, 0, 12);
	glColor3f(0, 0, 0);
	circle_wire(-364, 0, 12);


	//left window rim
	glColor3f(0.047, 0.043, 0.196);
	glBegin(GL_QUADS);
	glVertex2f(-514, 112);
	glVertex2f(-514, -108);
	glVertex2f(-686, -262);
	glVertex2f(-686, -38);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-514, 112);
	glVertex2f(-514, -108);
	glVertex2f(-686, -262);
	glVertex2f(-686, -38);
	glEnd();

	//left window glass
	glColor3f(0.729, 0.823, 0.870);
	glBegin(GL_QUADS);
	glVertex2f(-532, 75);
	glVertex2f(-532, -100);
	glVertex2f(-672, -225);
	glVertex2f(-672, -50);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-532, 75);
	glVertex2f(-532, -100);
	glVertex2f(-672, -225);
	glVertex2f(-672, -50);
	glEnd();

	//right door1 - QUAD
	glColor3f(0.407, 0.211, 0.133);
	glBegin(GL_QUADS);
	glVertex2f(308, 150);
	glVertex2f(420, 50);
	glVertex2f(420, -150);
	glVertex2f(308, -150);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(308, 150);
	glVertex2f(420, 50);
	glVertex2f(420, -150);
	glVertex2f(308, -150);
	glEnd();

	//right door1 knob - CIRCLE
	glColor3f(0.815, 0.690, 0.305);
	circle(392, -25, 12);
	glColor3f(0, 0, 0);
	circle_wire(392, -25, 12);

	//right door2 - QUAD
	glColor3f(0.407, 0.211, 0.133);
	glBegin(GL_QUADS);
	glVertex2f(532, -50);
	glVertex2f(616, -125);
	glVertex2f(616, -325);
	glVertex2f(532, -325);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(532, -50);
	glVertex2f(616, -125);
	glVertex2f(616, -325);
	glVertex2f(532, -325);
	glEnd();

	//right door2 knob - CIRCLE
	glColor3f(0.815, 0.690, 0.305);
	circle(588, -200, 12);
	glColor3f(0, 0, 0);
	circle_wire(588, -200, 12);

	//floor
	glColor3f(0.988, 0.823, 0.811);
	glBegin(GL_QUADS);
	glVertex2f(-252, 25);
	glVertex2f(252, 25);
	glVertex2f(700, -400);
	glVertex2f(-700, -400);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-252, 25);
	glVertex2f(252, 25);
	glVertex2f(700, -400);
	glVertex2f(-700, -400);
	glEnd();

	//Tubelight on the ceiling
	glColor3f(0, 0, 0);
	glRectf(-202, 400, -190, 375);

	glColor3f(0, 0, 0);
	glRectf(202, 400, 190, 375);

	glColor3f(0, 0, 0);
	glRectf(-280, 375, 280, 365);

	glColor3f(0.937, 0.913, 0.505);
	glBegin(GL_QUADS);
	glVertex2f(-275, 365);
	glVertex2f(-268, 350);
	glVertex2f(268, 350);
	glVertex2f(275, 365);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-275, 365);
	glVertex2f(-268, 350);
	glVertex2f(268, 350);
	glVertex2f(275, 365);
	glEnd();

	//Middle door Rim
	glColor3f(0.541, 0.384, 0.317);
	glBegin(GL_QUADS);
	glVertex2f(-168, 200);
	glVertex2f(168, 200);
	glVertex2f(168, 25);
	glVertex2f(-168, 25);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-168, 200);
	glVertex2f(168, 200);
	glVertex2f(168, 25);
	glVertex2f(-168, 25);
	glEnd();

	//Middle door
	glColor3f(0.317, 0.521, 0.541);
	glBegin(GL_QUADS);
	glVertex2f(-140, 175);
	glVertex2f(140, 175);
	glVertex2f(140, 25);
	glVertex2f(-140, 25);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-140, 175);
	glVertex2f(140, 175);
	glVertex2f(140, 25);
	glVertex2f(-140, 25);
	glEnd();

	//Middle door line
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 175);
	glVertex2f(0, 25);
	glEnd();

	//Middle door left knob
	glColor3f(0.815, 0.690, 0.305);
	circle(-28, 100, 10);
	glColor3f(0, 0, 0);
	circle_wire(-28, 100, 10);

	//Middle door right knob
	glColor3f(0.815, 0.690, 0.305);
	circle(28, 100, 10);
	glColor3f(0, 0, 0);
	circle_wire(28, 100, 10);

	//"Emergency room" sign
	glColor3f(0.913, 0.537, 0.505);
	glBegin(GL_QUADS);
	glVertex2f(-196, 212);
	glVertex2f(-196, 262);
	glVertex2f(196, 262);
	glVertex2f(196, 212);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-196, 212);
	glVertex2f(-196, 262);
	glVertex2f(196, 262);
	glVertex2f(196, 212);
	glEnd();
	glColor3f(0.2, 0.1, 0.2);
	vprint(-120, 230, GLUT_BITMAP_TIMES_ROMAN_24, "EMERGENCY ROOM");

	//Floor line1
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-168, 25);
	glVertex2f(-448, -400);
	glEnd();

	//Floor line2
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-84, 25);
	glVertex2f(-224, -400);
	glEnd();

	//Floor line3
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 25);
	glVertex2f(0, -400);
	glEnd();

	//Floor line4
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(84, 25);
	glVertex2f(224, -400);
	glEnd();

	//Floor line5
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(168, 25);
	glVertex2f(448, -400);
	glEnd();

	//floor horizontal line1
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-672, -375);
	glVertex2f(672, -375);
	glEnd();

	//floor horizontal line2
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-278, 0);
	glVertex2f(278, 0);
	glEnd();

	//floor horizontal line3
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-357, -75);
	glVertex2f(357, -75);
	glEnd();

	//floor horizontal line4
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-462, -175);
	glVertex2f(462, -175);
	glEnd();

	//floor horizontal line5
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-568, -275);
	glVertex2f(568, -275);
	glEnd();

	
	//Drawing Name and ID tag
	glColor3f(0.8, 0.741, 0.741);
	glRectf(-((winWidth / 2) - 10), (winHeight / 2) - 8, -((winWidth / 2) - 235), (winHeight / 2) - 55);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-((winWidth / 2) - 10), (winHeight / 2) - 8);
	glVertex2f(-((winWidth / 2) - 10), (winHeight / 2) - 55);
	glVertex2f(-((winWidth / 2) - 235), (winHeight / 2) - 55);
	glVertex2f(-((winWidth / 2) - 235), (winHeight / 2) - 8);
	glEnd();
	glColor3f(0.2, 0.1, 0.2);
	vprint(-((winWidth / 2) - 20), (winHeight / 2) - 32.5, GLUT_BITMAP_TIMES_ROMAN_24, "SUHEERA TANVIR");
	vprint(-((winWidth / 2) - 20), (winHeight / 2) - 50, GLUT_BITMAP_HELVETICA_18, "            21801211");

	//writing score and timer in the lower left of the screen
	glColor3f(0, 0, 0);
	vprint(-500, -273, GLUT_BITMAP_TIMES_ROMAN_24, "SCORE : %d", score);
	vprint(-350, -273, GLUT_BITMAP_TIMES_ROMAN_24, "TIME LEFT : %0.2f seconds", timer / 100);


}

void ShowGameover()   //paints this on the screen when game is over
{
	if (gameover == true)
	{
		glColor3f(0, 0, 0);
		glLineWidth(10);
		vprint2(-495, 5, 1.2, "GAME OVER");
		glLineWidth(1);
		glLineWidth(5);
		vprint2(-300, -100, 0.6, "SCORE : %d", score);
		glLineWidth(1);
	}

}
void ShowPauseScreen()  //paints this on the screen when gamestate is pause screen
{
	if (gameState == PAUSESCREEN)
	{
		glColor3f(0, 0, 0);
		glLineWidth(10);
		vprint2(-550, 5, 1.2, "GAME PAUSED");
		glLineWidth(1);
	}

}

void DrawHowToPlayScreen()  //paints this on the screen when gamestate is howtoplay
{
	glColor3f(0.219, 0.054, 0.325);
	glRectf(-800, 500, 800, -500);

	glColor3f(0, 0, 0);
	glRectf(-650, 350, 600, -250);

	glColor3f(0.737, 0.466, 0.913);
	glRectf(-600, 300, 550, -200);

	glColor3f(0, 0, 0);
	vprint(-300, 250, GLUT_BITMAP_HELVETICA_18, "Have you heard of the phrase 'An apple a day keeps the doctor away'?.....");
	vprint(-380, 225, GLUT_BITMAP_HELVETICA_18, "Apples have slowly destroyed humanity, with even doctors unable to stop their terrifying wrath!");
	vprint(-260, 200, GLUT_BITMAP_HELVETICA_18, "Help this brave and battered doctor end the Apple-pocalypse!");
	glLineWidth(5);
	vprint2(-190, 100, 0.4, "HOW TO PLAY");
	glLineWidth(1);
	vprint(-230, 50, GLUT_BITMAP_HELVETICA_18, "press the up and down arrows keys to move the doctor");
	vprint(-180, 25, GLUT_BITMAP_HELVETICA_18, "press spacebar to shoot at the apples");
	vprint(-280, 0, GLUT_BITMAP_HELVETICA_18, "press p to pause the game, u to unpause & h to open how to play");
	vprint(-240, -25, GLUT_BITMAP_HELVETICA_18, "Be Careful! Apples worth a higher score will be faster!");
	vprint(-305, -50, GLUT_BITMAP_HELVETICA_18, "The game will end in 20 seconds, so try to shoot as many as you can!");

	vprint(-130, -150, GLUT_BITMAP_HELVETICA_18, "press s to start/continue game!!!");
}

void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//switch statement to switch between gamestates
	switch (gameState) {
	case HOWTOPLAY: DrawHowToPlayScreen(); break;
	case GAME: drawBackground(),
		drawApples(),
		bullet(),
		drawDoctor(),
		ShowGameover(); break;
	case PAUSESCREEN: drawBackground(),
		ShowPauseScreen(); break;
	}

	glutSwapBuffers();
}


void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	//shoot bullet when space is pressed
	if (key == ' ')
		movement = true;

	//start game when s is pressed and current gamestate is howtoplay
	if ((key == 's' || key == 'S') && gameState == HOWTOPLAY)
		gameState = GAME;

	//pause game when p is pressed and current gamestate is game
	if ((key == 'p' || key == 'P') && gameState == GAME)
		gameState = PAUSESCREEN;

	//unpause game when u is pressed and current gamestate is pause
	if ((key == 'u' || key == 'U') && gameState == PAUSESCREEN)
		gameState = GAME;

	//open howtoplay screen when h is pressed
	if (key == 'h' || key == 'H')
		gameState = HOWTOPLAY;

	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	glutPostRedisplay();
}


void onSpecialKeyDown(int key, int x, int y)
{
	
	switch (key) {
	case GLUT_KEY_UP: up = true;
		if (yDoc < 310 && gameover == false) //move doctor up when the up arrow is pressed
		{
			yDoc += 10;  //move up doctor by increment of 10
			if (!movement) {
				yBullet = yDoc;       //move bullet back into pistol when movement is falsed
				xBullet = xDoc + 200;
			}

		}

		break;
	case GLUT_KEY_DOWN: down = true;
		if (yDoc > -80 && gameover == false)  //move doctor down when the down arrow is pressed
		{
			yDoc -= 10;   ////move down doctor by increment of 10
			if (!movement) {
				yBullet = yDoc;           //move bullet back into pistol
				xBullet = xDoc + 200;
			}
		}

		break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	glutPostRedisplay();
}

void onSpecialKeyUp(int key, int x, int y)
{

	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	glutPostRedisplay();
}


void onClick(int button, int stat, int x, int y)
{

	glutPostRedisplay();
}


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
 
	glutPostRedisplay();
}


void onMove(int x, int y) {

	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v)
{
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (gameover == false && gameState == GAME)  
	{
		timer -= 2;   //timer is decremented by 2 because time period constant is set to 20. this gives uniform decrementing of time

		if (timer == 0)
			gameover = true;   //gameover state happen when timer ends till 0

		BulletMovement();

		yApple -= point; //the apple moves faster if the score is higher this way, as it decrements the amount of its score.

		if (yApple < -500)
		{
			yApple = 460;  //if apple goes out of bounds, send it to the top and start decrementing again for movement
			yApple--;
		}

		yApple2 -= point2;  //same for all other apples
		if (yApple2 < -500)
		{
			yApple2 = 460;
			yApple2--;
		}

		yApple3 -= point3;
		if (yApple3 < -500)
		{
			yApple3 = 460;
			yApple3--;
		}

		yApple4 -= point4;
		if (yApple4 < -500)
		{
			yApple4 = 460;
			yApple4--;
		}

		yApple5 -= point5;
		if (yApple5 < -500)
		{
			yApple5 = 460;
			yApple5--;
		}
	}

	glutPostRedisplay(); 

}
#endif

void Init() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("SUHEERA TANVIR 21801211 - An apple a day keeps the doc....doctor's revenge");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}