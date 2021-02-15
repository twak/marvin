/**
   library header files
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define M_PI 3.14159265

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glut.h>
//#include "/opt/local/glut-3.7/include/GL/glut.h"
#endif
