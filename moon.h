#ifndef MOON_H

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"


#define texWidth 512
#define texHeight 256

class moon
{
  double size;
  point pos;

 public:

  GLuint model;

  GLuint texid;

/*   int loadTexture(char *fileName, GLubyte pic[texHeight][texWidth][3]); */
/*   int initTex(char *fileName); */
/*   void init(); */
  int LoadBitmap(char* szFileName, GLuint texid, int width, int height);
  /**
     Draws the moon
   */
  void makeList(double size)
    {
      GLfloat shine [] = {30.0,30.0,30.0,1.0};
      GLfloat spec [] = {10.0,10.0,10.0,1.0};

      //GLfloat diffOrange []   = {(double)197/256,(double)178/256,(double)156/256,1.0};
      //GLfloat diffBlack  []   = {0.0,0.0,0.0,1.0};
      //GLfloat diffBlue   []   = {(double)169/256,(double)201/256,(double)232/256,1.0};
      //glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
      //glMaterialfv (GL_FRONT, GL_SHININESS, shine);


      GLfloat diffOrange []   = {(double)197/256,(double)178/256,(double)156/256,1.0};
      //GLfloat diffBlack  []   = {0.0,0.0,0.0,1.0};
      //GLfloat diffBlue   []   = {(double)169/256,(double)201/256,(double)232/256,1.0};

      glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
      glMaterialfv (GL_FRONT, GL_SHININESS, shine);
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffOrange);

      glMaterialfv (GL_FRONT, GL_SHININESS, shine);

      texid = 1;

      LoadBitmap("moon_small.bmp", texid, texWidth, texHeight);

      GLUquadricObj *sphere;
      sphere = gluNewQuadric();
      gluQuadricDrawStyle(sphere, GLU_FILL);

      model = glGenLists(1);
      glNewList(model, GL_COMPILE);

      glEnable(GL_TEXTURE_2D);

      gluQuadricTexture(sphere, GL_TRUE);
      gluSphere(sphere, size, 30,30);

      glDisable(GL_TEXTURE_2D);

      glEndList();
    }

  moon()
    {
      size = 1.0;
      makeList(1.0);
      pos.set(2,2,0);
    }

  moon (double s)
    {
      size = s;
      // render, but dont display the head
      makeList(size);
      pos.set(2,2,0);
    }


 private:

  double myRand(double max)
    {
      double result;
      result = double(max * rand()/(RAND_MAX+1.0));
      return result;
    }

 public:
 

/*   /\** */
/*      This gives the rotation */
/*   *\/ */
/*   double theta(double xpos, double ypos, double zpos) */
/*     { */

/*     } */
  
/*   /\** */
/*      This gives the height rotation */
/*    *\/ */
/*   double phi(double xpos, double ypos, double zpos) */
/*     { */

/*     } */

/**
   convert degrees to radians
 */
  double r(double in)
    {
      return in*M_PI/180;
    }

  /**
     Phi is height, theta rotation along the horizon. 
   */
  void convert (double *t, double *p)
    {
      // from mathworld.wolfram.com/SphericalCoordinates.html,
      // swapping z and y, AND x and z axis to preserve right handed
      // system
      // if  x is negative, add pi

/*       rotate dir; */
/*       dir.x = pos.x; */
/*       dir.y = pos.y; */
/*       dir.z = pos.z; */
/*       //dir.normalise(size); */
/*       dir.toRotate(); */

/*       //dir.show(); */

/*       *t = r(-dir.y-M_PI); */
/*       *p = r(dir.x); */

/*       double x,y,z,r; */

/*       x = pos.z; */
/*       y = pos.x; */
/*       z = pos.y; */

/*       r = size; */

/*       if (x != 0) */
/* 	{ */
/* 	  *t = atan(y/x); */
/* 	} */
/*       else */
/* 	{ */
/* 	  *t = 0; */
/* 	} */
	 
/*       if (x < 0) */
/* 	{ */
/* 	  *p = acos(z/r)+M_PI; */
/* 	} */
/*       else */
/* 	{ */
/* 	  *p = acos(z/r); */
/* 	} */
      double a,b,c,d;

	a = pos.x;
	b = pos.y;
	c = pos.z;
	d = sqrt(b*b+c*c);

	if (b > 0)
	  {
	    *p = -acos(c/d);
	  }
	else
	  {
	    *p = acos(c/d);
	  }
	*t = -asin(-a)-M_PI/2;
    }

  void convertBack (double p, double t)
    {
      pos.z = size * sin (p) * cos (t);
      pos.x = size * sin (p) * sin (t);
      pos.y = size * cos (p);
    }

  //0.785398 pi/4
  void doNastyMath()
    {
      double a,b;
      printf("before\n");
      pos.show();
      convert(&a, &b);
      printf("middle: %f  %f\n",a,b);
      convertBack(a,b);
      printf("end\n");
      pos.show();
    }

  void setPoint(double angle)
    {
      // diameter is size
      //double theta, phi;
      angle++;
      // original theta and phi
      //double ot, op;
      // find the location of the man in polar coords
      //printf("starting\n");
      //pos.show(); 
      //convert(&theta, &phi+angle);
      //theta = theta - 0.03;
      //printf("theta is %f, phi is %f\n",theta, phi);
      //phi = phi + 0.03;
      //printf("theta is %f, phi is %f\n",theta, phi);
      //if (phi > M_PI) phi = -M_PI;
      //printf("theta is %f, phi is %f\n",theta, phi);
      // set point from new theta and phi
      //convertBack(theta,phi);
      //pos.show();

/*       pos.set(0,1,0); */
/*       pos.normalise(); */
/*       pos.mul(size); */
/*       doNastyMath(); */

/*       pos.set(1,1,1); */
/*       pos.normalise(); */
/*       pos.mul(size); */
/*       doNastyMath(); */
	

    }

  /**
     And here is the distance from the origin
   */
  double dist(double xpos, double ypos, double zpos)
    {
      return 3;
    }

  /**
     
   */
  void draw()
    {
      setPoint(1.0);
      glPushMatrix();
      //glTranslatef(pos.x*2,pos.y*2,pos.z*2);
      //glTranslatef(0,-15,0);
      glCallList(model);

      //glTranslatef(size+0.1,0,0);
      //glPointSize(10);
      glBegin(GL_POINTS);
      glVertex3f(0, 0, 0);
      glEnd();

      glPopMatrix();
    }
};
#define MOON_H 1
#endif
