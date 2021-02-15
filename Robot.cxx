//
// "$Id: Robot.cxx,v 1.6 2004/10/17 20:05:03 Administrator Exp $"
//
// Robot class implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

/*******************************************************************
*  This demo has been modified by Doanna Weissgerber
*  A trackball written by Daniel Keller has been added, it was then
*  modified by Tom Kelly to show a robot riding a bike
*******************************************************************/

#include "Robot.h"
#include "point.h"
#include "joint.h"
#include "leg.h"
#include "root.h"

// the frame rate
#define TIME 20
#define DIAM 10
//#include "/opt/local/glut-3.7/include/GL/glut.h"

//#define M_PI 3.141

double halfpi = M_PI/2;
double pi = M_PI;
double twopi = 2*M_PI;
Robot *me;

Robot::Robot(int x,int y,int w,int h,const char *l)
            : Fl_Gl_Window(x,y,w,h,l)
{
  size=5.0;
  xshift=0.0;
  yshift=6.0;
  me = this;
  frame = 0;
  framesNoInput = 0;
  
  // initalise var that rememvers if the user is holding a button down
  mouseDown = 0;
  oldTurn = 0;

  // rotation around globe
  radians = 0;
  // camera coordinates
  cameraRotate = 0;
  cameraDistB = 20;
  cameraDistA = 20;
  cameraRotB = 0;
  cameraRotA = 0;

  // set the location of the man
  bikePos.set(10,10,0);

  // bike direction
  bikeDir = 0.0;

  // set some of the key animation points
  r1.set(0,-2,-2);
  l1.set(0,-2,2);

  r2.set(1,-2,-2);
  l2.set(1,-2,2);

  // set this to what r1 normally was!
  r3.set(1,1,-3);
  l3.set(1,1,3);


    /* The cube definition. These are the vertices of a unit cube
     * centered on the origin.*/
    
    boxv0[0] = -0.5; boxv0[1] = -0.5; boxv0[2] = -0.5;
    boxv1[0] =  0.5; boxv1[1] = -0.5; boxv1[2] = -0.5;
    boxv2[0] =  0.5; boxv2[1] =  0.5; boxv2[2] = -0.5;
    boxv3[0] = -0.5; boxv3[1] =  0.5; boxv3[2] = -0.5;
    boxv4[0] = -0.5; boxv4[1] = -0.5; boxv4[2] =  0.5;
    boxv5[0] =  0.5; boxv5[1] = -0.5; boxv5[2] =  0.5;
    boxv6[0] =  0.5; boxv6[1] =  0.5; boxv6[2] =  0.5;
    boxv7[0] = -0.5; boxv7[1] =  0.5; boxv7[2] =  0.5;

	//set the rotation matrix to it's initial identity matrix
    memset(the_rotMatrix, 0, sizeof(the_rotMatrix));
    the_rotMatrix[0]=1;		 
    the_rotMatrix[5]=1;		 
    the_rotMatrix[10]=1;		 
    the_rotMatrix[15]=1;		 
}


void Robot::doLights()
{

  // ambient light
  GLfloat col4  [] = {0,0,0.5,0};

  // white color for highlights
  GLfloat white     [] = {0.55,0.5,0.2,1.0};
  //GLfloat black     [] = {0.0,0.0,0.0,1.0};
  GLfloat blue      [] = {4.0,4.0,5.0,1.0};

  // yellow spot light
  GLfloat light0Pos [] = {0.0 ,5.0, 0.0 ,1.0};
  GLfloat light0Dir [] = {0.0 ,-1.0, 0.0};
  GLfloat col0      [] = {30.0,30,0.0,1.0};

  // cyan light above & to one side
  GLfloat light1Pos [] = {20.0, 20.0 ,1.0 ,0.0};
  GLfloat col1      [] = {0.3, 0.3 ,0.1, 1.0};

  // white directional light above
  GLfloat light2Pos [] = {0.0 ,2.0,0.0,0.0};
  GLfloat col2      [] = {0.1, 0.1, 0.1, 1.0};

  // yellow spotlight
  glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0Dir);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, col0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 10);
  glLightf (GL_LIGHT0, GL_SPOT_EXPONENT,  128);

  // set constant brightness
  glLightf  (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
  glLightf  (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.9);
  glLightf  (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);

  // cyan positional
  glLightfv (GL_LIGHT1, GL_POSITION, light1Pos);
  glLightfv (GL_LIGHT1, GL_AMBIENT, col1);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, col1);
  glLightfv (GL_LIGHT1, GL_SPECULAR, white);
  glLightf  (GL_LIGHT1, GL_CONSTANT_ATTENUATION, 10);
  glLightf  (GL_LIGHT1, GL_LINEAR_ATTENUATION, 10);
  glLightf  (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01);

  // third light, white directional 
  glLightfv (GL_LIGHT2, GL_POSITION, light2Pos);
  glLightfv (GL_LIGHT2, GL_AMBIENT, col2);
  glLightfv (GL_LIGHT2, GL_DIFFUSE, col2);
  glLightfv (GL_LIGHT2, GL_SPECULAR, white);
  glLightf  (GL_LIGHT2, GL_CONSTANT_ATTENUATION, 10);
  glLightf  (GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.9);
  //glLightf  (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.2);

  //glLightfv (GL_LIGHT3, GL_POSITION, light1Pos);
  glLightfv (GL_LIGHT3, GL_AMBIENT, blue);
  glLightfv (GL_LIGHT3, GL_DIFFUSE, blue);
  glLightfv (GL_LIGHT3, GL_SPECULAR, white);
  glLightf  (GL_LIGHT3, GL_CONSTANT_ATTENUATION, 10);
  glLightf  (GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.02);
  glLightf  (GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 1);

  //glEnable(GL_LIGHT0);
      glEnable(GL_LIGHT1);
      //glEnable(GL_LIGHT2);
      //glEnable(GL_LIGHT3);

      glLightModelfv( GL_LIGHT_MODEL_AMBIENT, col4 );
}

/**
   convert degrees to radians
 */
  double r(double in)
    {
      return in*M_PI/180;
    }

/**
   These two function provided by Jes Frellsen, but unused at the mo
*/
void carToSpher(double x, double y, double z, double *r, double *theta, double *phi) {
  *r     = sqrt( x*x + y*y + z*z);

  if(x != 0)
    *theta = atan(y/x) + (x>=0 ? (y>=0 ? 0 : twopi) : pi);
  else
    *theta = (y>=0 ? halfpi : -halfpi);

  *phi   = acos (z/ *r);
}

void spherToCar(double r, double theta, double phi, double *x, double *y, double *z) {
  *x = r*cos(theta)*sin(phi);
  *y = r*sin(theta)*sin(phi);
  *z = r*cos(phi);
}


double mod(double a, double b)
{
  if (a > 0)
    {
      while (a > b) a= a - b;
    }
  else
    {
      a = 0-a;
      while (a > b) a= a - b;
      a = 0-a;
    }
  return a;
}

/*
  Does the math to move the bike to a new position

  Note this uses the mathematical convension for naming axis
  z is up!
*/
void Robot::moveBike()
{
  //bikeDir, bikePos
  double x,y,z;

  double r, phi, theta;

  double pi = M_PI;

  double lat1, lon1, d, tc, lon, lat,dlon;

  rotate rot;

  // set up the values nicely
  bikePos.normalise();
  bikePos.mul(10.0);

  printf("START\n");
  bikePos.show();

  x = bikePos.z;
  y = bikePos.x;
  z = bikePos.y;

  carToSpher(x,y,z,&r, &theta, &phi);

  printf("inital p: %f, t:%f\n",phi,theta);

  lat1 = phi;
  lon1 = theta;

  tc = 0;

  d = radians;

  lat =asin(sin(lat1)*cos(d)+cos(lat1)*sin(d)*cos(tc));
  dlon=atan2(sin(tc)*sin(d)*cos(lat1),cos(d)-sin(lat1)*sin(lat1));
  lon=mod(lon1-dlon +pi , 2*pi )-pi;

  // now we rotate by -theta then -phi, around 0,0,0
  // for theta we want to rotate around z

//   rot.set(theta,0,0);
//   bikePos.rot(&rot);

//   x = bikePos.z;
//   y = bikePos.x;
//   z = bikePos.y;

// //   rot.set(0,0,-phi);
// //   bikePos.rot(&rot);

//   bikePos.show();

  //carToSpher(x,y,z,&r, &theta, &phi);
  //printf("after whoodicky: %f, t:%f\n",phi,theta);

  // for phi we want to rotate around x

  //bikePos.rot(rot);

  phi = lat1;
  theta = lon1;

  spherToCar(r,phi,theta, &x, &y, &z);
  bikePos.set(z,x,y);

  //bikePos.set(10,10,0);

}

/**
   this is what is run every so often, depending on which animation
   sequence (scratching head, free wheeling etc...) is being run
   at the moment, we set the IK targets to different things
 */
void Robot::animateNow()
{
  radians = radians + 0.015;
  //moveBike();

  if (cameraDistA < cameraDistB) cameraDistA = cameraDistA + 0.5;
  if (cameraDistA > cameraDistB) cameraDistA = cameraDistA - 0.5;

  if (cameraRotA < cameraRotB) cameraRotA = cameraRotA + 0.5;
  if (cameraRotA > cameraRotB) cameraRotA = cameraRotA - 0.5;

  frame++;
  framesNoInput++;

  if (framesNoInput > 150)
    {
      doMotion((int)(rand()*10/(RAND_MAX+1.0)));
      framesNoInput = 0;
    }

  point c = new point(10,0,0);

  // this updates these pointer in Robot.h so that the
  // animation system can use them
  granny->getLocations (&lh,&rh,&lf, &rf);

  // bob twice as fast as we turn pedels
  c.set(0.2*sin(r((double)frame*P_DEGREES_FRAME*2+90)),1.6,0);

  // inform the schedulars that we are advancing a frame
  leftArmSched  -> frame();
  rightArmSched -> frame();
  leftLegSched  -> frame();
  rightLegSched -> frame();

  marvin -> setChest(c);

  marvin -> leftArm  -> setTarget (leftArmSched  -> getPoint());
  marvin -> rightArm  -> setTarget(rightArmSched -> getPoint());
  marvin -> leftLeg  -> setTarget (leftLegSched  -> getPoint());
  marvin -> rightLeg  -> setTarget(rightLegSched -> getPoint());

  marvin -> face  -> setTarget( lh);

  planet -> setPoint(granny -> direction);

//   marvin -> leftArm  -> setTarget( new point(0.3,0.3,cos (frame * 2* M_PI/ 300)-0.5));
//   marvin -> rightArm  -> setTarget( new point(0.3,0.3,sin (frame * 2* M_PI/ 300)-0.5));
//   marvin -> leftLeg  -> setTarget( new point(cos (frame * 2* M_PI/ 100)-0.5, sin (frame * 2* M_PI/ 100)-1.5,0));
//  marvin -> rightLeg  -> setTarget( new point(sin (frame * 2* M_PI/ 100)-0.5, cos (frame * 2* M_PI/ 100)-1.5,0));

  redraw();
}

/**
   static pointer to above object method
 */
void animate(int value)
{
  me -> animateNow();
  glutTimerFunc(TIME,animate,value+1);
}

void Robot::setLook()
{
  double cx,cy,cz;
  double lx,ly,lz;
  double ux,uy,uz;

  //double x,y,z;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  cx = 14*sin(radians) + cameraRotA*cos(radians);
  cy = 14*cos(radians) + -cameraRotA*sin(radians);
  cz = cameraDistA;

  // camera rotation moves the camera forwards or backwards


  lx = 12*sin(radians);
  ly = 12*cos(radians);
  lz = 0;

  // these two for bike move
  //cx = bikePos.x;
  //cy = bikePos.y;
  //cz = bikePos.z + 5;

  //lx = bikePos.x;
  //ly = bikePos.y;
  //lz = bikePos.z;

//   lx = 0;
//   ly = 0;
//   lz = 0;

   if (cameraRotate == 1)
     {
       ux = sin(radians);
       uy = cos(radians);
       uz = 0;
     }
   else
     {
       ux = 0;
       uy = 1.0;
       uz = 0;
     }

  // camera position,  looking at    up vector
  //gluLookAt(5,5,5,  0.0,0.0,0.0,  0.0,1.0,0.0);

  gluLookAt(cx,cy,cz,lx,ly,lz,ux,uy,uz);
}

void Robot::draw() {
    if (!valid()) {
        glLoadIdentity();
        glViewport(0,0,w(),h());
	glGetIntegerv(GL_VIEWPORT, vp);
        //glOrtho(-10,10,-10,10,-20030,10000);
        glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// round stars!
	glEnable(GL_POINT_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45,1,0.1,2000);

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

      //glutIdleFunc(animate);

      //leftLeg = new leg(new point(0,0,0), LEFT, 1.0);
      //leftArm = new arm(new point(1,0,0), RIGHT, 1.0);
      marvin = new body(new point(0,0,0), 1.0);
      granny = new bike(1.0);
      planet = new moon(9.0);
      {
	// set up the schedulars for each limb
	// first 2 lf's are dummies
	granny->getLocations (&lh,&rh,&lf, &rf);
	leftArmSched = new sched(&lh);
	rightArmSched = new sched(&rh);
	leftLegSched = new sched(&lf);
	rightLegSched = new sched(&rf);
      }
      // the stars!
      sky = new stars(4);
      // set up timer
      glutTimerFunc(TIME,animate,30);
    }

    setLook();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    //glTranslatef(xshift, yshift, 0);
    //glMultMatrixf(the_rotMatrix);
    //glScalef(float(size),float(size),float(size));

    doLights();

    //drawCube();
    sky -> draw();
    planet->draw();

    glPushMatrix();
    {
      //glScalef(0.5,0.5,0.5);
      glTranslatef(10*sin(radians), 10*cos(radians),0);
      glRotatef(-360*radians/(2*M_PI) ,0,0,1);
      glTranslatef(-1,1.6,0);

      marvin->draw();
      granny -> draw(frame);
    }
    glPopMatrix();

//     glTranslatef(bikePos.x, bikePos.y, bikePos.z);

// 	glPointSize(15);
//       glBegin(GL_POINTS);
//       {

// 	glVertex3f(0, 0, 0);
//       }
//       glEnd();

    glPopMatrix();
}


/* Project the given window point onto the rotation sphere */
void Robot::projectToSphere(int x, int y, double * result) 
{
    /* Calculate the x and y components */
    int width  = vp[2]-vp[0];
    int height = vp[3]-vp[1];
    double vx = ((2.0 * x) - width) / width;
    double vy = ((2.0 * y) - height) / height;

    /* And their length */
    double dxy = sqrt(vx*vx + vy*vy);

    /* Now calc the z component */
    double vz = 0;
    if (dxy < 1)
        vz = sqrt(1.0 - dxy);

    /* Calculate the normalizing factor */
    double norm = 1.0 / sqrt(vx*vx + vy*vy + vz*vz);
    
    /* Store the normalized result */
    result[0] = vx * norm;
    result[1] = vy * norm;
    result[2] = vz * norm;
}

/* Rotate to the given window point */
void Robot::trackballRotate(int x, int y)
{
    /* Project the current point onto the sphere */
    projectToSphere(x, y, new_vector);

    /* Calculate the distance it's moved */
    double dx = new_vector[0] - old_vector[0];
    double dy = new_vector[1] - old_vector[1];
    double dz = new_vector[2] - old_vector[2];

    /* Update the number of degrees of rotation */
    double angle = 90 * sqrt(dx*dx + dy*dy + dz*dz);

    /* Setup the axis to rotate around (cross product of the old and
     * new mouse vectors)
     */
    double ax = old_vector[1]*new_vector[2] - old_vector[2]*new_vector[1];
    double ay = old_vector[2]*new_vector[0] - old_vector[0]*new_vector[2];
    double az = old_vector[0]*new_vector[1] - old_vector[1]*new_vector[0];

    /* Now apply the new rotation... */
    glPushMatrix();
    glLoadIdentity();
    glRotatef(angle, ax, ay, az);
	
    /* Extract the per-axis rotation and upset our GUI to show it */
    GLfloat tmp[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
    adjustRotFromMatrix(tmp);

    /* Add in the rest of the accumulated matrix and save the result */
    glMultMatrixf(the_rotMatrix);
    glGetFloatv(GL_MODELVIEW_MATRIX, the_rotMatrix);
    glPopMatrix();

    /* Move the new position to the old... */
    old_vector[0] = new_vector[0];
    old_vector[1] = new_vector[1];
    old_vector[2] = new_vector[2];
}
//THIS MAY ONLY UPDATE SLIDERS?!?
/* Adjust our rotation amounts to take into account the new rotation in
 * the given matrix
 */
enum {
    X_AXIS = 0,
    Y_AXIS = 1,
    Z_AXIS = 2
};
void Robot::adjustRotFromMatrix(GLfloat *rotMatrix)
{
    /* Extract the x rotation and save it */
    double xRot = asin(rotMatrix[6]) * (180 / M_PI);
    adjustRotState(X_AXIS, xRot);

    /* Same for Y */
    double yRot = atan2(-rotMatrix[2], rotMatrix[10]) * (180 / M_PI);
    adjustRotState(Y_AXIS, yRot);

    /* And Z */
    double zRot = atan2(-rotMatrix[4], rotMatrix[5]) * (180 / M_PI);
    adjustRotState(Z_AXIS, zRot);

    /* Update the UI */
    //refreshOwner();
}
/* Keep track of how much rotation we've done */
void Robot::adjustRotState(int axis, double degrees)
{
    /* Update the current position */
    rot_values[axis] += degrees;
    if (rot_values[axis] > 360)
        rot_values[axis] -= 360;
    else if (rot_values[axis] < -360)
        rot_values[axis] += 360;
}

/**
   This schedules a the required action i=0...9
 */
void Robot::doMotion(int i)
{
  switch(i)
    {
    case 0:
      break;
    case 1:
      //hands by side
      rightArmSched -> add(&rf, 30, 100, LINEAR);
      rightArmSched -> add(&rh, 30, 0, LINEAR);
      // add a slight delay
      leftArmSched -> add(&lh, 10, 20, LINEAR);
      leftArmSched -> add(&lf, 30, 60, LINEAR);
      leftArmSched -> add(&lh, 30, 0, LINEAR);
      break;
    case 2:
      // steer with feet
      rightLegSched -> add(&rh, 30, 150, LINEAR);
      rightLegSched -> add(&rf, 30, 0, LINEAR);
      leftLegSched  -> add(&lf, 30, 0  , LINEAR);
      leftLegSched  -> add(&lh, 30, 120, LINEAR);
      leftLegSched  -> add(&lf, 30, 0  , LINEAR);
      break;
    case 3:
      // wave legs around a bit
      rightLegSched -> add(&r1, 30, 5, LINEAR);
      rightLegSched -> add(&r2, 10, 5, LINEAR);
      rightLegSched -> add(&r1, 10, 5, LINEAR);
      rightLegSched -> add(&r2, 10, 5, LINEAR);
      rightLegSched -> add(&r1, 10, 5, LINEAR);
      rightLegSched -> add(&r2, 10, 5, LINEAR);
      rightLegSched -> add(&r1, 10, 5, LINEAR);
      rightLegSched -> add(&r2, 10, 5, LINEAR);
      rightLegSched -> add(&rf, 30, 5, LINEAR);

      leftLegSched -> add(&l2, 30, 5, LINEAR);
      leftLegSched -> add(&l1, 10, 5, LINEAR);
      leftLegSched -> add(&l2, 10, 5, LINEAR);
      leftLegSched -> add(&l1, 10, 5, LINEAR);
      leftLegSched -> add(&l2, 10, 5, LINEAR);
      leftLegSched -> add(&l1, 10, 5, LINEAR);
      leftLegSched -> add(&l2, 10, 5, LINEAR);
      leftLegSched -> add(&l1, 10, 5, LINEAR);
      leftLegSched -> add(&l2, 10, 5, LINEAR);
      leftLegSched -> add(&l1, 10, 5, LINEAR);
      leftLegSched -> add(&l2, 10, 5, LINEAR);
      leftLegSched -> add(&lf, 30, 0, LINEAR);
      break;
    case 4:
      // pretend to be a plane
      rightArmSched -> add(&r3, 30, 100, LINEAR);
      rightArmSched -> add(&rh, 30, 0, LINEAR);
      leftArmSched -> add(&l3, 30, 100, LINEAR);
      leftArmSched -> add(&lh, 30, 0, LINEAR);
      break;
    case 5:
      {
	// scratch arm
	point *one, *two;
	one = &((root *)marvin->face->jj)->result[0];
	two = &((root *)marvin->face->jj)->result[4];
	rightArmSched -> add(one, 20, 0, LINEAR);
	rightArmSched -> add(two, 10, 0, LINEAR);
	rightArmSched -> add(one, 10, 0, LINEAR);
	rightArmSched -> add(two, 10, 0, LINEAR);
	rightArmSched -> add(&rh, 20, 0, LINEAR);
      }
      break;
    case 6:
      // 1-6 or 3-1
      {
	point *one, *two;
	one = &((root *)marvin->rightArm->jj)->result[1];
	two = &((root *)marvin->rightArm->jj)->result[6];
	rightArmSched -> add(&lh, 20, 80, LINEAR);
	rightArmSched -> add(&rh, 20, 0, LINEAR);

	leftArmSched -> add(&lh, 10, 10, LINEAR);
	leftArmSched -> add(one, 20, 0, LINEAR);
	leftArmSched -> add(two, 10, 0, LINEAR);
	leftArmSched -> add(one, 10, 0, LINEAR);
	leftArmSched -> add(two, 10, 0, LINEAR);
	leftArmSched -> add(one, 10, 0, LINEAR);
	leftArmSched -> add(two, 10, 0, LINEAR);
	leftArmSched -> add(&lh, 20, 0, LINEAR);
      }
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    }
}

int Robot::handle(int event)
{
    bool handled = false;
    int x = Fl::event_x();
    int y = vp[3] - Fl::event_y();

    int key;

    switch(event)
      {
      case FL_SHORTCUT:
	key = Fl::event_key();
	//printf("got the key %d\n",key);
	// this schedules different events
	if (key >= 48 && key <= 57)
	  {
	    framesNoInput = 0;
	    doMotion(key-48);
	    return 1;
	  }

	if (key == 114)
	  {
	    cameraRotate = 1-cameraRotate;
	    return 1;
	  }
	else if (key == 91)
	  {
	    cameraDistB = cameraDistB - 3;
	    return 1;
	  }
	else if (key == 93)
	  {
	    cameraDistB = cameraDistB + 3;
	    return 1;
	  }
	else if (key == 44)
	  {
	    cameraRotB = cameraRotB - 3;
	    return 1;
	  }
	else if (key == 46)
	  {
	    cameraRotB = cameraRotB + 3;
	    return 1;
	  }

	if (key == 65307)
	  {
	    exit(0);
	    return 1;
	  }
      }
    /* Handle each button */
    switch (Fl::event_button())
    {
    case FL_LEFT_MOUSE:
    	{
    		switch (event)
		{
		    case FL_PUSH:
		        projectToSphere(x, y, old_vector);
			oldTurn = 0;
			mouseDown = 1;
			return true;
		        break;
		
		    case FL_RELEASE:
			mouseDown = 0;
		        return true;
		        break;
	
		    case FL_DRAG:
		        trackballRotate(x, y);
			if (oldTurn == 0) oldTurn = x;
			if (mouseDown)
			  {
			    granny -> direction = granny -> direction + (x-oldTurn)*0.5;
			    if (granny -> direction >  80) granny -> direction = 80;
			    if (granny -> direction < -80) granny -> direction = -80;
                          oldTurn = x;
			  }
			redraw();	
		        return true;
		        break;
		}
    	}
    case FL_MIDDLE_MOUSE:
      handled = false;
      break;

    case FL_RIGHT_MOUSE:
      handled = false;
      break;
    }

    /* If we handled it, return 1 */
    if (handled)
    {
	redraw();
	return 1;
    }
    /* If we didn't handle it, pass it on */
    return Fl_Gl_Window::handle(event);
}


//
// End of "$Id: Robot.cxx,v 1.6 2004/10/17 20:05:03 Administrator Exp $".
//
