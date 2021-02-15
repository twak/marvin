/**
   This defines a point, for the purposes of translating the
   points, it uses homogeneous coordinate of 1

   This is also sometimes used as a translation itself, 
   so this should have been called 'vecotor'

*/
#ifndef POINT_H
#define POINT_H 1
#include "include.h"
#include "rotate.h"


class point {

public:
    double x,y,z;
    double **result;

    point(double xx, double yy, double zz)
	{
	    x = xx;
	    y = yy;
	    z = zz;
	}

    point()
      {
	x = 0;
	y = 0;
	z = 0;
	result = NULL;
      }

    point (point *p)
      {
	x = p->x;
	y = p->y;
	z = p->z;
	result = NULL;
      }


    /**
       allocate space for array
     */
    void make()
      {
	int i,j;
	result = (double**)malloc(sizeof(double)*4);
	for (i = 0; i < 4; i++)
	  {
	    result[i] = (double *)malloc(sizeof(double)*4);
	    for (j = 0; j < 4; j++)
	      {
		result[i][j] = 0.0;
	      }
	  }
	for (i = 0; i < 4; i++) result[i][i] = 1.0;
      }

    void deleteMatrix(double **in)
      {
	int j;
	for (j = 0; j < 4; j++)
	  {
	    free(in[j]);
	  }
	free(in);
      }

    //this displays the point on the screen
    void show()
      {
	printf("(%f,%f,%f)\n",x,y,z);
      }

    // multiplies the points by matrix m
    void multiply(double **m)
      {
	int i,j;

	double xx = 0,yy = 0,zz = 0;

	double v, t;

	for (i = 0; i < 4; i++)
	  {
	    t = 0.0;
	    for(j= 0; j < 4; j++)
	      {
		switch (j)
		  {
		  case 0:
		    v = x;
		    break;
		  case 1:
		    v = y;
		    break;
		  case 2:
		    v = z;
		    break;
		  case 3:
		    // homogeneous coord
		    v = 1.0;
		    break;
		  }
		t = t+ v*m[j][i];
	      }
	    switch(i)
	      {
		  case 0:
		    xx = t;
		    break;
		  case 1:
		    yy = t;
		    break;
		  case 2:
		    zz = t;
		    break;
		  case 3:
		    // do nothing homogeneous coord
		    break;
	      }
	  }
	// set up final values
	x = xx;
	y = yy;
	z = zz;
      }


    /**
       Adds values to each coordinate, in the form of
       a vector addition or for specific values
     */
    void add(point *r)
      {
	x += r -> x;
	y += r -> y;
	z += r -> z;
      }

    void add(double xx, double yy, double zz)
      {
	x += xx;
	y += yy;
	z += zz;
      }

    double **getMatrix()
      {
	if (result == NULL) make();
	// enter the values to move
	result[3][0] = x;
	result[3][1] = y;
	result[3][2] = z;
	return result;
      }

    void rot(rotate *r)
      {
	double **m;
	m = r->getMatrix();
	multiply(m);
	r->matrixDeleted();
	free (r);
      }

    void set(double xx, double yy, double zz)
      {
	x = xx;
	y = yy;
	z = zz;
      }

    void set(point p)
      {
	x = p.x;
	y = p.y;
	z = p.z;
      }

    double mag()
      {
	return sqrt(x*x+y*y+z*z);
      }

    double dist(point b)
      {
	double xx,yy,zz;
	xx = b.x -x;
	yy = b.y -y;
	zz = b.z -z;
	return sqrt(xx*xx+yy*yy+zz*zz); 
      }

    void mul(double in)
      {
	x *= in;
	y *= in;
	z *= in;
      }

    /**
       dist from origin
     */
    double dist()
      {
	double xx,yy,zz;
	xx = 0; yy = 0; zz = 0;
	xx =  -x;
	yy =  -y;
	zz =  -z;
	return sqrt(xx*xx+yy*yy+zz*zz); 
      }

    /**
       allocated a new rotate that is the subtraction of one from another
     */
    rotate *subtract(point b)
      {
	rotate *result = new rotate(x,y,z);
	result->x -= b.x;
	result->y -= b.y;
	result->z -= b.z;
	return result;
      }

    /**
       Treats the point as a vector and normalises it
     */
    void normalise()
      {
	double mag;
	mag = sqrt(x*x+y*y+z*z);
	x /= mag;
	y /= mag;
	z /= mag;
      }

    /**
       This sets the value of this point to midway between
       a and b. If fraction = 0 we are at a, if 1, b.
       fraction is paremtric t in most eqns!
     */
    void setBetween(point a, point b, double fraction)
      {
	double i;
	i = 1- fraction;
	x = a.x * i + b.x *fraction;
	y = a.y * i + b.y *fraction;
	z = a.z * i + b.z *fraction;
      }


private:

};
#endif
//
// End of "$Id: CubeView.h,v 1.5 2004/10/17 20:05:06 Administrator Exp $".
//


