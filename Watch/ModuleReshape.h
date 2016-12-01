
/* de visualisation                       */

#ifndef MODULERESHAPE
#define MODULERESHAPE

void setParametresOrthoBasique(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax) ;
void reshapeOrthoBasique(int w,int h) ;
void setParametresFrustumBasique(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax,float dx,float dy,float dz) ;
void reshapeFrustumBasique(int w,int h) ;
void setParametresPerspectiveBasique(float angle,float ratio,float zmin,float zmax,float dx,float dy,float dz) ;
void setParametresPerspectiveBasique(float angle,float ratio,float zmin,float zmax,float dx,float dy,float dz,int fen) ;
void reshapePerspectiveBasique(int w,int h) ;
float getXmin(void) ;
float getYmin(void) ;
float getXmax(void) ;
float getYmax(void) ;
float getTaillePixel(void) ;
float anglePerspectiveBasique(void) ;
float ratioPerspectiveBasique(void) ;

#endif
