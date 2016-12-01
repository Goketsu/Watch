
/* Gestion des menus                */

#include <gl/freeglut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
#include <stdlib.h>

static void select(int selection) {
  switch (selection) {
    case 0  : exit(0); }
  glutPostRedisplay();
}

void creationMenuBasique(void) {
  glutCreateMenu(select);
  glutAddMenuEntry("Quitter",0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
