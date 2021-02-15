
#ifndef PISTON_H
#define PISTON_H 1

#include "include.h"
#include "rotate.h"
#include "point.h"
//#include "/opt/local/glut-3.7/include/GL/glut.h"


class piston 
{

public:

  piston()
    {
    }

  double myRand(double max)
    {
      double result;
      result = double(max * rand()/(RAND_MAX+1.0));
      return result;
    }

  /**
     This uses GL to draw a piston between two points on the 
     screen
     piston from a to b. width is size.
     rotation is jiggle (so they are all not the same
     housing is length of bit of piston that is not a shaft
  */

  void drawPiston(point a, point b, double size, double housing,double jiggle)
    {
      //{p1, p2, size, offset}
      double distance;
      double rodDiam;
      distance = a.dist(b);
      rodDiam = size*2/4;
      rotate *dir;
/*       GLfloat shine [] = {30.0,30.0,30.0,1.0}; */
/*       GLfloat spec [] = {10.0,10.0,10.0,1.0}; */
/*       GLfloat diffBlue []   = {0.0,0.8,1.0,1.0}; */

/*       diffBlue[0] = myRand(0.6)+0.3; */
/*       diffBlue[1] = myRand(0.6)+0.3; */
/*       diffBlue[2] = myRand(0.6)+0.8; */

/*       glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffBlue); */
/*       glMaterialfv (GL_FRONT, GL_SPECULAR, spec); */
/*       glMaterialfv (GL_FRONT, GL_SHININESS, shine); */

      // calculate offset
      dir = b.subtract(a);

 /*      b.show(); */
/*       a.show(); */
/*       printf("before:"); */
/*       dir->show(); */

      dir->normalise();
 /*      printf("narmaliseded:"); */
/*       dir->show(); */

      // convert from vector to angle rotation
      dir -> toRotate();

/*       printf("final:"); */
/*       dir->show(); */

      if (housing > distance) housing = distance;

      glPushMatrix();

      glTranslatef(a.x,a.y,a.z);
      //printf(" %f  %f \n",dir->x,dir->y)
      glRotatef(dir -> x, 1,0,0);
      glRotatef(-dir -> y-90, 0,1,0);
      // z is always 0!
      glRotatef(-dir -> z, 0,0,1);

      //draw the housing
      glPushMatrix();
      glScalef(housing,size,size);
      glTranslatef(0.5,0,0);
      glRotatef(jiggle,1,0,0);
      glutSolidCube(1);
      glPopMatrix();

      // draw the shaft
      glPushMatrix();
      glScalef(distance,rodDiam,rodDiam);
      glTranslatef(0.5,0,0);
      glRotatef(jiggle,1,0,0);
      glutSolidCube(1);
      glPopMatrix();

      glPopMatrix();
      delete (dir);
    }
};
#endif
