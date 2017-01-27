/* loadppm.h
*  v1.3 11.10.2005
*/
#include <Windows.h>
#include <GL\freeglut.h>

#ifndef __LOADPPM_H
#define __LOADPPM_H

#include <iostream>
#include <fstream>
#include <math.h>
//#include <GL/gl.h>

using namespace std;

class PPMImage {
public:
	int sizeX, sizeY;
	GLubyte *data;
	PPMImage(char *filename);
	~PPMImage(){
		if (data)
			delete[] data;
	}
};

#endif
