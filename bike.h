#ifndef BIKE_H

/**
   Creates a whole bike by calling other components
*/

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"
#include "wheel.h"
#include "handleBar.h"

#define P_DEGREES_FRAME -3

class bike
{
 public:

  //rotation of han
  double direction;
  GLuint model;
  GLuint ped;

  wheel *backWheel;
  handleBar *hb;

  // the current frame
  int frame;

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

      glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
      glMaterialfv (GL_FRONT, GL_SHININESS, shine);

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
      {
	// final value was 1
	glTranslatef(-0.7,-0.2,0);

	glPushMatrix();
	{
	  glRotatef(90,1,0,0);

	  glRotatef(15,0,1,0);
	  //upright
	  gluCylinder(sphere,0.06*size, 0.06*size, 1.7*size, 5,4); 

	  glTranslatef(0,0,1.7*size);

	  glRotatef(122,0,1,0);

	  // bottom cross bar
	  gluCylinder(sphere,0.06*size, 0.06*size, 1.95*size, 5,4); 

	  glTranslatef(0.3*size,0,size*0.8);

	  // top cross bar
	  gluCylinder(sphere,0.06*size, 0.06*size, 1.4*size, 5,4);
	  // and its curve
	  gluSphere(sphere, size*0.06, 5, 5); 

	  glRotatef(160,0,1,0);

	  gluCylinder(sphere,0.06*size, 0.06*size, 0.3*size, 5,4);

	  glTranslatef(0,0,0.3*size);
	  gluSphere(sphere, size*0.06, 5, 5); 
	  glRotatef(-20,0,1,0);
	  gluCylinder(sphere,0.06*size, 0.06*size, 0.27*size, 5,4);
	}
	glPopMatrix();

	//                        side support right
	glPushMatrix();
	{
	  glTranslatef(0.1*size,-0.4*size,0.1*size);
	  glRotatef(85,1,0,0);
	  glRotatef(-35,0,1,0);
	  gluCylinder(sphere,0.04*size, 0.04*size, 1.53*size, 5,4);

	  glTranslatef(0,0,1.53*size);
	  glRotatef(130,0,1,0);
	  glRotatef(7,1,0,0);

	  gluCylinder(sphere,0.04*size, 0.04*size, 1.3*size, 5,4);
	}
	glPopMatrix();

	//                        side support left
	glPushMatrix();
	{
	  glTranslatef(0.1*size,-0.4*size,-0.1*size);
	  glRotatef(95,1,0,0);
	  glRotatef(-35,0,1,0);
	  gluCylinder(sphere,0.04*size, 0.04*size, 1.53*size, 5,4); 

	  glTranslatef(0,0,1.53*size);
	  glRotatef(130,0,1,0);
	  glRotatef(-7,1,0,0);

	  gluCylinder(sphere,0.04*size, 0.04*size, 1.3*size, 5,4);

	}
	glPopMatrix();

	// brown saddle
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffOrange);

	glScalef(1.5,0.3,1);
	glutSolidCube(0.3);

	//gluSphere(sphere, size*0.07, 5, 5); 
	//gluCylinder(sphere,0.06*size, 0.06*size, 0.25*size, 5,4);
      }
      glPopMatrix();

      glEnd();
      glEndList();
    }

  /** 
      this prerenders the pedels
  */
  void drawPedels(double size, int side)
    {
      GLfloat shine [] = {1.0,1.0,1.0,1.0};
      GLfloat spec [] = {1.0,1.0,1.0,1.0};

      // offset between two pedels in degrees(180!);
      double o;
      o = 180;

      GLfloat diffOrange []   = {(double)100/256,(double)100/256,(double)100/256,1.0};
      //GLfloat diffBlack  []   = {0.0,0.0,0.0,1.0};

      glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
      glMaterialfv (GL_FRONT, GL_SHININESS, shine);

      // from the example in the text book!
      GLUquadricObj *sphere;
      sphere = gluNewQuadric();
      gluQuadricDrawStyle(sphere, GLU_FILL);

      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffOrange);


      glPushMatrix();
	{

	  if (side == -1)
	    {
	      o = 0;
	      glRotatef(180,0,1,0);
	      glTranslatef(0.5,0,0);
	    }
	  // -1.75 is down, +0.25 is to one side
	  glTranslatef(-0.25,-1.75,0.25);
	  glRotatef(frame*P_DEGREES_FRAME*side+o,0,0,1);
	  // left pedel first
	  
	  glPushMatrix();
	  {
	    glTranslatef(0,0.5,0);
	    glRotatef(-frame*P_DEGREES_FRAME*side-o,0,0,1);
	    glScalef(0.7*size,0.2*size,1*size);
	    glutSolidCube(0.3);
	  }
	  glPopMatrix();

	  glPushMatrix();
	  {
	    glTranslatef(0,0.25,-0.15);
	    glScalef(0.05*size,0.5*size,0.05*size);
	    glutSolidCube(1);
	  }
	  glPopMatrix();
	}
	glPopMatrix();

      glEnd();
    }

  /**
     This sets the given points to the current positions in the
     bike
   */
  void getLocations (point *lh, point *rh, point *lf, point *rf)
    {
      double x,y,z;
      // feet are pedels
      x = 0.5*sin(-r(frame*P_DEGREES_FRAME+180));
      y = 0.5*cos(-r(frame*P_DEGREES_FRAME+180));
      z = 0.3;

      x = x + 0.25;
      y = y - 1.75;
      //play with y for ik
      y = y + 0.15;

      lf -> set(x,y,z);

      x = 0.5*sin(-r(frame*P_DEGREES_FRAME));
      y = 0.5*cos(-r(frame*P_DEGREES_FRAME));
      x = x + 0.25;
      y = y - 1.75;
      //play with y for ik
      y = y + 0.15;
      z = -0.3;

      rf -> set(x,y,z);

      // now calculate the position for the handlebar

      z = 0.75*sin(r(direction+90));
      x = -0.75*cos(r(direction+90));
      //x = x + 0.25;
      //y = y - 1.75;
      y = 0.6;

      x = x + 1.6;
      //z = z + 0.3;

      lh -> set(x,y,z);

      glPushMatrix();

      glTranslatef(x,y,z);
      //glutSolidCube(0.5);

      glPopMatrix();

      z = 0.75*sin(r(direction+90+180));
      x = -0.75*cos(r(direction+90+180));
      //x = x + 0.25;
      //y = y - 1.75;
      y = 0.6;

      x = x + 1.6;
      //z = z - 0.3;

      rh -> set(x,y,z);
      glPushMatrix();

      glTranslatef(x,y,z);
      //glutSolidCube(0.5);

      glPopMatrix();
      // hands are handlebars, a function of variable "direction"
    }


  /**
     sets the direction of the handlebars
   */
  void setDirection(double d)
    {
      direction = d;
    }

  /**
     sets the frame
   */
  void setFrame(int f)
    {
      frame = f;
    }

  bike()
    { 
      makeList(1.0);
      hb = new handleBar(1.0);
      backWheel = new wheel(1.0);
    }

  bike (double s)
    {
      makeList(1.0);
      hb = new handleBar(1.0);
      backWheel = new wheel(1.0*s);
    }

  void draw(int i)
    {
      point p = new point(0,0,0);


      setFrame(i);
      getLocations (&p,&p,&p,&p);
      glPushMatrix();
      {
	glTranslatef(0.6,0,0);
	//draw the bike
	glCallList(model);

	// and the pedels
	drawPedels(1.0,1);
	drawPedels(1.0,-1);

	//         frame, angle
	hb -> draw(i, direction);
	// position of the back wheel
	glTranslatef(-2.5,-1.8,0);
	//back wheel
	backWheel -> draw(i);
      }
      glPopMatrix();
    }
};
#define BIKE_H 1
#endif
