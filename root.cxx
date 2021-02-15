#include "root.h"

// defines a small angle in radians
#define SMALL 0.01
// iterative accuracy
#define ACCURACY 0.01
#define TRUE 1
#define FALSE 0

/**
   Root main class with calculations
*/

int root::countChildren()
{
  int i = 0;
  joint *c;
  c = child;
  while (c != NULL)
    {
      //printf("cow %d 0x%x\n",i,c->child);
      c = c-> child;
      i++;
    }
  numChildren = i;
  return i;
}

/**
   does what it says, debugging routine
*/
void showMatrix(double **c)
{
  int i,j;
  printf("***************************\n");
  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++)
	{
	  printf(" %f ",c[j][i]);
	}
      printf("\n");
    }
  printf("***************************\n");      
}

/**
   Allocated space for a matrix
*/
double **makeMatrix()
{
  int i,j;
  double **result;
  result = (double**)malloc(sizeof(double)*4);
  for (i = 0; i < 4; i++)
    {
      result[i] = (double *)malloc(sizeof(double)*4);
      for (j = 0; j < 4; j++)
	{
	  result[i][j] = 0.0;
	}
    }

  // inialize to idnetity matrix
  for (i = 0; i < 4; i++) result[i][i] = 1.0;

  return result;
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

/**
   Calculates a.b, frees b, and returns new array
*/
double ** multiply(double **a, double **b)
{
  double v;
  int i,j,k;
  double **result;

  result = makeMatrix();

  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++)
	{
	  v = 0.0;
	  // calculate position i,j
	  for (k = 0; k < 4; k++)
	    {
	      v = v + a[i][k]*b[k][j];
	    }
	  result[i][j] = v;
	}
    }
  deleteMatrix (b);
  // free unwanted array
  return result;
}

/**
   calculates the endpoint of this chain

   we go through and set the location of each joint to the location of
   its parents, accumulating a position and rotation as we go

   */
point root::endPoint(int final)
{
  // this will return the identity matrix if requested
  point p = new point(), q = new point();
  joint *j = this;
  //point *length;
  double **matrix;

  matrix = makeMatrix();

  //if (child == NULL) return new point();
  //j = child;
  
  // move everything to the start position
  //matrix = multiply(this->parentPosition->getMatrix(),matrix);

  while (j != NULL)
    {
//       printf("before rotate:");
//       j->direction.show();
//       showMatrix(matrix);

      // 1) rotate to correct orientation
      if (j != this)matrix = multiply(j->direction.getMatrix(),matrix);
      // 2) move to parent position
      matrix = multiply(j->parentPosition->getMatrix(),matrix);

      // if final set up geometry, translate all hooks and
      // bits to relevant positions
      if (final)j -> setHooks(matrix);

      j = j -> child;
    }

  // return end point

  matrix = multiply(finalLength.getMatrix(),matrix);
  p.multiply(matrix);
  deleteMatrix(matrix);
  return p;
}


double absf(double a)
{
  if (a >= 0) return a;
  return -a;
}


/**
   This does the IK, using an iterative formula, setup the
   angles, so that the distance to the target is minimized

   for each limb, and each rotation we find the change in 
   distance, that changing the value a small ammount makes
 */
void root::doIK(point target)
{
  joint *j = this;

  double d1, d2, dx, oldVal;
  int done, direction;

  int jo;

  jo = 0;

  while (j != NULL)
    {
      //printf("joint %d\n",jo++);
      direction = 0;
      oldVal = 0;
      done = FALSE;
      while (!done)
	{
	  d1 = target.dist(endPoint(0));
	  // change angle by a little
	  j -> direction.add(SMALL,0,0);
	  d2 = target.dist(endPoint(0));
	  // undo change
	  j -> direction.add(-SMALL,0,0);
	  dx = d1-d2;
	  //printf("dx: %f\n",dx);

	  if (dx == oldVal) break;

	  if (dx > 0)
	    {
	      if (direction == 2) done = TRUE;
	      direction = 1;
	      j -> direction.add(SMALL*(j->tension),0,0);	      
	    }
	  else if (dx < 0)
	    {
	      if (direction == 1) done = TRUE;
	      direction = 2;
	      j -> direction.add(-SMALL*(j->tension),0,0);
	      // else dx == 0
	    }
	  else done = TRUE;
	  oldVal = dx;
	}

      direction = 0;
      done = FALSE;
      oldVal = 0;

      while (!done)
	{
	  d1 = target.dist(endPoint(0));
	  // change angle by a little
	  j -> direction.add(0,0,SMALL);
	  d2 = target.dist(endPoint(0));
	  // undo change
	  j -> direction.add(0,0,-SMALL);
	  dx = d1-d2;

	  if (dx == oldVal) break;
	  //printf("dz: %f\n",dx);
	  //printf("dx: %f\n",dx);

	  if (dx > 0)
	    {
	      if (direction == 2) done = TRUE;
	      direction = 1;
	      j -> direction.add(0,0,SMALL*(j->tension));	      
	    }
	  else if (dx < 0)
	    {
	      if (direction == 1) done = TRUE;
	      direction = 2;
	      j -> direction.add(0,0,-SMALL*(j->tension));
	      // else dx == 0
	    }
	  else done = TRUE;
	  oldVal = dx;
	}
      j = j -> child;
    }
  // update position of all geometry
  endPoint(1);
}



//BAD CODE



      // THIS IS NOT RUNNING
//        while (!done)
//  	{
	  
//  	  d1 = target.dist(endPoint(0));
//  	  // change angle by a little
//  	  j -> direction.add(0,0,SMALL);
//  	  d2 = target.dist(endPoint(0));
//  	  // undo change
//  	  j -> direction.add(0,0,-SMALL);
//  	  dz = d1-d2;

//  	  //printf("dz: %f\n",dz);

//  	  if (d2 == oldVal) done = trye;

//  	  if (absf(dz) > ACCURACY)
//  	    {
//  	      if (dz > 0)
//  		{
//  		  if (direction == 2) done = TRUE;
//  		  direction = 1;
//  		  j -> direction.add(0,0,SMALL);	      
//  		}
//  	      else if (dz < 0)
//  		{
//  		  if (direction == 1) done = TRUE;
//  		  direction = 2;
//  		  j -> direction.add(0,0,-SMALL);
//  		  // else dz == 0
//  		}
//  	      else done = TRUE;
//  	    }
//  	  oldVal = d2;
//  	}



      // printf("rotate matrix:");
//       showMatrix(j->direction.getMatrix());

//       printf("after rotate:");
//       showMatrix(matrix);


      //showMatrix(j->parentPosition->getMatrix());



      // 3) draw geometry

      // free all the matricies we have made

      // now calculate position for next time
      //p.add(j -> parentPosition);
//       if (j->child != NULL)
// 	{
// 	  // not an end point
// 	  length = j->child->parentPosition;
// 	} 
//       else
// 	{
// 	  // end second
// 	  length = finalLength;
// 	}


      // angle is j->direction (Y IS IGNORED)
      //matrix = multiply(j->direction.getMatrix(),matrix);

      //draw geomety for j here, by multiplying points
      // by the current matrix
      //if (final)
      //{
      //}


