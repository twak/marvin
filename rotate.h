
#ifndef ROTATE_H
#define ROTATE_H 1

#include "include.h"
#include "stdio.h"
#include "stdlib.h"

class rotate {

public:
    double x,y,z;
    double **result;

    double xMin,yMin,zMin,xMax,yMax,zMax;

    rotate(double xx, double yy, double zz)
	{
	    x = xx;
	    y = yy;
	    z = zz;
	    result = NULL;
	    xMin = -7777;
	    yMin = -7777;
	    zMin = -7777;
	}

    rotate()
      {
	x = 0;
	y = 0;
	z = 0;
	result = NULL;
	xMin = -7777;
	yMin = -7777;
	zMin = -7777;
      }

    /**
       this allows limits to be set for this rotate object
       leaves untouched if 0, if ?min != 0 so mus ?max
    */
    void setLimits(double xmin, double xmax,
		   double ymin, double ymax,
		   double zmin, double zmax)
	{
	    if (xmin != 0) xMin = xmin;
	    if (xmax != 0) xMax = xmax;
	    if (ymin != 0) yMin = ymin;
	    if (ymax != 0) yMax = ymax;
	    if (zmin != 0) zMin = zmin;
	    if (zmax != 0) zMax = zmax;
	}

    /**
       Enforces the above limits, if setLimits has been called
     */
    void checkLimits()
	{
	    if (xMin != -7777)
	    {
		if (x < xMin) 
		{
		    x = xMin;
		}
		else if (x > xMax)
		{
		    x = xMax;
		}
	    }

	    if (yMin != -7777)
	    {
		if (y < yMin) 
		{
		    y = yMin;
		}
		else if (y > yMax)
		{
		    y = yMax;
		}
	    }

	    if (zMin != -7777)
	    {
		if (z < zMin) 
		{
		    z = zMin;
		}
		else if (z > zMax)
		{
		    z = zMax;
		}
	    }
	}

    /**
       debuggin routine to set thte value of rotation for a limb
     */
    void set(double xx, double yy, double zz)
      {
	if (xx != 0) x = xx;
	if (yy != 0) y = yy;	
	if (zz != 0) z = zz;
	checkLimits();
      }

    void add(double xx, double yy, double zz)
      {
	x += xx;
	y += yy;	
	z += zz;
	checkLimits();
      }

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
      }

    void show()
      {
	printf("rotate x:%f, y:%f z: %f\n",x,y,z);
      }

    void matrixDeleted()
      {
	result = NULL;
      }

    /**
       This allocates space for another 4x4 matrix,
       it is the calling procedures duty to destroy the
       matrix when done

     */
    double** getMatrix()
      {
	// result + sin + cos of z and x
	double           cx,sx,sz,cz;

	if (result == NULL) make();

	result[0][0] = 4;
	cx = cos (x);
 	cz = cos (z);
	sx = sin (x);
	sz = sin (z);
	result[0][0] = cz;
	result[1][0] =  -sz*cx;
	result[2][0] = sz*sx;
	result[3][0] = 0;
	result[0][1] = sz;
	result[1][1] = cx*cz;
	result[2][1] = -cz*sx;
	result[3][1] = 0;
	result[0][2] = 0;
	result[1][2] = sx;
	result[2][2] = cx;
	result[3][2] = 0;
	result[0][3] = 0;
	result[1][3] = 0;
	result[2][3] = 0;
	result[3][3] = 1;

	return result;
      }

    /**
       This converts this rotate from a vector to a proper rotate
       we assume that x,y and z are normalised, result in degrees
    */
    void toRotate()
      {
	double a,b,c,d;

	a = x;
	b = y;
	c = z;
	d = sqrt(b*b+c*c);
	// this b malarkey is because of the properties of acos, asin!
	if (b > 0)
	  {
	    x = -180*acos(c/d)/M_PI;
	  }
	else
	  {
	    x = 180*acos(c/d)/M_PI;
	  }
	y = 180*asin(-a)/M_PI;
	z = 0;
      }

    /**
       Treats as a vector and normalises
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
       As above, but normalises to a particular length
     */
    void normalise(double size)
      {
	double mag;
	mag = sqrt(x*x+y*y+z*z);
	x /= mag*size;
	y /= mag*size;
	z /= mag*size;
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

    ~rotate()
      {
	//deleteMatrix(result);
      }

private:

};


#endif
