
/* Un manipulateur souris et clavier      */
#include <Windows.h>

#include <gl/freeglut.h>
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int mouvement = 0 ;
static int button = 0 ;
static int modifier = 0 ;
static float rotx = 0;
static float roty = 0;
static float rotz = 0;
static float transx = 0;
static float transy = 0;
static float zoom = 1.0F;
static int px ;
static int py ;
static int tx ;
static float mt[16];
static float manipulatorDistance = 1.0F;
static GLfloat view_rotx=0.0F ;
static GLfloat view_roty=0.0F ;
static GLfloat view_rotz=0.0F ;
static int anim = 0;
static float fact = 1.0F;
static int idleWindow = -1;

static int getModifier() {
  int modif = 0;
  if ( (glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0 ) {
    if ( (glutGetModifiers() & GLUT_ACTIVE_CTRL) != 0 )
      modif = 2;
      else
      modif = 1; }
    else
    if ( (glutGetModifiers() & GLUT_ACTIVE_CTRL) != 0 )
      modif = 3;
      else
      if ( (glutGetModifiers() & GLUT_ACTIVE_ALT) != 0 )
        modif = 4;
  return(modif);
}

int sourisManipulateur(int bouton,int etat,int x,int y) {
  glGetFloatv(GL_PROJECTION_MATRIX,mt);
  tx = glutGet(GLUT_WINDOW_WIDTH);
  button = bouton;
  if ( bouton == GLUT_LEFT_BUTTON ) {
    modifier = getModifier();
    if ( modifier ) {
      px = x ;
      py = y ; 
      if ( etat == GLUT_DOWN ) {
        mouvement = 1 ;}
      if ( etat == GLUT_UP ) {
        mouvement = 0 ; } 
      return(1); } }
  return(0);
}

int motionManipulateur(int x,int y) {
  switch ( button ) {
    case GLUT_LEFT_BUTTON  : if ( mouvement == 1 ) {
                               switch ( modifier ) {
                                 case 1 : roty += (x-px) ;
                                          rotx += (y-py) ;
                                          px = x; 
                                          py = y;
                                          break;
                                 case 2 : rotz += (x-px) ;
                                          rotx += (y-py) ;
                                          px = x; 
                                          py = y;
                                          break;
                                 case 3 : if ( ( y-py ) > 0 )
                                            zoom *= pow(1.02F,(y-py)/5.0F) ;
                                          if ( ( y-py ) < 0 )
                                            zoom /= pow(1.02F,(py-y)/5.0F) ;
                                          py = y;
                                          break;
                                 case 4 : transx += (x-px)*2.0F/tx/mt[0]*manipulatorDistance ;
                                          transy -= (y-py)*2.0F/tx/mt[0]*manipulatorDistance ;
                                          px = x; 
                                          py = y;
                                          break; }
                               return(1); }
                             break;}
  return(0);
}

void setManipulateurSourisAngle(float ax,float ay,float az) {
  rotx = ax;
  roty = ay;
  rotz = az;
}

void manipulateurSouris() {
  glTranslatef(transx,transy,0.0F);
  glRotatef(rotx,1.0f,0.0F,0.0F);
  glRotatef(roty,0.0f,1.0F,0.0F);
  glRotatef(rotz,0.0f,0.0F,1.0F);
  glScalef(zoom,zoom,zoom);
}

void setManipulateurDistance(float d) {
  manipulatorDistance = d;
}

int keyManipulateur(unsigned char key,int x,int y) {
  modifier = getModifier();
  if ( modifier == 1 ) {
    switch ( key ) {
      case 0x0D : anim = (anim+1)%2;
                  return(1);
                  break; } }
  switch ( key ) {
    case 'x'    : view_rotx +=2 ;
                  return(1);
                  break;
    case 'X'    : view_rotx -=2 ;
                  return(1);
                  break;
    case 'y'    : view_roty +=2 ;
                  return(1);
                  break;
    case 'Y'    : view_roty -=2 ;
                  return(1);
                  break;
    case 'z'    : view_rotz +=2 ;
                  return(1);
                  break;
    case 'Z'    : view_rotz -=2 ;
                  return(1);
                  break;
    case '\033' : exit(0);
                  break ; }
  return(0);
}

int specialManipulateur(int k,int x,int y) {
  modifier = getModifier();
  if ( modifier == 1 ) {
    switch (k) {
      case GLUT_KEY_LEFT      : 
      case GLUT_KEY_PAGE_UP   : 
      case GLUT_KEY_UP        : fact *= 1.1F;
                                return(1);
                                break;
      case GLUT_KEY_RIGHT     : 
      case GLUT_KEY_PAGE_DOWN : 
      case GLUT_KEY_DOWN      : fact /= 1.1F;
                                return(1);
                                break; } }
  if ( modifier == 3 ) {
    switch (k) {
      case GLUT_KEY_UP        : view_rotx = 0.0F;
                                view_roty = 0.0F;
                                view_rotz = 0.0F;
                                rotx = 0;
                                roty = 0;
                                rotz = 0;
                                return(1);
                                break;
      case GLUT_KEY_DOWN      : view_rotx = 180.0F;
                                view_roty = 0.0F;
                                view_rotz = 0.0F;
                                rotx = 0;
                                roty = 0;
                                rotz = 0;
                                return(1);
                                break;
      case GLUT_KEY_LEFT      : view_roty = 0.0F;
                                view_rotx = 0.0F;
                                view_rotz = 0.0F;
                                rotx = 0;
                                roty = 0;
                                rotz = 0;
                                return(1);
                                break;
      case GLUT_KEY_RIGHT     : view_roty = 180.0F;
                                view_rotx = 0.0F;
                                view_rotz = 0.0F;
                                rotx = 0;
                                roty = 0;
                                rotz = 0;
                                return(1);
                                break;
      case GLUT_KEY_PAGE_DOWN : view_rotz = 0.0F;
                                view_rotx = 0.0F;
                                view_roty = 0.0F;
                                rotx = 0;
                                roty = 0;
                                rotz = 0;
                                return(1);
                                break;
      case GLUT_KEY_PAGE_UP   : view_rotz = 180.0F;
                                view_rotx = 0.0F;
                                view_roty = 0.0F;
                                rotx = 0;
                                roty = 0;
                                rotz = 0;
                                return(1);
                                break; }
    return(0); }
  float dif;
  switch(modifier) {
    case 0 : dif = 1.0F;
             break;
    case 4 : dif = 10.0F;
             break; }
  switch (k) {
    case GLUT_KEY_UP        : view_rotx -= dif;
                              return(1);
                              break;
    case GLUT_KEY_DOWN      : view_rotx += dif;
                              return(1);
                              break;
    case GLUT_KEY_LEFT      : view_roty -= dif;
                              return(1);
                              break;
    case GLUT_KEY_RIGHT     : view_roty += dif;
                              return(1);
                              break;
    case GLUT_KEY_PAGE_DOWN : view_rotz += dif;
                              return(1);
                              break;
    case GLUT_KEY_PAGE_UP   : view_rotz -= dif;
                              return(1);
                              break; }
  return(0);
}

void manipulateurClavier() {
  glRotatef(view_rotx,1.0,0.0,0.0);
  glRotatef(view_roty,0.0,1.0,0.0);
  glRotatef(view_rotz,0.0,0.0,1.0);
}

void setManipulateurClavierAngle(float ax,float ay,float az) {
  view_rotx = ax;
  view_roty = ay;
  view_rotz = az;
}

void sourisBasique(int bouton,int etat,int x,int y) {
  if ( sourisManipulateur(bouton,etat,x,y) )
    glutPostRedisplay();
}

void motionBasique(int x,int y) {
  if ( motionManipulateur(x,y) )
    glutPostRedisplay();
}

void keyBasique(unsigned char key,int x,int y) {
  if ( keyManipulateur(key,x,y) )
    glutPostRedisplay();
}

void specialBasique(int k,int x,int y) {
  if ( specialManipulateur(k,x,y) )
    glutPostRedisplay();
}

void setIdleWindow(int w) {
  idleWindow = w;
}

void idleBasique(void) {
  if ( anim ) {
    rotx += (1.1F*fact);
    roty += (2.1F*fact);
    rotz += (3.1F*fact);
    if ( idleWindow == -1 )
      glutPostRedisplay();
      else
      glutPostWindowRedisplay(idleWindow); }
}

