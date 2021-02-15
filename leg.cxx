#include "leg.h"

void leg::draw()
    {
      int i;
      point p;
      piston *t = new piston();
      point *res;

      GLfloat shine [] = {0.6,0.6,0.6,0.2};
      GLfloat spec [] = {0.6,0.6,0.6,0.2};

      GLfloat diffBlue []   = {0.0,0.8,1.0,1.0};


      if (0)
	{
      glBegin(GL_LINE_STRIP);
      {
	glColor4f(2.0, 1.0, 1.0, 1.0);
	for (i = 0; i <   ((root *)jj) -> numResult; i++)
	  {
	    p = ((root *)jj) -> result[i];
	    glVertex3f(p.x,p.y,p.z);	
	  }
      }
      glEnd();
      glBegin(GL_POINTS);
      {
	glColor4f(2.0, 0.0, 0.0, 1.0);
	for (i = 0; i <   ((root *)jj) -> numResult; i++)
	  {
	    p = ((root *)jj) -> result[i];
	    glVertex3f(p.x,p.y,p.z);
	  }

	glColor4f(0.0, 1.0, 1.0, 1.0);
	glVertex3f(target.x,target.y,target.z);
      }
      glEnd();


      glBegin(GL_POINTS);
      glColor4f(0.0, 1.0, 1.0, 1.0);
      glVertex3f(target.x,target.y,target.z);
      glEnd();

	}



      res = ((root *)jj)->result;

      // loop over all pistons in the leg
      for (i = 0; i < count; i++)
	{
	  // set the color
	  diffBlue[0] = pistonAttrib[i][3];
	  diffBlue[1] = pistonAttrib[i][4];
	  diffBlue[2] = pistonAttrib[i][5];

	  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffBlue);
	  glMaterialfv (GL_FRONT, GL_SPECULAR, spec);
	  glMaterialfv (GL_FRONT, GL_SHININESS, shine);

	   t->drawPiston(res[pistonMap[i][0]],res[pistonMap[i][1]], pistonAttrib[i][1], pistonAttrib[i][2], pistonAttrib[i][0]);
	}
	 delete t;
    }
