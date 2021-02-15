#ifndef HB_H

/**
   The draws the handlebar at the correct angle for "steering"
 */

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"
#include "wheel.h"

class handleBar
{
  //rotation of han
  double direction;
  GLuint model;

  // number of pistons so far
  int count;
  wheel *frontWheel;

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
      GLfloat shine [] = {0.0,0.2,0.0,1.0};
      GLfloat spec [] = {0.0,0.2,0.0,1.0};

      GLfloat diffOrange []   = {(double)10/256,(double)150/256,(double)10/256,1.0};

      GLfloat diffBlack  []   = {0.7,0.7,0.7,1.0};

      glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
      glMaterialfv (GL_FRONT, GL_SHININESS, shine);

      // from the example in the text book!
      GLUquadricObj *sphere;
      sphere = gluNewQuadric();
      gluQuadricDrawStyle(sphere, GLU_FILL);

      model = glGenLists(1);
      glNewList(model, GL_COMPILE);

      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffOrange);

      // forks
      glPushMatrix();
      glRotatef(90,1,0,0);
      gluCylinder(sphere,0.06*size, 0.06*size, 0.5*size, 5,4); 

      glPushMatrix();
      glRotatef(90,1,0,0);
      glTranslatef(0,0.5*size,-0.125*size);
      gluSphere(sphere, size*0.07, 5, 5); 
      gluCylinder(sphere,0.06*size, 0.06*size, 0.25*size, 5,4);
      glTranslatef(0,0.0,0.25*size);
      gluSphere(sphere, size*0.07, 5, 5); 
      glPopMatrix();

      glPushMatrix();
      glTranslatef(0,0.15*size,0.5*size);
      gluCylinder(sphere,0.06*size, 0.04*size, 0.7*size, 5,4);
      glTranslatef(0,0,0.68*size);
      glRotatef(20,0,1,0);
      gluCylinder(sphere,0.04*size, 0.03*size, 0.4*size, 5,4);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(0,-0.15*size,0.5*size);
      gluCylinder(sphere,0.06*size, 0.04*size, 0.7*size, 5,4);
      glTranslatef(0,0,0.68*size);
      glRotatef(20,0,1,0);
      gluCylinder(sphere,0.04*size, 0.03*size, 0.4*size, 5,4);
      glPopMatrix();
      // start of handlebars
      glRotatef(-180,1,0,0);
      // this is the anoying patch on the steering column

      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffBlack);

      glRotatef(-15,0,0,1);
      gluDisk(sphere, 0, 0.06*size, 5, 1);
      glRotatef(15,0,0,1);
      glRotatef( 180,1,0,0);
      glTranslatef(0,0,-0.2*size);
      gluCylinder(sphere,0.04*size, 0.04*size, 0.2*size, 5,4);
      glRotatef(-180,1,0,0);
      gluDisk(sphere, 0, 0.04*size, 5, 1);
      glRotatef(180,1,0,0);
      glRotatef(-40,0,1,0);
      glTranslatef(0,0,-0.15*size);
      gluCylinder(sphere,0.04*size, 0.04*size, 0.15*size, 5,4); 
      //crossbar
      glRotatef(90,1,0,0);
      glTranslatef(0,0,-0.35*size);
      gluCylinder(sphere,0.05*size, 0.05*size, 0.7*size, 5,4);

      // right bar
      glPushMatrix();
      gluSphere(sphere, size*0.05, 5, 5); 
      glRotatef(130,1,0,0);
      gluCylinder(sphere,0.05*size, 0.05*size, 0.3*size, 5,4);
      glTranslatef(0,0,0.3*size);
      gluSphere(sphere, size*0.05, 5, 5);
      glRotatef(140,1,0,0);
      glRotatef(-50,0,1,0);
      gluCylinder(sphere,0.05*size, 0.05*size, 0.5*size, 5,4);
      glTranslatef(0,0,0.5*size);
      gluSphere(sphere, size*0.05, 5, 5);
      glPopMatrix();

      // left bar
      glTranslatef(0,0,0.7*size);
      glPushMatrix();
      gluSphere(sphere, size*0.05, 5, 5); 
      glRotatef(50,1,0,0);
      gluCylinder(sphere,0.05*size, 0.05*size, 0.3*size, 5,4);
      glTranslatef(0,0,0.3*size);
      gluSphere(sphere, size*0.05, 5, 5);
      glRotatef(-140,1,0,0);
      glRotatef(-50,0,1,0);
      gluCylinder(sphere,0.05*size, 0.05*size, 0.5*size, 5,4);
      glTranslatef(0,0,0.5*size);
      gluSphere(sphere, size*0.05, 5, 5);
      glPopMatrix();

      glPopMatrix();
      // top of forks

      glEnd();
      glEndList();
    }

  handleBar()
    { 
      makeList(1.20);
      frontWheel = new wheel(1.0);
    }

  handleBar (double s)
    {
      makeList(1.20*s);
      frontWheel = new wheel(1.0*s);
    }

  void draw(int i, double angle)
    {
      glTranslatef(1.0,0,0.0);

      glPushMatrix();

      // this rotates left to right
      glRotatef(angle,0,1,0);

      // this adds the slight angle to the bars as a whole
      glRotatef(15,0,0,1);

      glCallList(model);
      //                      left right
      //               up down
      //glTranslatef(-1.0,0,1.0);
      glTranslatef(0.15,-1.8,0.0);
      frontWheel -> draw(i);

      glPopMatrix();
    }
};
#define HB_H 1
#endif
