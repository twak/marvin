#ifndef WHEEL_H

/**
   This makes a wheel that rotates in a forward direction
   x is "forward"
 */

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"

#define DEGREES_FRAME -3

class wheel
{
  joint *jj,*kj,*lj, *mj;
  // target destination
  point target;

  GLuint model;

  // number of pistons so far
  int count;

 public:

  /**
     converts degrees to radians
   */
  double r(double in)
    {
      return in*M_PI/180;
    }

  /**
     Draws the head pointing along the x axis
   */
  void makeList(double size)
    {

      int i;
      double sx,sy,ex,ey;

      GLfloat shine [] = {30.0,30.0,30.0,1.0};
      GLfloat spec [] = {10.0,10.0,10.0,1.0};

      GLfloat diffOrange []   = {(double)100/256,(double)100/256,(double)100/256,1.0};
      GLfloat diffBlack  []   = {0.0,0.0,0.0,1.0};

      glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
      glMaterialfv (GL_FRONT, GL_SHININESS, shine);

      // from the example in the text book!
      GLUquadricObj *sphere;
      sphere = gluNewQuadric();
      gluQuadricDrawStyle(sphere, GLU_FILL);

      model = glGenLists(1);
      glNewList(model, GL_COMPILE);

      // rim of wheel
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffBlack);
      glutSolidTorus(0.07*size, 1*size, 6, 30);
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffOrange);
      glutSolidTorus(0.06*size, 0.95*size, 4, 30);

      // hub
      glPushMatrix();
      glTranslatef(0,0,-0.05);
      gluCylinder(sphere,0.06*size, 0.06*size, 0.1, 10,1);
      glTranslatef(0,0,0.1);
      gluDisk(sphere, 0, 0.06, 10, 1); 
      glPopMatrix();

      glPushMatrix();
      glRotatef(180, 1, 0, 0);
      gluDisk(sphere, 0, 0.06, 10, 1); 
      glPopMatrix();

      glLineWidth(1);

      glBegin(GL_LINES);
      // spokes, near side first
      for( i = 10; i < 360; i = i + 36)
	{
	  // central points are r = 0.06, i - 90; i + 90;
	  // end points are r = 1, i - 17/2; i + 17/2
	  sx = 0.06*sin(r(i-90));
	  sy = 0.06*cos(r(i-90));
	  ex = sin(r(i-8.5));
	  ey = cos(r(i-8.5));
	  glVertex3f(sx, sy, 0.05);
	  glVertex3f(ex, ey, 0.00);
	  sx = 0.06*sin(r(i+90));
	  sy = 0.06*cos(r(i+90));
	  ex = sin(r(i+8.5));
	  ey = cos(r(i+8.5));
	  glVertex3f(sx, sy, 0.05);
	  glVertex3f(ex, ey, 0.00);
	}

      // Then far side
      for( i = 27; i < 360; i = i + 36)
	{
	  // central points are r = 0.06, i - 90; i + 90;
	  // end points are r = 1, i - 17/2; i + 17/2
	  sx = 0.06*sin(r(i-90));
	  sy = 0.06*cos(r(i-90));
	  ex = sin(r(i-8.5));
	  ey = cos(r(i-8.5));
	  glVertex3f(sx, sy, -0.05);
	  glVertex3f(ex, ey, 0.00);
	  sx = 0.06*sin(r(i+90));
	  sy = 0.06*cos(r(i+90));
	  ex = sin(r(i+8.5));
	  ey = cos(r(i+8.5));
	  glVertex3f(sx, sy, -0.05);
	  glVertex3f(ex, ey, 0.00);
	}

      glEnd();
      glEndList();
    }

  wheel()
    { 
      makeList(1.0);
    }

  wheel (double s)
    {
      makeList(s);
    }

  void draw(int frame)
    {
      glPushMatrix();
      glRotatef(frame*DEGREES_FRAME, 0,0,1);
      glCallList(model);
      glPopMatrix();
    }
};
#define WHEEL_H 1
#endif
