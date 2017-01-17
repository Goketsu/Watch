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
int distanceCamera;

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
	//printf("new test %d \n", newInstant.tm_sec);
	//printf("test %d \n",instant.tm_sec);
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

/* Fonction qui ne fonctionne pas avec les autres ?MYSTERE? */
void displayCrochet(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

// dessin du disque de la montre
void displayCadran(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurJauneClair(1.0f));
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

// dessin du verre
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

// dessin de l'aiguille des secondes
void displaySecondes(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	manipulateurSouris();
	manipulateurClavier();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleurNoir(2.0f));

	float angle = (instant.tm_sec * 6) % 360;
	//printf("salut %d", instant.tm_sec);
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 4.8, 2.4);
	glScalef(0.5, 5.0, 0.3);
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
		glTranslatef(0.0, 9.1, 2.0);
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
			glTranslatef(0.0, 9.4, 2.0);
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

	float angle = (instant.tm_min * 6) % 360;
	//printf("salut mintute %d", instant.tm_min);
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 4.0, 2.4);
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

	float angle = (((instant.tm_hour %12) * 30 + (instant.tm_min/2)) % 360);
	//printf("%d",instant.tm_min/2);
	glRotatef(angle, 0.0, 0.0, -0.5);

	glTranslatef(0.0, 3.0, 2.4);
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
	distanceCamera = 38;
	gluLookAt(0, 0, distanceCamera, 0, 0, 0, 0, 1, 0);
	//glPushMatrix();
	displayCadran();
	display2();
	
	displaySecondes();
	displayMinutes();
	displayHeures();
	
	displayMarquages();
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

// fonction de recalcul de la taille de la fenetre
void reshape(int x, int y)
{
	if (x > y)
		glViewport(0, (y - x) / 2, x, x);
	else
		glViewport((x - y) / 2, 0, y, y);

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
	gluPerspective(45.0, 1.0, 0.1, 50.0); // angle, repport h/w, plan de clipping
	glMatrixMode(GL_MODELVIEW);
	
	/* Ne fonctionnne pas pour le deplacement de la camera */
	//setParametresOrthoBasique(-25.0, 25.0, -25.0, 25.0, -500.0, 500.0);
	//setManipulateurDistance(0.1f);
	

	// permet de mettre à jour les aiguilles
	glutIdleFunc(myIdle);

	glutReshapeFunc(reshape);
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