/**
   root.h

   This contains the location of the root joint, one that starts an IK
   chain

   each limb needs a point it is trying to get to
   we need a method of returning an (array?) of points from root

   we set the end point of a limb, then run a method to find the
   orientation of the joints. Then we run a method that moves all
   the hooks to the correct orientations.

*/
#ifndef ROOT_H
#define ROOT_H 1

/**
#include <FL/Fl.H>
#  include <FL/Fl_Gl_Window.H>
#  include <FL/gl.h>

#include <stdlib.h>
*/
#include "include.h"
#include "point.h"
#include "joint.h"

// the maximum number of hooks on a limb

// max number of hooks (incl. joints) that can be returned form
// a root

#define MAX_POINTS 30

class root: public joint{

public:
   // aray storing all the points to be output
    point result[MAX_POINTS];
    // number of things in result
    int numResult;
    // stores number of points once calculated.
    int numPoints;
    //the vector between the final joint and the point
    // whose distance to the target is bein minimized
    point finalLength;


  root (point *p, point ep):joint(NULL, p)
    {
      target = NULL;
      //target.shdow();
      numPoints = 0;
      finalLength = ep;
      numResult = 0;
    }

  void setPos(point p)
    {
      parentPosition -> set(p);
    }

  /**
     This runs through all the children of this
     root accumulating the translated hooks
   */
  void hookers()
    {
      int i,c;
      joint *j = this;
      c = 0;
      while (j != NULL)
	{
	  for (i = 0; i < j-> numHooks; i++)
	    {
	      result[c] = j -> dhooks[i];
	      c++;
	    }
	  j = j -> child;
	}
      numResult = c;
    }

  void showHookers()
    {
      int i;
      for (i = 0; i < numResult; i++)
	{
	  printf("%d)) ",i);
	  result[i].show();
	}
    }

  void addChild(joint *j);

  /**
     Initializes the chain
   */
  //void finalize();
  void finalize()
  {
    numPoints = countChildren();
  }

  int countChildren();

  /**
     Shows the parent position of each child
   */
  void showChildren()
    {
      int i = 0;
      joint *j = this;
      while (j != NULL)
	{
	  //printf("%d) child pos is ",i);
	  //j->parentPosition->show();
	  i++;
	  j = j->child;
	}
    }

  /**
     calculates the position at the end of the chain,
     and minizes the error in this chain
  */
  point endPoint(int i);
  void  doIK(point target);

private:
  point *target;
  int numChildren;
};

#endif
