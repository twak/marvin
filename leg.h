#ifndef LEG_H

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"

#define LEFT 1
#define RIGHT -1

#define NUM_PISTONS 7

class leg
{
  // gives the mapping of points to pixels
  int pistonMap[NUM_PISTONS][2];
  // other attributes for the piston
  double pistonAttrib[NUM_PISTONS][6];
  joint *jj,*kj,*lj,*mj;
  // target destination
  point target;

  // number of pistons so far
  int count;

 public:

  leg()
    {
      count = 0;
      // read in? piston locations?
      setPistonMap();
    }

  leg (point origin, int side, double size)
    {
      count = 0;

      point p0(origin);
      point p1(0,-1*size,0);
      point p2(0,-1*size,0);
      point p3(0.2*size,0,0);

      point z(0,0,0);

      //location we try and minimize to
      target.set(0,-3,0);

      jj = new root(&p0, &z);
      jj->setXMinMax(-0.1,0.1);
      jj->setZMinMax(-01.5,01.5);  

      jj->direction.set(-0.5,0,-0.5);

      kj = new joint(jj,&p1);
      kj->setXMinMax(-1.5,1.5);
      kj->setZMinMax(-01.5,03);
      kj->addHook(new point(-0.1*size,0.5*size,0));


      kj->direction.set(-0.5,0,-0.5);

      lj = new joint(kj,&p2);
      lj->setXMinMax(-0.1,0.1);
      lj->setZMinMax(-3,-0.2);
      lj->addHook(new point(-0.05*size,0.5*size,0));
      lj->addHook(new point( 0.05*size,0.5*size,0));

      mj = new joint(lj,&p3);
      mj->setXMinMax(-0.5,0.5);
      mj->setZMinMax(-0.5,0.5);
      // unecessary, but complicated to remove
      mj->addHook(new point(0,0.0,0));
      mj->addHook(new point(+0.2*size,0.0,0));

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
      
      // colour - bluey-grey scaled
      pistonAttrib[count][3] = myRand(0.8)+0.8;
      pistonAttrib[count][4] = myRand(0.8)+0.3;
      pistonAttrib[count][5] = myRand(0.8)+0.3;

      count++;
      if (count >= NUM_PISTONS) 
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
      s(0,1, 0.4, 0.6);
      s(1,3, 0.2, 0.3);
      s(3,8, 0.1, 0.2);
      s(2,4, 0.1, 0.2);
      s(5,7, 0.1, 0.1);
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
     
   */
  void draw();

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
#define LEG_H 1
#endif
