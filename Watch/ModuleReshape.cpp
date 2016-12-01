
/* Module de gestion des transformations  */
/* de visualisation                       */
#include <Windows.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

static double orthoXmin;
static double orthoXmax;
static double orthoYmin;
static double orthoYmax;
static double orthoZmin;
static double orthoZmax;

static double frustumXmin;
static double frustumXmax;
static double frustumYmin;
static double frustumYmax;
static double frustumZmin;
static double frustumZmax;
static double frustumdx;
static double frustumdy;
static double frustumdz;

static double perspectiveAngle;
static double perspectiveRatio;
static double perspectiveZmin;
static double perspectiveZmax;
static double perspectivedx;
static double perspectivedy;
static double perspectivedz;

static double xmin;
static double ymin;
static double xmax;
static double ymax;
static float tpix;

float getXmin(void) {
  return((float) xmin);
}

float getYmin(void) {
  return((float) ymin);
}

float getXmax(void) {
  return((float) xmax);
}

float getYmax(void) {
  return((float) ymax);
}

float getTaillePixel(void) {
  return(tpix);
}

void setParametresOrthoBasique(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax) {
  orthoXmin = xmin;
  orthoXmax = xmax;
  orthoYmin = ymin;
  orthoYmax = ymax;
  orthoZmin = zmin;
  orthoZmax = zmax;
}

void reshapeOrthoBasique(int w,int h) {
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if( w <= h) {
    xmin = orthoXmin;
    xmax = orthoXmax;
    ymin = orthoYmin*(double) h/w;
    ymax = orthoYmax*(double) h/w; }
    else {
    xmin = orthoXmin*(double) w/h;
    xmax = orthoXmax*(double) w/h;
    ymax = orthoYmax;
    ymin = orthoYmin; }
  tpix =(float) (ymax-ymin)/h;
  glOrtho(xmin,xmax,ymin,ymax,orthoZmin,orthoZmax);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void setParametresFrustumBasique(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax,float dx,float dy,float dz) {
  frustumXmin = xmin;
  frustumXmax = xmax;
  frustumYmin = ymin;
  frustumYmax = ymax;
  frustumZmin = zmin;
  frustumZmax = zmax;
  frustumdx = dx;
  frustumdy = dy;
  frustumdz = dz;
}

void reshapeFrustumBasique(int w,int h) {
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if( w <= h) {
    xmin = frustumXmin;
    xmax = frustumXmax;
    ymin = frustumYmin*(double) h/w;
    ymax = frustumYmax*(double) h/w; }
    else {
    xmin = frustumXmin*(double) w/h;
    xmax = frustumXmax*(double) w/h;
    ymax = frustumYmax;
    ymin = frustumYmin; }
  tpix =(float) (ymax-ymin)/h;
  glFrustum(xmin,xmax,ymin,ymax,frustumZmin,frustumZmax);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(frustumdx,frustumdy,frustumdz);
}

static float angle;
static float ratio;
static int fenetre = -1;

void setParametresPerspectiveBasique(float angle,float ratio,float zmin,float zmax,float dx,float dy,float dz) {
  perspectiveAngle = angle;
  perspectiveRatio = ratio;
  perspectiveZmin = zmin;
  perspectiveZmax = zmax;
  perspectivedx = dx;
  perspectivedy = dy;
  perspectivedz = dz;
}

void setParametresPerspectiveBasique(float angle,float ratio,float zmin,float zmax,float dx,float dy,float dz,int fen) {
  setParametresPerspectiveBasique(angle,ratio,zmin,zmax,dx,dy,dz) ;
  fenetre = fen;
}

float anglePerspectiveBasique(void) {
  return(angle);
}

float ratioPerspectiveBasique(void) {
  return(ratio);
}

void reshapePerspectiveBasique(int w,int h) {
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  ratio = (float) w/(float) h; 
  if( ratio < perspectiveRatio) {
    angle = perspectiveAngle/ratio*perspectiveRatio; }
    else {
    angle = perspectiveAngle; }
  gluPerspective(angle,ratio,perspectiveZmin,perspectiveZmax);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(perspectivedx,perspectivedy,perspectivedz);
  if ( fenetre != -1 )
    glutPostWindowRedisplay(fenetre);
}
