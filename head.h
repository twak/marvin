#ifndef HEAD_H

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"


#define LEFT 1
#define RIGHT -1

#define NUM_PISTONS 7

class robotHead
{
 public:
  // gives the mapping of points to pixels
  int pistonMap[NUM_PISTONS][2];
  // other attributes for the piston
  double pistonAttrib[NUM_PISTONS][6];
  joint *jj,*kj,*lj, *mj;
  // target destination
  point target;

  GLuint model;

  // number of pistons so far
  int count;

  /**
     Draws the head pointing along the x axis
   */
  void makeList(double size)
    {
      GLfloat shine [] = {0.6,0.6,0.6,0.2};
      GLfloat spec [] = {0.6,0.6,0.6,0.2};

      GLfloat diffOrange []   = {(double)197/256,(double)178/256,(double)156/256,1.0};
      GLfloat diffBlack  []   = {0.0,0.0,0.0,1.0};
      GLfloat diffBlue   []   = {(double)169/256,(double)201/256,(double)232/256,1.0};

      glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
      glMaterialfv (GL_FRONT, GL_SHININESS, shine);

      GLUquadricObj *sphere;
      sphere = gluNewQuadric();
      gluQuadricDrawStyle(sphere, GLU_FILL);

      model = glGenLists(1);
      glNewList(model, GL_COMPILE);

      //size = 0.4
      glPushMatrix();
      glTranslatef(0.3*size,0,0);
      glRotatef(90,0,1,0);
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffOrange);
      // eye shield
      gluCylinder(sphere,0.2*size, 0.22*size, 0.2, 15,1);
      glTranslatef(0,0,0.1*size);
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffBlack);
      // slightly less than .22!, eye back ground
      gluDisk(sphere, 0, 0.21, 15,1);
      // eye
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffBlue);
      gluSphere(sphere, size*0.15, 7, 7);
      glPopMatrix();

      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffBlack);

      // ears
      glPushMatrix();
      glTranslatef(0.0,-0.401*size,0);
      glRotatef(90,1,0,0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glTranslatef(0,0.1,0.0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glTranslatef(0.0,-0.2,0.0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glTranslatef(-0.1,0.15,0.0);
      gluDisk(sphere, 0, 0.04, 6, 1);
      glTranslatef(0.0,-0.1,0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glPopMatrix();

      // other ear
      glPushMatrix();
      glTranslatef(0.0,0.401*size,0);
      glRotatef(90,1,0,0);
      glRotatef(180,0,1,0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glTranslatef(0,0.1,0.0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glTranslatef(0.0,-0.2,0.0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glTranslatef(0.1,0.15,0.0);
      gluDisk(sphere, 0, 0.04, 6, 1);
      glTranslatef(0.0,-0.1,0);
      gluDisk(sphere, 0, 0.04, 6,1);
      glPopMatrix();

      // main head
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffOrange);
      gluSphere(sphere, size*0.4, 15, 15);

/*       glEnd(); */
      glEndList();
    }

  /**
     sets the root position of the head
   */
  void setPos(point p)
    {
      ((root *)jj) -> setPos(p);
    }

  robotHead()
    {
      count = 0;
    }

  robotHead (point origin, int side, double size)
    {
      count = 0;

      point p0(origin);
      point p1(0,0.5*size,0);
      point p2(0.5*size,0.01*size,0);

      point z(0,0,0);

      //location we try and minimize to
      target.set(0,-3,0);

      jj = new root(&p0, &z);
      jj->setXMinMax(-0.1,0.1);
      jj->setZMinMax(-0.1,0.1);

      kj = new joint(jj,&p1);
      kj->setXMinMax(-0.1,0.1);
      kj->setZMinMax(-0.1,0.1);

      lj = new joint(kj,&p2);
      lj->setXMinMax(-3,3);
      lj->setZMinMax(-3,3);
      lj->addHook(new point(-0.2,-0.2,0.0));
      lj->addHook(new point(-0.7,-0.3,0.0));
      lj->addHook(new point(-0.7,-0.3,0.0));

      ((root *)jj) -> finalize();

      // debug function to show location of children
      ((root *)jj) -> showChildren();

      ((root *)jj) -> hookers();
      //((root *)jj) -> showHookers();

      // render, but dont display the head
      makeList(size);

    }

  

 private:

  double myRand(double max)
    {
      double result;
      result = double(max * rand()/(RAND_MAX+1.0));
      return result;
    }

 public:
  /**
     Sets the location of this limb
   */
  void setLimb(int i, double in)
    {
      joint *j;
      switch(i)
	{
	case 1:
	  j = jj;
	  break;
	case 2:
	  j = kj;
	  break;
	case 3:
	  j = lj;
	  break;
	case 4:
	  j = mj;
	  break;
	}
      j -> direction.set(0,0,in);
      ((root *)jj) -> endPoint(1);
      //target.dist(endPoint(0));
      ((root *)jj) -> hookers();
    }

  /**
     
   */
  void draw()
    {
      point *res;
      res = ((root *)jj)->result;
      drawHead(res[1], res[2]);
    }

  void drawHead(point a, point b)
    {
      rotate *dir;
      dir = b.subtract(a);

      dir->normalise();
      //makeList(2);

      // convert from vector to angle rotation
      dir -> toRotate();

      glPushMatrix();

      glTranslatef(a.x,a.y,a.z);

      //printf(" %f  %f \n",dir->x,dir->y);
      glRotatef(dir -> x, 1,0,0);
      glRotatef(-dir -> y-90, 0,1,0);

      glCallList(model);

      glPopMatrix();
      delete (dir);
    }
  /**
     Gets the specified hook number
   */
  point getHook(int i)
    {
      root *r;

      r = (root *)jj;
      if (i > r->numResult)
	{
	  printf("Invalid hood %d\n",i);
	  exit(-1);
	}

      return (r -> result)[i];
    }


  /**
     moves the target 
   */
  void setTarget(double x, double y, double z)
    {
      target.set(x,y,z);
      ((root *)jj)->doIK(target);
      ((root *)jj) -> endPoint(1);
      ((root *)jj) -> hookers();
    }

  void setTarget(point in)
    {
      target.set(in.x,in.y,in.z);
      ((root *)jj)->doIK(target);
      ((root *)jj) -> endPoint(1);
      ((root *)jj) -> hookers();
    }
};
#define HEAD_H 1
#endif
