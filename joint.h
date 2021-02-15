/**
http://freespace.virgin.net/hugo.elias/models/m_ik2.htm
http://www.4p8.com/eric.brasseur/cppcen.html#l1



*/
#ifndef JOINT_H
#define JOINT_H 1

/**
#include <FL/Fl.H>
#  include <FL/Fl_Gl_Window.H>
#  include <FL/gl.h>

#include <stdlib.h>
*/
#include "include.h"
#include "point.h"
#include "rotate.h"

// the maximum number of hooks on a limb
#define MAX_HOOKS 8

class joint {

 public:
    // the natural angles for this joint ?? unused at the moment
    double nAX, nAY, nAZ;
    // the current angles for this joint, updated automatically
    rotate direction;

    // these are accumulated from the root along each joint
    // to determine to position of the end point
    rotate totalRotate;
    point  totalTranslate;
    // the max and min angles for this joint's root
    double maxXRot, maxZRot, minZRot , minXRot ;
    // a pointer to the parent joint of us, NULL if root
    joint *parent, *child;
    // the point on the parent that this joint is located
    point *parentPosition; 
    //rotate *parentRotate;
    // the weight of the limb
    double tension;
    // the hooks, location given 
    int numHooks;
    // dhooks contains output values on joint, hooks input
    point hooks[MAX_HOOKS];
    point dhooks[MAX_HOOKS];

    // constructor, point is the location of the child node, joint is parent
    joint(joint * p, point *rel)
      {
	parent = p;
	if (parent == this)
	  {
	    printf("cant pass pointer to self in joint.h\n");
	    exit(-1);
	  }
	if (parent != NULL)
	  {
	    parent -> addChild(this);
	  }
	child  = NULL;

	direction = rotate(0,0,0);
	totalRotate = rotate(0,0,0);
	// we use point here as a vector
	totalTranslate = point(0,0,0);
	// set default angles as straight ahead
	parentPosition = new point(rel);
	//we always add a point at the local origin
	hooks[0] = new point(0,0,0);
	numHooks = 1;
	// default tension
	tension = 1.0;
      }



    /**
       Set ik limits
     */
    void setZMinMax(double min, double max)
	{
	    direction.setLimits(0,0,0,0,min,max);
	}
    void setXMinMax(double min, double max)
	{
	    direction.setLimits(min,max,0,0,0,0);
	}

    void setTension(double d)
	{
	    tension = d;
	}


    /**
       adds a hook to the system
     */
    void addHook(point p)
      {
	if (numHooks+1 >=MAX_HOOKS)
	  {
	    printf("trying to add too many hooks\n");
	    exit(-1);
	  }
	hooks[numHooks] = new point(p);
	numHooks++;
      }

    /**
       Sets up dhooks to be the hooks translated
       by the given matrix
     */
    void setHooks(double **matrix)
      {
	int i;
	for (i = 0; i < numHooks; i++)
	  {
	    //printf("going in");
	    //hooks[i].show();
	    dhooks[i] = hooks[i];
	    dhooks[i].multiply(matrix);
	    //	    printf("comming out ");
	    //dhooks[i].show();
	  }
      }

    /**
       Gives output points when requested, it converts them to 
       the reference system of the parent
     */
    int getDefinedHooks()
	{
	    return numHooks;
	}

    /**
       returns the hooks for the joint (needs rotation+translation!)
     */
    point getHook(int h)
	{
	    if (h < 0 || h > numHooks)
	      {
		printf("Bad numHooks in joint.h\n");
		exit(-1);
	      }
	    return hooks[h];
	}

    /**
       adds the child to this joint
     */
    void addChild(joint *c)
      {
	child = c;
      }

private:
};

//
// End of "$Id: CubeView.h,v 1.5 2004/10/17 20:05:06 Administrator Exp $".
//

#endif
