#include <Windows.h>
#include <GL\freeglut.h>

//#include <gl\GL.h>
//#include <GL\GL.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>


#include "ModuleCouleurs.h"
#include "ModuleManipulateur.h"
#include "ModuleMenus.h"
#include "ModuleReshape.h"
#include "loadppm.h"

char texFileName[] = "hexagonalTrianglesRotate.ppm";
PPMImage *image;

GLfloat minX = 0.0, maxX = 1.0;
bool texture = false;

static struct tm instant;
int heures, minutes, secondes;
bool open = true, pause = false;
bool rapide = false;
bool recule = false;
bool reculeSec = false;
bool tempsSynchro = true;
int xCam = 0, yCam = 45, zCam = 0;
//float Sin[360], Cos[360]; // pas de 1 pour les calcul
int distanceCamera = 38;
float coefTransparence = 1;
int anglex, angley, x, y, xold, yold;
char presse;
#define PI 3.14159265

float rotateCouvX;
float rotateCouvY;
float rotateCouvZ;
float translateCouvX[10];
float translateCouvY;
float translateCouvZ[10];
float angleCouv[10];
int animationCouv = 0;

//gestion du temps systeme
void systemTime(void)
{
	time_t temps;
	//struct tm instant;

	time(&temps);
	localtime_s(&instant, &temps);
	heures = instant.tm_hour;
	minutes = instant.tm_min;
	secondes = instant.tm_sec;

	printf("%d/%d ; %d:%d:%d\n", instant.tm_mday, instant.tm_mon + 1, instant.tm_hour, instant.tm_min, instant.tm_sec);
	printf("%d:%d:%d\n", heures, minutes, secondes);
}

void majTemps(void)
{
	//printf("%d:%d:%d\n", heures, minutes, secondes);
	//secondes = secondes++ % 60;
	printf("%d:%d:%d\n", heures, minutes, secondes);
	if (secondes >= 60) secondes -= 60;
	if (secondes <= -1) secondes += 60;
	if (reculeSec){
		if (secondes == 59){
			minutes = minutes-- % 60;
			if (minutes <= -1) minutes += 60;
			printf("min --");
		}
	}
	if (secondes == 0 && reculeSec == false){
		minutes = minutes++ % 60;
		if (minutes >= 60) minutes -= 60;
		printf("min ++");
	}
	if (recule || reculeSec && secondes == 59){
		if (minutes == 59){
			printf("heure --");
			heures = heures-- % 24;
			if (heures <= -1) heures += 24;
			recule = false;
		}
	}
	
	if (minutes == 0 && recule == false && reculeSec == false){
		printf("heure ++");
		heures = heures++ % 24;
		if (heures >= 24) heures -= 24;
	}
	/*
	if (minutes == 0){
		printf("recule : %d", recule);
		if (recule){
			printf("heure --");
			heures = heures-- % 24;
		}else{
			printf("heure ++");
			heures = heures++ % 24;
		}
		if (heures >= 24) heures -= 24;
	}*/
}

void myIdle(void)
{
	
	time_t temps;
	//struct tm instant;
	struct tm newInstant;
	if (open == false){
		if (animationCouv < 45){
			animationCouv++;
			printf(" animation : %d", animationCouv/5);
			//printf("rotation : %f, %f, %f, %f", angleCouv[animationCouv], rotateCouvX, rotateCouvY, rotateCouvZ);
			//printf("translation : %f, %f, %f", translateCouvX[animationCouv], translateCouvY, translateCouvZ[animationCouv]);
		}
	}
	if (open == true){
		if (animationCouv > 0){
			animationCouv--;
			printf(" animation : %d", animationCouv);
			//printf("rotation : %f, %f, %f, %f", angleCouv[animationCouv], rotateCouvX, rotateCouvY, rotateCouvZ);
			//printf("translation : %f, %f, %f", translateCouvX[animationCouv], translateCouvY, translateCouvZ[animationCouv]);
		}
	}
	
	if (rapide == true){
		secondes = secondes++ % 60;
		if (secondes >= 60) secondes -= 60;
		if (secondes == 0){
			minutes = minutes++ % 60;
			if (minutes >= 60) minutes -= 60;
			printf("min ++");
		}
		if (minutes == 0 && secondes == 0){
			heures = heures++ % 24;
			if (heures >= 24) heures -= 24;
		}
	}
	
	//printf("%d:%d:%d\n", heures, minutes, secondes);
	time(&temps); 
	localtime_s(&newInstant, &temps);
	if (newInstant.tm_sec != instant.tm_sec && tempsSynchro == true)
	{
		
		//printf("new test %d \n", newInstant.tm_sec);
		instant = newInstant;
		//secondes = instant.tm_sec;
		if (rapide == false && pause == false){
			
			secondes = secondes++ % 60;
			if (secondes >= 60) secondes -= 60;
			if (secondes == 0){
				minutes = minutes++ % 60;
				if (minutes >= 60) minutes -= 60;
				printf("Idlemin ++");
			}
			if (minutes == 0 && secondes == 0){
				heures = heures++ % 24;
				if (heures >= 24) heures -= 24;
			}
			/*
			heures = instant.tm_hour;
			minutes = instant.tm_min;
			secondes = instant.tm_sec;
			*/
			
		}
		printf("test %d \n", secondes);
		
		//float angle = (instant.tm_sec / 10) % 360;
	}
	//printf("new test %d \n", newInstant.tm_sec);
	//printf("test %d \n",instant.tm_sec);
	if (pause == false){
		glutPostRedisplay(); // reaffiche la scène
	}
}

// gestion des lumieres et melanges
void myInit(void)
{
	image = new PPMImage(texFileName);
	
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image->data);
	delete image;
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	
	GLfloat light_position[] = { 1.0, 1.0, 20.0, 0.0 };
	glEnable(GL_LIGHTING);
	// on defini les parametres de la source 0
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, couleurNoir());
	glLightfv(GL_LIGHT0, GL_SPECULAR, couleurNoir());
	glLightfv(GL_LIGHT0, GL_DIFFUSE, couleurBlanc());
	glEnable(GL_LIGHT0);

	// definition de façon de traiter les faces
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// gestion des parametres de transparence
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// façon de gerer le modele d'eclairage
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	

}

// gear
static void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
	GLint teeth, GLfloat tooth_depth)
{
	GLint i;
	GLfloat r0, r1, r2;
	GLfloat angle, da;
	GLfloat u, v, len;

	r0 = inner_radius;
	r1 = outer_radius - tooth_depth / 2.0;
	r2 = outer_radius + tooth_depth / 2.0;

	da = 2.0 * PI / teeth / 4.0;

	glShadeModel(GL_FLAT);

	glNormal3f(0.0, 0.0, 1.0);

	//draw front face
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * PI / teeth;
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
	}
	glEnd();

	//draw front sides of teeth
	glBegin(GL_QUADS);
	da = 2.0 * PI / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * PI / teeth;

		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
	}
	glEnd();

	glNormal3f(0.0, 0.0, -1.0);

	//draw back face
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * PI / teeth;
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
	}
	glEnd();

	//draw back sides of teeth
	glBegin(GL_QUADS);
	da = 2.0 * PI / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * PI / teeth;

		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
	}
	glEnd();

	//draw outward faces of teeth
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * PI / teeth;

		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		u = r2 * cos(angle + da) - r1 * cos(angle);
		v = r2 * sin(angle + da) - r1 * sin(angle);
		len = sqrt(u * u + v * v);
		u /= len;
		v /= len;
		glNormal3f(v, -u, 0.0);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
		v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
		glNormal3f(v, -u, 0.0);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
	}

	glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
	glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

	glEnd();

	glShadeModel(GL_SMOOTH);

	//draw inside radius cylinder
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * PI / teeth;

		glNormal3f(-cos(angle), -sin(angle), 0.0);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	}
	glEnd();

}

void displayTexture(void)
{
	glEnable(GL_TEXTURE_2D);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glBegin(GL_QUADS);
	glTexCoord2f(minX, 0.0); glVertex3f(-10.0, -10.0, -0.01);
	glTexCoord2f(minX, 1.0); glVertex3f(-10.0, 10.0,- 0.01);
	glTexCoord2f(maxX, 1.0); glVertex3f(10.0, 10.0, -0.01);
	glTexCoord2f(maxX, 0.0); glVertex3f(10.0, -10.0, -0.01);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.5); glVertex3f(-10.0, 0.0, -0.01);
	glTexCoord2f(0.15, 0.15); glVertex3f(-7.4, -7.4, -0.01);
	glTexCoord2f(0.5, 0.0); glVertex3f(0.0, -10.0, -0.01);
	glTexCoord2f(0.85, 0.15); glVertex3f(7.4, -7.4, -0.01);
	glTexCoord2f(1.0, 0.5); glVertex3f(10.0, 0.0, -0.01);
	glTexCoord2f(0.85, 0.85); glVertex3f(7.4, 7.4, -0.01);
	glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 10.0, -0.01);
	glTexCoord2f(0.15, 0.85); glVertex3f(-7.4, 7.4, -0.01);
	*/
	glBegin(GL_POLYGON);
	glTexCoord2f(0.05, 0.75); glVertex3f(-8.0, 4.4, -0.01);
	glTexCoord2f(0.05, 0.25); glVertex3f(-8.0, -5.4, -0.01);
	glTexCoord2f(0.5, 0.0); glVertex3f(0.0, -10.0, -0.01);
	glTexCoord2f(0.95, 0.25); glVertex3f(8, -5.4, -0.01);
	//glTexCoord2f(1.0, 0.5); glVertex3f(10.0, 0.0, -0.01);
	glTexCoord2f(0.95, 0.75); glVertex3f(8, 4.6, -0.01);
	glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 10.0, -0.01);
	//glTexCoord2f(0.15, 0.85); glVertex3f(-7.4, 7.4, -0.01);
	glEnd();
	//glutSwapBuffers();
	//texture = true;
	glDisable(GL_TEXTURE_2D);
}

/* Fonction qui ne fonctionne pas avec les autres ?MYSTERE? 
Probablement du au glClear en début de fonction
*/
void displayCrochet(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurJauneClair(1.0f));

	glPushMatrix();
	glTranslatef(0.0, 0.0, 5.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(1.0, 1.5, 50, 50);
	//glutSolidTorus(0.75, 10.0, 50, 50);
	glPopMatrix();

	
	glPopMatrix();
	glPopMatrix();
}

// tentative de texte pour les chiffres
void displayChiffre(void)
{
	unsigned char douze[3] = { 'X', 'I', 'I' };
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glTranslatef(-0.7, 7.0, 1.5);
	glRasterPos2f(0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
	//glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, douze);
	glPopMatrix();
	glPopMatrix();
}

void displayChiffre12(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-0.8, 7.8, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.2, 7.8, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.85, 7.8, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void displayChiffre11(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-3.9, 6.6, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.85, 6.6, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glPopMatrix();
}

void displayChiffre10(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-6.5, 3.5, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	
	glPopMatrix();
}

void displayChiffre9(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-7.1, 0.0, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);

	glPopMatrix();
	glPushMatrix();
	glTranslatef(-8.1, 0.0, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void displayChiffre8(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-6.7, -3.7, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.7, -3.7, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.1, -3.7, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.5, -3.7, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();


	glPopMatrix();
}

void displayChiffre7(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-3.9, -6.6, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.9, -6.6, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix(); 
	
	glPushMatrix();
	glTranslatef(-2.3, -6.6, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();


	glPopMatrix();
}

void displayChiffre6(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-0.5, -7.8, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -7.8, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void displayChiffre5(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(3.9, -6.6, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);

	glPopMatrix();
	glPopMatrix();
}

void displayChiffre4(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(6.7, -3.7, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0); 
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(5.2, -3.7, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glPopMatrix();
}

void displayChiffre3(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	//glTranslatef(-7.1, 0.0, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(6.8, 0.0, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(7.45, 0.0, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(8.1, 0.0, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void displayChiffre2(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	//glTranslatef(-7.1, 0.0, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(6.3, 3.5, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(6.95, 3.5, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void displayChiffre1(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	//glRasterPos2f(0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	//glTranslatef(-7.1, 0.0, 1.5);
	//glRasterPos2f(0.0, 0.0);
	glPushMatrix();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(3.9, 6.6, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void displayChiffres(void)
{

	displayChiffre12();
	displayChiffre11();
	displayChiffre10();
	displayChiffre9();
	displayChiffre8();
	displayChiffre7();
	displayChiffre6();
	displayChiffre5();
	displayChiffre4();
	displayChiffre3();
	displayChiffre2();
	displayChiffre1();
}

// dessin du disque de la montre
void displayCadran(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurJauneClair(coefTransparence));
	//glScalef(17.0, 5.0, 10.0);

	/* Anneau pour la chaine */
	glPushMatrix();
	glTranslatef(0.0, 13.1, 0.75);
	//glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidTorus(0.3, 1.2, 50, 50);
	glPopMatrix();

	/* Bouton pour modifier l'heure */
	glPushMatrix();
	glTranslatef(0.0, 13.0, 0.75);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(1, 1.5, 50, 50);
	//glutSolidTorus(0.75, 10.0, 50, 50);
	glPopMatrix();

	/* Attache au dessus de la montre */
	glPushMatrix();
	glTranslatef(0.0, 12.0, 0.75);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.5, 3.0, 50, 50);
	//glutSolidTorus(0.75, 10.0, 50, 50);
	glPopMatrix();

	/* Cadran principal */
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.75);
	glutSolidTorus(0.75,10.0,50,50);
	glPopMatrix();

	glutSolidCylinder(10.0, 1.5, 50, 50);
	glPopMatrix();
	glPopMatrix();
}

// dessin du balancier qui sert à reguler le mouvement
void displayBalancier(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurMagenta(1.2f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	//float angle = (secondes * 6) % 360;
	//glRotatef(angle - 1, 0.0, 0.0, -0.5);
	float angle = (secondes % 2 * 12) - 6;

	glPushMatrix();
	glTranslatef(2.8, 1.05, 0.7);
	glRotatef(angle, 0.0, 0.0, 1.0);
	gear(0.1, 0.8, 0.15, 12, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.8, 1.05, 0.75);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glutSolidCylinder(0.5, 0.1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	if (secondes%2 ==0)
		glTranslatef(2.4, 1.05, 0.77);
	else
		glTranslatef(2.8, 1.45, 0.77);
	//glRotatef(angle, 0.0, 0.0, 1.0);
	glutSolidCylinder(0.03, 0.15, 50, 50);
	glPopMatrix();

	glPushMatrix();
	if (secondes%2 == 0)
		glTranslatef(2.8, 0.65, 0.77);
	else
		glTranslatef(2.4, 1.05, 0.77);
	//glRotatef(angle, 0.0, 0.0, 1.0);
	glutSolidCylinder(0.03, 0.15, 50, 50);
	
	glPopMatrix();
	//glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'ancre qui transmet la force
void displayAncre(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurMagentaFonce(1.2f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	//float angle = (secondes * 6) % 360;
	//glRotatef(angle - 1, 0.0, 0.0, -0.5);

	
	glTranslatef(2.3, 1.05, 0.9);
	if (secondes % 2 == 0)
		glRotatef(65, 0.0, 0.0, 1.0);
	else
		glRotatef(-65, 0.0, 0.0, 1.0);
	glScalef(0.03, 0.7, 0.04);
	glutSolidCube(1.0);

	glPopMatrix();

	glPushMatrix();
	if (secondes % 2 == 0){
		glTranslatef(1.99, 1.2, 0.9);
		glRotatef(-25, 0.0, 0.0, 1.0);
	}
	else{
		glTranslatef(1.99, 0.9, 0.9);
		glRotatef(25, 0.0, 0.0, 1.0);
	}
	glScalef(0.03, 0.5, 0.04);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	if (secondes % 2 == 0){
		glTranslatef(2.0, 1.4, 0.91);
		glRotatef(100, 0.0, 0.0, 1.0);
	}
	else{
		glTranslatef(1.85, 1.05, 0.91);
		glRotatef(150, 0.0, 0.0, 1.0);
	}
	glScalef(0.03, 0.15, 0.04);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	if (secondes % 2 == 0){
		glTranslatef(1.85, 1.05, 0.91);
		glRotatef(-150, 0.0, 0.0, 1.0);
	}
	else{
		glTranslatef(2.03, 0.7, 0.91);
		glRotatef(-100, 0.0, 0.0, 1.0);
	}
	glScalef(0.03, 0.15, 0.04);
	glutSolidCube(1.0);
	glPopMatrix();
	
	//glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}
// dessin de la roue d'échappement qui compte les oscillations du balancier
void displayRoueEchappement(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurCyanFonce(1.2f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	float angle = (secondes * 15) % 360;

	glPushMatrix();
	glTranslatef(1.05, 1.05, 0.95);
	glRotatef(angle - 1, 0.0, 0.0, 0.5);
	gear(0.1, 0.7, 0.15, 12, 0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.05, 1.05, 0.45);
	glRotatef(angle - 1, 0.0, 0.0, 0.5);
	glutSolidCylinder(0.1, 0.6, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.05, 1.05, 0.45);
	glRotatef(angle - 1, 0.0, 0.0, 0.5);
	gear(0.1, 0.2, 0.1, 12, 0.1);
	glPopMatrix();

	//glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'engrenage
void displayEngrenageSecondes(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurRouge(1.2f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	float angle = (secondes * 6) % 360;
	glRotatef(angle-1, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 0.0, 0.45);
	gear(0.7, 1.2, 0.15, 30.0, 0.1);
	//glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'engrenage
void displayEngrenageMinutes(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBleu(1.2f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	float angle = (minutes * 6) % 360;
	glRotatef(angle - 1, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 0.0, 0.3);
	gear(0.6, 1.7, 0.15, 30.0, 0.1);
	//glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'engrenage
void displayEngrenageHeures(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurVert(1.2f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	float angle = (((heures % 12) * 30 + (minutes / 2)) % 360);
	glRotatef(angle - 1, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 0.0, 0.15);
	gear(0.5, 2.2, 0.15, 30.0, 0.1);
	//glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void displayCouvercle(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurJauneClair(1.0f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);

	glTranslatef(translateCouvX[animationCouv/5], translateCouvY, translateCouvZ[animationCouv/5]);
	glRotatef(angleCouv[animationCouv/5], rotateCouvX, rotateCouvY, rotateCouvZ);
	//printf("animation : %d", animationCouv);
	//printf("rotation : %f, %f, %f, %f", angleCouv[animationCouv], rotateCouvX[animationCouv], rotateCouvY[animationCouv], rotateCouvZ[animationCouv]);
	//printf("rotation : %f, %f, %f", translateCouvX[animationCouv], translateCouvY[animationCouv], translateCouvZ[animationCouv]);
	//glTranslatef(-10.0, 0.0, 10.5);
	//glRotatef(90, 0.0, -1.0, 0.0);
	glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void animationCouvercle(void)
{
	angleCouv[1] = 80.0;
	//translateCouvX[1] = -10.0;
	//translateCouvZ[1] = 11.4;
	translateCouvX[1] = -8.;
	translateCouvZ[1] = 11.4;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[2] = 70.0;
	translateCouvX[2] = -6.8;
	translateCouvZ[2] = 10.9;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[3] = 60.0;
	translateCouvX[3] = -5.3;
	translateCouvZ[3] = 9.8;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[4] = 50.0;
	translateCouvX[4] = -4.;
	translateCouvZ[4] = 8.8;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[5] = 40.0;
	translateCouvX[5] = -2.9;
	translateCouvZ[5] = 7.6;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[6] = 30.0;
	translateCouvX[6] = -1.9;
	translateCouvZ[6] = 6.3;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[7] = 20.0;
	translateCouvX[7] = -1.2;
	translateCouvZ[7] = 4.7;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[8] = 10.0;
	translateCouvX[8] = -0.5;
	translateCouvZ[8] = 3.2;
	//glutPostRedisplay();
	//Sleep(500);
	angleCouv[9] = 0.0;
	translateCouvX[9] = 0.0;
	translateCouvZ[9] = 1.6;
	//glutPostRedisplay();
	

	rotateCouvX = 0.0;	rotateCouvY = -1.0;	rotateCouvZ = 0.0;	
	translateCouvX[0] = -10.0;	translateCouvY = 0.0;	translateCouvZ[0] = 11.5;
	angleCouv[0] = 90.0;
}

// dessin du verre
void displayVerre(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBlanc(0.25f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	glTranslatef(0.0, 0.0, 1.5);
	glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'aiguille des secondes
void displaySecondes(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurRouge(2.0f));

	float angle = (secondes * 6) % 360;
	//printf("%d\n", secondes);
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 4.5, 1.51);
	glScalef(0.5, 4.5, 0.0);
	glutSolidOctahedron();
	//glutSolidSphere(1.25, 50, 50);
	glPopMatrix();
	glPopMatrix();
}

void displayMarquages(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(2.0f));


	//glPushMatrix();
	//float angle = (instant.tm_min * 6) % 360;
	int i = 0;
	for (i = 0; i < 12; i++)
	{
		glPushMatrix();
		glRotatef(30*i, 0.0, 0.0, -0.5);
		glTranslatef(0.0, 9.1, 1.51);
		glRotatef(35, 0.0, 0.0, 0.5);
		glScalef(1.0, 1.0, 0.0);
		glutSolidTetrahedron();
		glPopMatrix();
	}

	i = 0;
	for (i = 1; i < 60; i++)
	{
		if (i % 5 != 0){
			glPushMatrix();
			glRotatef(6 * i, 0.0, 0.0, 0.5);
			glTranslatef(0.0, 9.4, 1.51);
			glRotatef(35, 0.0, 0.0, 0.5);
			glScalef(0.5, 0.5, 0.0);
			glutSolidTetrahedron();
			glPopMatrix();
		}
	}
	
	//glutSolidOctahedron();
	//glutSolidSphere(1.25, 50, 50);
	
	glPopMatrix();
}

// dessin de l'aiguille des minutes
void displayMinutes(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(2.0f));

	float angle = (minutes * 6) % 360;
	//printf("salut mintute %d", instant.tm_min);
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 4.0, 1.51);
	glScalef(0.35, 4.0, 0.3);
	glutSolidOctahedron();
	//glutSolidSphere(1.25, 50, 50);
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'aiguille des minutes
void displayHeures(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(2.0f));

	float angle = (((heures %12) * 30 + (minutes/2)) % 360);
	//printf("%d",instant.tm_min/2);
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 3.0, 1.51);
	glScalef(0.5, 3.0, 0.3);
	glutSolidOctahedron();
	//glutSolidSphere(1.25, 50, 50);
	glPopMatrix();
	glPopMatrix();
}

void display(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	// gestion de la camera tournante
	gluLookAt(0, 0, distanceCamera, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(angley, 1.0, 0.0, 0.0);
	glRotatef(anglex, 0.0, 1.0, 0.0);
	//glPushMatrix();
	//glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor3d(0, 0, 0); // Texte en blanc
	displayChiffres();

	displayCouvercle();
	displaySecondes();
	displayMinutes();
	displayHeures();
	
	displayMarquages();
	displayEngrenageSecondes();
	displayEngrenageMinutes();
	displayEngrenageHeures();
	displayBalancier();
	displayAncre();
	displayRoueEchappement();

	if (!texture)
		displayTexture();

	displayCadran();
	displayVerre();
	/*
	glPushMatrix();
	displayCrochet();
	glPopMatrix();
	*/
	//glPopMatrix();
	glFlush();
	glutSwapBuffers();

}
/*
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, 0, 25);
	glOrtho(-50.0, 50.0, -50.0*h / w, 50.0*h / w, -100.0, 100.0);
	// pour la camera je donne sa position, où elle regarde (point visé) et l'orientation de la tête (vecteur)
	//gluLookAt(0, 0, 0, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glLoadIdentity();
}
*/

//la gestion du clavier
void clavier(unsigned char touche, int x, int y)
{
	
	switch (touche)
	{
		/*
		// faire tourner la camera
		
	case 'z':angle += 2;
		if (angle >= 360) angle -= 360; glutPostRedisplay(); break;
	case 'a':angle -= 2;
		if (angle <= 0) angle += 360; glutPostRedisplay(); break;
		// allumer et eteindre les lumieres
	case 'w': glEnable(GL_LIGHT0); glutPostRedisplay(); break;
	case 'x': glDisable(GL_LIGHT0); glutPostRedisplay(); break;
	case 'c': glEnable(GL_LIGHT1); glutPostRedisplay(); break;
	case 'v': glDisable(GL_LIGHT1); glutPostRedisplay(); break;
		// gestion de la specularité
	case 'm': Mspec[0] += 0.1; if (Mspec[0] > 1) Mspec[0] = 1;
		Mspec[1] += 0.1; if (Mspec[1] > 1) Mspec[1] = 1;
		Mspec[2] += 0.1; if (Mspec[2] > 1) Mspec[2] = 1;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Mspec);
		glutPostRedisplay(); break;
	case 'l': Mspec[0] -= 0.1; if (Mspec[0] < 0) Mspec[0] = 0;
		Mspec[1] -= 0.1; if (Mspec[1] < 0.9) Mspec[1] = 0.9;
		Mspec[2] -= 0.1; if (Mspec[2] < 0) Mspec[2] = 0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Mspec);
		glutPostRedisplay(); break;
		// gestion de la brillance
	case 'j': Mshiny -= 1; if (Mshiny < 0) Mshiny = 0;
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Mshiny);
		glutPostRedisplay(); break;
	case 'k': Mshiny += 1; if (Mshiny > 128) Mshiny = 128;
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Mshiny);
		glutPostRedisplay(); break;
		*/
	case 'o': if (open) open = false; else open = true; break;
		//animationCouv++; break;
	case 'l':
		if (glIsEnabled(GL_LIGHT0))
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0); 
		glutPostRedisplay(); break;
	case 'm': minutes = minutes++ % 60; if (minutes >= 60) minutes -= 60;
		majTemps();// glutPostRedisplay(); 
		break;
	case 'M': minutes = minutes-- % 60; if (minutes <= -1){ minutes += 60; }
		recule = true;
		majTemps();// glutPostRedisplay(); 
		recule = false;
		break;

	case 's': secondes = secondes++ % 60;if (secondes >= 60) secondes -= 60;
		/*tempsSynchro = false;*/ majTemps(); break;
	case 'S': secondes = secondes-- % 60; if (secondes <= -1) secondes += 60;
		reculeSec = true;
		majTemps();
		reculeSec = false; 
		break;

	case 'h': heures = heures++ % 24; majTemps(); if (heures >= 24) heures -= 24; break;
	case 'H': heures = heures-- % 24; majTemps(); if (heures <= -1) heures += 24; break;

	case 't': if (coefTransparence == 1.0) coefTransparence = 0.2; else coefTransparence = 1.0;
		if (texture) texture = false; else texture = true; break;
	case 'z': distanceCamera--; break;
	case 'Z': distanceCamera++; break;
	case 'r': heures = instant.tm_hour;
		minutes = instant.tm_min;
		secondes = instant.tm_sec;
		tempsSynchro = true;
		printf(" remise à l'heure \n");
		break;
	case 'a': if (rapide == false){
				  rapide = true;
				  printf(" vitesse rapide \n");
				}
			  else {
				  rapide = false;
				  printf(" vitesse normale \n");
			  }
			  break;
	case 'p': if (pause == false){
				  pause = true;
				  printf(" pause \n");
				}
			  else { 
				  pause = false;
				  printf(" reprise \n");
			  }
			  break;
		// fin de programme
	case 'q': exit(0);
	}
}

void speciale(int k, int x, int y)
{
	printf("test");
	switch (k)
	{
		
	case GLUT_KEY_UP: yCam = (yCam + 1) % distanceCamera/2;
		if (yCam >= distanceCamera/2) yCam -= distanceCamera/2; glutPostRedisplay(); break;
	case GLUT_KEY_DOWN: yCam = (yCam - 2) % 360; 
		if (yCam <= 0) yCam += 360; glutPostRedisplay(); break;
	case GLUT_KEY_LEFT: xCam = (xCam + 2) % 360; break;
	case GLUT_KEY_RIGHT: xCam = (xCam - 2) % 360; break;
	//case GLUT_KEY_CTRL_L: hand = (hand + 5) % 360; break;
	//case GLUT_KEY_CTRL_R: hand = (hand - 5) % 360; break;
	}
	glutPostRedisplay();

}
/*
void calcTabCosSin(void)
{
	int i;
	for (i = 0; i < 360; i++)
	{
		Cos[i] = cos((float)i / 100.0*PI);
		Sin[i] = sin((float)i / 100.0*PI);
	}
}*/

// fonction de recalcul de la taille de la fenetre
void reshape(int x, int y)
{
	if (x > y)
		glViewport(0, (y - x) / 2, x, x);
	else
		glViewport((x - y) / 2, 0, y, y);

}

void mouse(int button, int state, int x, int y)
{
	/* si on appuie sur le bouton gauche */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		presse = 1; /* le booleen presse passe a 1 (vrai) */
		xold = x; /* on sauvegarde la position de la souris */
		yold = y;
	}
	/* si on relache le bouton gauche */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		presse = 0; /* le booleen presse passe a 0 (faux) */
	if (button == 3){
		distanceCamera++;
		printf("dezoom \n");
	}if (button == 4){
		distanceCamera--;
		printf("dezoom \n");
	}
}

void mousemotion(int x, int y)
{
	if (presse) /* si le bouton gauche est presse */
	{
		/* on modifie les angles de rotation de l'objet
		en fonction de la position actuelle de la souris et de la derniere
		position sauvegardee */
		anglex = anglex + (x - xold);
		angley = angley + (y - yold);
		glutPostRedisplay(); /* on demande un rafraichissement de
							 l'affichage */
	}

	xold = x; /* sauvegarde des valeurs courante de le position de la souris
			  */
	yold = y;
}

int main(int argc, char** argv)
{
	/* Gestion de creation de fenetre*/
	glutInit(&argc, argv);
	// on travaille en rgba avec un double buffer et en profondeur
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(400, 350);
	glutCreateWindow("Montre OpenGL");
	myInit();
	creationMenuBasique();

	// mise en place de la perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 500.0); // angle, repport h/w, plan de clipping
	glMatrixMode(GL_MODELVIEW);
	animationCouvercle();
	
	/* Ne fonctionnne pas pour le deplacement de la camera */
	//setParametresOrthoBasique(-25.0, 25.0, -25.0, 25.0, -500.0, 500.0);
	//setManipulateurDistance(0.1f);
	

	// permet de mettre à jour les aiguilles
	glutIdleFunc(myIdle);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(clavier);
	glutSpecialFunc(specialBasique);
	glutMotionFunc(motionBasique);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);
	glutDisplayFunc(display);
	
	//print du temps systeme
	systemTime();

	// entrée de la boucle principale
	glutMainLoop();
	return(0);
}