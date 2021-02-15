#ifndef STARS_H

/**
   The stars in the night sky
*/

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"


#define NUM_STARS 500
//distance stars are drawn at (big)
#define FAR       1000
class stars
{
  //rotation of han
  double xPos[NUM_STARS];
  double yPos[NUM_STARS];
  double size[NUM_STARS];
  // the pre rendered stars
  GLuint model;
  double starSize;
 public:

  /**
     converts degrees to radians
  */
  double r(double in)
    {
      return in*M_PI/180;
    }

  double myRand(double max)
    {
      double result;
      result = double(max * rand()/(RAND_MAX+1.0));
      return result;
    }

  /**
     Calcualtes the position for NUM_STARS stars
     uses polar coordinates, prerenders too!
   */
  void makeList(double size)
    {
      int i;
      double xPos, yPos, zPos, s;
      // height and rotation
      double h,u;

      glPushMatrix();
      model = glGenLists(1);
      glNewList(model, GL_COMPILE);
      glBegin(GL_POINTS);
      for (i = 0; i < NUM_STARS; i++)
	{
	  h = myRand(360);
	  u = myRand(360);
	  xPos = FAR * sin(r(h)) * cos(r(u));
	  yPos = FAR * sin(r(h)) * sin(r(u));
	  zPos = FAR * cos(r(h));
	  s =   myRand(size)+1;
	  glPointSize(s);
	  glVertex3f(xPos, yPos, zPos);
	}
      glEnd();
      glEndList();
    }

  /**
     Make stars of up to the size wanted
   */
  stars()
    { 
      starSize = 1.0;
      makeList(1.0);
    }

  stars (double s)
    {
      starSize = s;
      makeList(s);
    }

  void draw()
    {
      // stars always glow white!
      GLfloat diffWhite   []   = {10,10,1,1.0};
      glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, diffWhite);

      glPointSize(starSize);
      glCallList(model);

      // set emissions back to black
      GLfloat diffBlack   []   = {0,0,0,0};
      glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, diffBlack);

    }
};
#define STAR_H 1
#endif
