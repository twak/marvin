#ifndef ARM_H

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"

// in order to mirror some values for the other side, we can
// just multiply by one of these values
#define LEFT 1
#define RIGHT -1

// its a beast of an arm
#define ARM_NUM_PISTONS 19

class arm
{
 public:
  // gives the mapping of points to pixels
  int pistonMap[ARM_NUM_PISTONS][2];
  // other attributes for the piston
  double pistonAttrib[ARM_NUM_PISTONS][6];
  joint *jj,*kj,*lj,*mj,*nj;
  // target destination
  point target;

  // number of pistons so far
  int count;

  arm()
    {
      count = 0;
      // read in? piston locations?
      setPistonMap();
    }

  void setPos(point p)
    {
      ((root *)jj) -> setPos(p);
    }

  /**
     Programs in the coordinates and hooks for an arm
     of the given size on the given size
   */
  arm (point origin, int side, double size)
    {
      count = 0;

      // neck
      point p0(origin);
      // shoulder
      point p1(0,-0.8*size,0.9*size*side);
      //elbow
      point p2(0,-0.8*size,0.3*side);
      //wrist
      point p3(0,-0.8*size,0);
      //end of hand
      point p4(0,-0.3*size,0);


      point z(0,0,0);

      //location we try and minimize to
      target.set(0,-3,0);

      jj = new root(&p0, &z);
      jj->setXMinMax(-0.2,0.2);
      jj->setZMinMax(-0.2,0.2);  

      kj = new joint(jj,&p1);
      kj->setXMinMax(-0.2,0.2);
      // this was -0.7
      kj->setZMinMax(-2.7,2.7);
      kj->addHook(new point(-0.5*size,0,0));
      kj->addHook(new point(-0.25*size,0.4*size,0.3*size*side));
      // this is the head attach - number 4
      kj->addHook(new point(-0.20*size,0.4*size,0));

      lj = new joint(kj,&p2);
      // this is the only joint not symettric in x
      if (side == RIGHT)
	{
	  lj->setXMinMax(0.1,1.4);
	}
      else if (side == LEFT)
	{
	  lj->setXMinMax(-1.4,0.1);
	}
      lj->setZMinMax(-3,3);
      lj->addHook(new point(0.2*size,0,-0.2*size*side));
      lj->addHook(new point(-0.12*size,0,-0.2*size*side));
      lj->addHook(new point(0.034*size,0.03*size,0.12*size*side));

      mj = new joint(lj,&p3);
      mj->setXMinMax(-1.5,1.5);
      mj->setZMinMax(-0.1,0.1);
      mj->addHook(new point(0,size*0.4,0));

      nj = new joint(mj,&p4);
      nj->setXMinMax(-1,1);
      nj->setZMinMax(-1,1);
      nj->addHook(new point(0,size*-0.2,-0.1*size*side));
      nj->addHook(new point(0,size*-0.2,0.1*size*side));

      ((root *)jj) -> finalize();

      // debug function to show location of children
      ((root *)jj) -> showChildren();

      ((root *)jj) -> hookers();
      //((root *)jj) -> showHookers();

      setPistonMap();
    }

 private:

  double myRand(double max)
    {
      double result;
      result = double(max * rand()/(RAND_MAX+1.0));
      return result;
    }

  /**
     Chooses random variations in the pistons rendered
     such as colour and offset
   */
  void s(int o, int t, double size, double housing)
    {
      pistonMap[count][0] = o;
      pistonMap[count][1] = t;

      // rotational offset
      pistonAttrib[count][0] = myRand(30)-15.0;
      // the sizes and housing properties
      pistonAttrib[count][1] = size;
      pistonAttrib[count][2] = housing;
      
      // color - bluey-grey scaled
      pistonAttrib[count][3] = myRand(0.6)+0.8;
      pistonAttrib[count][4] = myRand(0.6)+0.3;
      pistonAttrib[count][5] = myRand(0.6)+0.3;

      count++;
      if (count >= ARM_NUM_PISTONS) 
	{
	  printf("Too many pistons in leg\n");
	  exit(-1);
	}
    }

    public:
  /**
     This the sets of hooks that have pistons between them
   */
  void setPistonMap()
    {
      s(0,1, 0.08, 0.4);
      s(0,2, 0.08, 0.6);
      s(3,0, 0.2, 0.4);
      s(1,2, 0.05, 0.2);
      s(2,3, 0.05, 0.2);
      s(3,1, 0.05, 0.2);
      s(1,6, 0.08, 0.4);
      s(2,7, 0.08, 0.6);
      s(3,8, 0.2, 0.6);
      s(6,10, 0.1, 0.2);
      s(7,10, 0.1, 0.2);
      s(8,10, 0.1, 0.2);
      s(7,8, 0.05, 0.2);
      s(8,6, 0.05, 0.2);
      s(6,8, 0.05, 0.2);
      s(10,11, 0.1, 0.2);
      s(11,12, 0.05, 0.2);
      s(11,13, 0.05, 0.2);
    }

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
     does what it says
   */
  void draw();

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
#define ARM_H 1
#endif
