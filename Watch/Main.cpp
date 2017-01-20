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

static struct tm instant;
int heures, minutes, secondes;
bool open = true, pause = false;
bool rapide = false;
int xCam = 0, yCam = 45, zCam = 0;
float Sin[360], Cos[360]; // pas de 1 pour les calcul
int distanceCamera = 38;
#define PI 3.14159265

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

void myIdle(void)
{
	
	time_t temps;
	//struct tm instant;
	struct tm newInstant;
	
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
	if (newInstant.tm_sec != secondes)
	{
		
		//printf("new test %d \n", newInstant.tm_sec);
		//instant = newInstant;
		//secondes = instant.tm_sec;
		if (rapide == false && pause == false){
			
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

/* Fonction qui ne fonctionne pas avec les autres ?MYSTERE? 
Probablement du au glClear en début de fonction
*/
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
	
	glTranslatef(-10.0, 0.0, 10.5);
	glRotatef(90, 0.0, -1.0, 0.0);
	glutSolidCone(9.0, 0.8, 50, 50);
	//glutSolidCube(1.0);
	glPopMatrix();
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

	float angle = (secondes * 6) % 360;
	//printf("%d\n", secondes);
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

	float angle = (minutes * 6) % 360;
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

	float angle = (((heures %12) * 30 + (minutes/2)) % 360);
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
	gluLookAt(0, 0, distanceCamera, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glPushMatrix();
	displayCadran();
	displayCouvercle();
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
	case 'r': heures = instant.tm_hour;
		minutes = instant.tm_min;
		secondes = instant.tm_sec;
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

void calcTabCosSin(void)
{
	int i;
	for (i = 0; i < 360; i++)
	{
		Cos[i] = cos((float)i / 100.0*PI);
		Sin[i] = sin((float)i / 100.0*PI);
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
	
	/* Ne fonctionnne pas pour le deplacement de la camera */
	//setParametresOrthoBasique(-25.0, 25.0, -25.0, 25.0, -500.0, 500.0);
	//setManipulateurDistance(0.1f);
	

	// permet de mettre à jour les aiguilles
	glutIdleFunc(myIdle);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(clavier);
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