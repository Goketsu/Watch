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

// fichiers pour les differentes textures
char texFileName[] = "hexagonalTrianglesRotate.ppm";
char texFileName2[] = "hexagonalTrianglesRotate2.ppm";
char texFileName3[] = "hexagonalTrianglesRotate3.ppm";
PPMImage *image;

// numero de la texture actuelle
int numeroTex = 1;

// booleen pour savoir si on affiche ou non la texture
bool texture = false;

// numero de la couleur choisi pour la montre
static int couleur = 11;

// gestion du temps systeme
static struct tm instant;
int heures, minutes, secondes;

// booleen pour savoir si le couvercle est ouvert ou non
bool open = true;
// boolean pour savoir si on a mis en pause
bool pause = false;
// booleen pour savoir si on accelere
bool rapide = false;
// booleen pour savoir si on recule l'aiguille des minutes
bool recule = false;
// booleen pour savoir si on recule l'aiguille des secondes
bool reculeSec = false;
// booleen pour savoir si on est synchroniser avec le temps systeme
bool tempsSynchro = true;
// position de la camera
int xCam = 0, yCam = 45, zCam = 0;
// distance de la camera
int distanceCamera = 38;
// coefficient de transparence (0 = invisible, 1 = visible)
float coefTransparence = 1;
// gestion de la position de la camera
int anglex, angley, x, y, xold, yold;
// booleen pour savoir si le bouton gauche de la souris en enfoncé
char presse;
#define PI 3.14159265

// gestion de la position du couvercle pour permettre son animation
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

	//printf("%d/%d ; %d:%d:%d\n", instant.tm_mday, instant.tm_mon + 1, instant.tm_hour, instant.tm_min, instant.tm_sec);
	printf("%d:%d:%d\n", heures, minutes, secondes);
}

// permet de modifier l'heure interne en avancant/reculant les aiguilles
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
			//printf("min --");
		}
	}
	if (secondes == 0 && reculeSec == false){
		minutes = minutes++ % 60;
		if (minutes >= 60) minutes -= 60;
		//printf("min ++");
	}
	if (recule || reculeSec && secondes == 59){
		if (minutes == 59){
			//printf("heure --");
			heures = heures-- % 24;
			if (heures <= -1) heures += 24;
			recule = false;
		}
	}
	
	if (minutes == 0 && recule == false && reculeSec == false){
		//printf("heure ++");
		heures = heures++ % 24;
		if (heures >= 24) heures -= 24;
	}
}

// fonction qui est appelé en permanence quand il n'y a aucune autre action
void myIdle(void)
{
	
	time_t temps;
	struct tm newInstant;

	// animation du couvercle
	if (open == false){
		if (animationCouv < 18){
			animationCouv++;
		}
	}
	if (open == true){
		if (animationCouv > 0){
			animationCouv--;
		}
	}
	
	// gestion de l'acceleration
	if (rapide == true){
		secondes = secondes++ % 60;
		if (secondes >= 60) secondes -= 60;
		if (secondes == 0){
			minutes = minutes++ % 60;
			if (minutes >= 60) minutes -= 60;
			//printf("min ++");
		}
		if (minutes == 0 && secondes == 0){
			heures = heures++ % 24;
			if (heures >= 24) heures -= 24;
		}
	}
	
	// gestion de l'animation normale de la montre
	time(&temps); 
	localtime_s(&newInstant, &temps);
	if (newInstant.tm_sec != instant.tm_sec && tempsSynchro == true)
	{
		
		instant = newInstant;
		if (rapide == false && pause == false){
			
			secondes = secondes++ % 60;
			if (secondes >= 60) secondes -= 60;
			if (secondes == 0){
				minutes = minutes++ % 60;
				if (minutes >= 60) minutes -= 60;
				//printf("Idlemin ++");
			}
			if (minutes == 0 && secondes == 0){
				heures = heures++ % 24;
				if (heures >= 24) heures -= 24;
			}
			
		}
	}
	
	// on reaffiche si on est pas en pause
	if (pause == false){
		glutPostRedisplay(); // reaffiche la scène
	}
}

// gestion des lumieres et textures
void myInit(void)
{
	image = new PPMImage(texFileName);
	
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
	
	// on defini la position des lumieres
	GLfloat light_position[] = { 1.0, 1.0, 20.0, 0.0 };
	glEnable(GL_LIGHTING);
	// on defini les parametres de la source 0
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, couleurNoir());
	glLightfv(GL_LIGHT0, GL_SPECULAR, couleurNoir());
	glLightfv(GL_LIGHT0, GL_DIFFUSE, couleurRouge());
	glEnable(GL_LIGHT0);
	// on defini les parametres de la source 1
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, couleurNoir());
	glLightfv(GL_LIGHT1, GL_SPECULAR, couleurNoir());
	glLightfv(GL_LIGHT1, GL_DIFFUSE, couleurBleu());
	glEnable(GL_LIGHT1);
	// on defini les parametres de la source 2
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, couleurNoir());
	glLightfv(GL_LIGHT2, GL_SPECULAR, couleurNoir());
	glLightfv(GL_LIGHT2, GL_DIFFUSE, couleurVert());
	glEnable(GL_LIGHT2);

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

// fonction pour creer un engrenage
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

// affichage de la texture au dos de la montre en fonction de celle choisi
void displayTexture(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	if (numeroTex == 1)
		image = new PPMImage(texFileName);
	else if (numeroTex == 2)
		image = new PPMImage(texFileName2);
	else
		image = new PPMImage(texFileName3);

	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image->data);
	delete image;
	
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.05, 0.75); glVertex3f(-8.0, 4.4, -0.01);
	glTexCoord2f(0.05, 0.25); glVertex3f(-8.0, -5.4, -0.01);
	glTexCoord2f(0.5, 0.0); glVertex3f(0.0, -10.0, -0.01);
	glTexCoord2f(0.95, 0.25); glVertex3f(8, -5.4, -0.01);
	glTexCoord2f(0.95, 0.75); glVertex3f(8, 4.6, -0.01);
	glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 10.0, -0.01);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

// affichage du chiffre romain XII
void displayChiffre12(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-0.8, 7.8, 1.5);

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.95, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain XI
void displayChiffre11(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-3.9, 6.6, 1.5);

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.05, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain X
void displayChiffre10(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-6.5, 3.5, 1.5);

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain IX
void displayChiffre9(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-7.1, 0.0, 1.5);

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.05, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain VIII
void displayChiffre8(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-6.7, -3.7, 1.5);

	glPushMatrix();
	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.1, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain VII
void displayChiffre7(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-3.9, -6.6, 1.5);

	glPushMatrix();
	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix(); 
	
	glPushMatrix();
	glTranslatef(1.5, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain VI
void displayChiffre6(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(-0.5, -7.8, 1.5);

	glPushMatrix();
	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain V
void displayChiffre5(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(3.9, -6.6, 1.5);
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

// affichage du chiffre romain IV
void displayChiffre4(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));
	glTranslatef(6.7, -3.7, 1.5);

	glPushMatrix();
	glRotatef(30, 0.0, 0.0, -1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glScalef(0.5, 1.5, 0.0);
	glutSolidCube(1.0); 
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1.5, 0.0, 0.0);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain III
void displayChiffre3(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));

	glPushMatrix();
	glTranslatef(6.8, 0.0, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.45, 0.0, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.1, 0.0, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain II
void displayChiffre2(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));

	glPushMatrix();
	glTranslatef(6.3, 3.5, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.95, 3.5, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

// affichage du chiffre romain I
void displayChiffre1(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));

	glPushMatrix();
	glTranslatef(3.9, 6.6, 1.5);
	glScalef(0.5, 1.4, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

// affichage de tous les chiffres
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
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	if (couleur == 11){
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurJauneClair(coefTransparence));
	}
	if (couleur == 12){
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBlanc(coefTransparence));
	}
	if (couleur == 13)
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBleuCielFonce(coefTransparence));

	/* Anneau pour la chaine */
	glPushMatrix();
	glTranslatef(0.0, 13.1, 0.75);
	glutSolidTorus(0.3, 1.2, 50, 50);
	glPopMatrix();

	/* Bouton pour modifier l'heure theorique */
	glPushMatrix();
	glTranslatef(0.0, 13.0, 0.75);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(1, 1.5, 50, 50);
	glPopMatrix();

	/* Attache au dessus de la montre */
	glPushMatrix();
	glTranslatef(0.0, 12.0, 0.75);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.5, 3.0, 50, 50);
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
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurMagenta(1.2f));
	glPushMatrix();
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
	glutSolidCylinder(0.03, 0.15, 50, 50);
	glPopMatrix();

	glPushMatrix();
	if (secondes%2 == 0)
		glTranslatef(2.8, 0.65, 0.77);
	else
		glTranslatef(2.4, 1.05, 0.77);
	glutSolidCylinder(0.03, 0.15, 50, 50);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'ancre qui transmet la force
void displayAncre(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurMagentaFonce(1.2f));
	glPushMatrix();
	
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

	glPopMatrix();
	glPopMatrix();
}

// dessin de la roue d'échappement qui compte les oscillations du balancier
void displayRoueEchappement(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurCyanFonce(1.2f));
	glPushMatrix();
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

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'engrenage des secondes
void displayEngrenageSecondes(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurRouge(1.2f));
	glPushMatrix();
	float angle = (secondes * 6) % 360;
	glRotatef(angle-1, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 0.0, 0.45);
	gear(0.7, 1.2, 0.15, 30.0, 0.1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'engrenage des minutes
void displayEngrenageMinutes(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBleu(1.2f));
	glPushMatrix();
	float angle = (minutes * 6) % 360;
	glRotatef(angle - 1, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 0.0, 0.3);
	gear(0.6, 1.7, 0.15, 30.0, 0.1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'engrenage des heures
void displayEngrenageHeures(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurVert(1.2f));
	glPushMatrix();
	float angle = (((heures % 12) * 30 + (minutes / 2)) % 360);
	glRotatef(angle - 1, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 0.0, 0.15);
	gear(0.5, 2.2, 0.15, 30.0, 0.1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void displayCouvercle(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	if (couleur == 11)
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurJauneClair(1.0f));
	if (couleur == 12)
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBlanc(1.0f));
	if (couleur == 13)
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBleuCielFonce(1.0f));
	glPushMatrix();

	glTranslatef(translateCouvX[animationCouv/2], translateCouvY, translateCouvZ[animationCouv/2]);
	glRotatef(angleCouv[animationCouv/2], rotateCouvX, rotateCouvY, rotateCouvZ);
	glutSolidCone(10.0, 0.8, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void animationCouvercle(void)
{
	angleCouv[1] = 80.0;
	translateCouvX[1] = -8.;
	translateCouvZ[1] = 11.4;

	angleCouv[2] = 70.0;
	translateCouvX[2] = -6.8;
	translateCouvZ[2] = 10.9;

	angleCouv[3] = 60.0;
	translateCouvX[3] = -5.3;
	translateCouvZ[3] = 9.8;
	
	angleCouv[4] = 50.0;
	translateCouvX[4] = -4.;
	translateCouvZ[4] = 8.8;
	
	angleCouv[5] = 40.0;
	translateCouvX[5] = -2.9;
	translateCouvZ[5] = 7.6;
	
	angleCouv[6] = 30.0;
	translateCouvX[6] = -1.9;
	translateCouvZ[6] = 6.3;
	
	angleCouv[7] = 20.0;
	translateCouvX[7] = -1.2;
	translateCouvZ[7] = 4.7;
	
	angleCouv[8] = 10.0;
	translateCouvX[8] = -0.5;
	translateCouvZ[8] = 3.2;
	
	angleCouv[9] = 0.0;
	translateCouvX[9] = 0.0;
	translateCouvZ[9] = 1.6;

	rotateCouvX = 0.0;	rotateCouvY = -1.0;	rotateCouvZ = 0.0;	
	translateCouvX[0] = -10.0;	translateCouvY = 0.0;	translateCouvZ[0] = 11.5;
	angleCouv[0] = 90.0;
}

// dessin du verre
void displayVerre(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBlanc(0.25f));
	//glPushMatrix();
	glTranslatef(0.0, 0.0, 1.5);
	glutSolidCone(10.0, 0.8, 50, 50);
	//glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin de l'aiguille des secondes
void displaySecondes(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurRouge(2.0f));

	float angle = (secondes * 6) % 360;
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 4.5, 1.51);
	glScalef(0.5, 4.5, 0.0);
	glutSolidOctahedron();
	glPopMatrix();
	glPopMatrix();
}

// affiche les marquages (triangle) sur les contours du cadran
void displayMarquages(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(2.0f));

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
	
	glPopMatrix();
}

// dessin de l'aiguille des minutes
void displayMinutes(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(2.0f));

	float angle = (minutes * 6) % 360;
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 4.0, 1.51);
	glScalef(0.35, 4.0, 0.3);
	glutSolidOctahedron();

	glPopMatrix();
	glPopMatrix();
}

// dessin de l'aiguille des heures
void displayHeures(void)
{
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(2.0f));

	float angle = (((heures %12) * 30 + (minutes/2)) % 360);
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 3.0, 1.51);
	glScalef(0.5, 3.0, 0.3);
	glutSolidOctahedron();

	glPopMatrix();
	glPopMatrix();
}

// affichage principal
void display(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();

	// gestion de la camera tournante
	gluLookAt(0, 0, distanceCamera, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(angley, 1.0, 0.0, 0.0);
	glRotatef(anglex, 0.0, 1.0, 0.0);
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

	glFlush();
	glutSwapBuffers();

}

//la gestion du clavier
void clavier(unsigned char touche, int x, int y)
{
	
	switch (touche)
	{
	case '1': numeroTex = 1; printf("texture 1 séléctionnée \n"); break;
	case '2': numeroTex = 2; printf("texture 2 séléctionnée \n"); break;
	case '3': numeroTex = 3; printf("texture 3 séléctionnée \n"); break;
	case 'o': if (open) open = false; else open = true; break;

	case 'l':
		if (glIsEnabled(GL_LIGHT0)){
			glDisable(GL_LIGHT0);
			printf(" lumiére rouge éteinte \n");
		}
		else{
			glEnable(GL_LIGHT0);
			printf(" lumiére rouge allumée \n");
		}
		glutPostRedisplay(); break;
	case 'L':
		if (glIsEnabled(GL_LIGHT1)){
			glDisable(GL_LIGHT1);
			printf(" lumiére bleue éteinte \n");
		}
		else{
			glEnable(GL_LIGHT1);
			printf(" lumiére bleue allumée \n");

		}
		glutPostRedisplay(); break;
	case 'k':
		if (glIsEnabled(GL_LIGHT2)){
			glDisable(GL_LIGHT2);
			printf(" lumiére verte éteinte \n");
		}
		else{
			glEnable(GL_LIGHT2);
			printf(" lumiére verte allumée \n");
		}
		glutPostRedisplay(); break;
	case 'm': minutes = minutes++ % 60; if (minutes >= 60) minutes -= 60;
		majTemps();
		break;
	case 'M': minutes = minutes-- % 60; if (minutes <= -1){ minutes += 60; }
		recule = true;
		majTemps();
		recule = false;
		break;

	case 's': secondes = secondes++ % 60;if (secondes >= 60) secondes -= 60;
		majTemps(); break;
	case 'S': secondes = secondes-- % 60; if (secondes <= -1) secondes += 60;
		reculeSec = true;
		majTemps();
		reculeSec = false; 
		break;

	case 'h': heures = heures++ % 24; majTemps(); if (heures >= 24) heures -= 24; break;
	case 'H': heures = heures-- % 24; majTemps(); if (heures <= -1) heures += 24; break;

	case 't': if (coefTransparence == 1.0){
				  coefTransparence = 0.2;
				  printf(" montre transparente \n");
				}
			  else{
				  coefTransparence = 1.0;
				  printf(" montre mate \n");
			  }
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

// fonction de recalcul de la taille de la fenetre
void reshape(int x, int y)
{
	if (x > y)
		glViewport(0, (y - x) / 2, x, x);
	else
		glViewport((x - y) / 2, 0, y, y);

}

// fonction de gestion de la souris
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
		//printf("dezoom \n");
	}if (button == 4){
		distanceCamera--;
		//printf("dezoom \n");
	}
}

// gestion du deplacement de la souris
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

// fonction de selection de couleur associer au menu
void selectCouleur(int selection) {

	switch (selection) {

	case 11: /* si c'est une des trois premieres valeurs */

	case 12: 

	case 13: couleur = selection;// printf("couleur %d", couleur);

		break;

	case 0: exit(0);
	}

	glutPostRedisplay();

}

// fonction du menu quitter
void select(int selection) {

	switch (selection) {

	case 0: exit(0);
	}

	glutPostRedisplay();

}

// fonction principale du programme
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

	printf(" Voici quelques commandes utiles dans l'application : \n");
	printf(" a : accelere/ralenti le temps \n");
	printf(" z/Z : zoom et dezoom (également faisable à la souris) \n");
	printf(" r : remise à l'heure de la montre \n");
	printf(" t : rend la montre transparente/mate pour voir les mecanismes \n");
	printf(" o : ouverture/fermeture du couvercle \n");
	printf(" p : permet de mettre le temps en pause ou de reprendre \n");
	printf(" s/S : permet d'avancer/reculer l'aiguille des secondes \n");
	printf(" m/M : permet d'avancer/reculer l'aiguille des minutes \n");
	printf(" h\H : permet d'avancer/reculer l'aguille des heures \n");
	printf(" k : permet d'allumer/éteindre la lumière Verte \n");
	printf(" l : permet d'allumer/éteindre la lumière Rouge \n");
	printf(" L : permet d'allumer/éteindre la lumière Bleue \n");
	printf(" 1 : permet de choisir la première texture (par défaut) \n");
	printf(" 2 : permet de choisir la deuxième texture \n");
	printf(" 3 : permet de choisir la troisième texture \n");
	printf(" q : permet de quitter l'application \n");
	printf(" Un menu est également disponible via le clic droit de la souris \n");

	// on creer un menu pour modifier la couleur de la montre
	int menuCouleur = glutCreateMenu(selectCouleur);
	glutAddMenuEntry("Jaune", 11);
	glutAddMenuEntry("Blanc", 12);
	glutAddMenuEntry("Bleu", 13);

	/* et on lie les sous-menus */
	glutCreateMenu(select);
	glutAddSubMenu("Couleur", menuCouleur);
	glutAddMenuEntry("Quitter", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// mise en place de la perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 500.0); // angle, repport h/w, plan de clipping
	glMatrixMode(GL_MODELVIEW);
	animationCouvercle();	

	// permet de mettre à jour les aiguilles
	glutIdleFunc(myIdle);

	// toutes les fonctions necessaire pour gerer les entrées clavier ou souris
	glutReshapeFunc(reshape);
	glutKeyboardFunc(clavier);
	glutMotionFunc(motionBasique);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);

	// fonction d'affichage
	glutDisplayFunc(display);
	
	//print du temps systeme
	systemTime();

	// entrée de la boucle principale
	glutMainLoop();
	return(0);
}