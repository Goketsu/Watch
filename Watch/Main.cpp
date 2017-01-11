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

struct tm instant;

//gestion du temps systeme
void systemTime(void)
{
	time_t secondes;
	//struct tm instant;

	time(&secondes);
	localtime_s(&instant, &secondes);

	printf("%d/%d ; %d:%d:%d\n", instant.tm_mday, instant.tm_mon + 1, instant.tm_hour, instant.tm_min, instant.tm_sec);
}

void myIdle(void)
{
	
	time_t secondes;
	//struct tm instant;
	struct tm newInstant;

	time(&secondes); 
	localtime_s(&newInstant, &secondes);
	if (&newInstant.tm_sec != &instant.tm_sec)
	{
		instant = newInstant;
		//float angle = (instant.tm_sec / 10) % 360;
	}
	printf("new test %d \n", newInstant.tm_sec);
	printf("test %d \n",instant.tm_sec);
	glutPostRedisplay(); // reaffiche la scène
}

// gestion des lumieres et melanges
void myInit(void)
{
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	// façon de gerer le modele d'eclairage
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

}

// dessin du disque de la montre
void display1(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurJauneClair(0.5f));
	//glScalef(17.0, 5.0, 10.0);
	glutSolidCylinder(10.0, 1.5, 50, 50);
	glPopMatrix();
	glPopMatrix();
}

// dessin du deuxieme cube
void display2(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurBlanc(0.8f));
	glPushMatrix();
	//glScalef(9.0, 9.0, 18.0);
	glTranslatef(0.0, 0.0, 1.5);
	glutSolidCone(10.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

// dessin du deuxieme cube
void display3(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(1.0f));

	float angle = (instant.tm_sec * 6) % 360;
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 4.0, 3.0);
	glScalef(0.5, 5.0, 0.3);
	glutSolidOctahedron();
	//glutSolidSphere(1.25, 50, 50);
	glPopMatrix();
	glPopMatrix();
}

void display(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	display1();
	display2();
	display3();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();

}

int main(int argc, char** argv)
{
	printf("ca craint");
	//glutIdleFunc(myIdle);
	//printf("c'est le début");

	/* Gestion de creation de fenetre*/
	glutInit(&argc, argv);
	// on travaille en rgba avec un double buffer et en profondeur
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(400, 350);
	glutCreateWindow("Test de transparence");
	myInit();
	creationMenuBasique();
	setParametresOrthoBasique(-11.0, 11.0, -11.0, 11.0, -500.0, 500.0);
	setManipulateurDistance(1.0f);

	glutIdleFunc(myIdle);

	glutReshapeFunc(reshapeOrthoBasique);
	glutKeyboardFunc(keyBasique);
	glutSpecialFunc(specialBasique);
	glutMotionFunc(motionBasique);
	glutMouseFunc(sourisBasique);
	glutDisplayFunc(display);
	
	//print du temps systeme
	systemTime();

	// entrée de la boucle principale
	glutMainLoop();
	return(0);
}