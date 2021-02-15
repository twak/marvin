//
// "$Id: Robot.h,v 1.5 2004/10/17 20:05:06 Administrator Exp $"
//
// Robot class definitions for the Fast Light Tool Kit (FLTK).
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
*  A trackball written by Daniel Keller has been added
*  a robot written by tom kelly has been added
*******************************************************************/

#ifndef ROBOT_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <FL/glut.h>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <FL/glut.h>
#endif

#include "include.h"
#include "leg.h"
#include "arm.h"
#include "point.h"
#include "body.h"
#include "wheel.h"
#include "bike.h"
#include "stars.h"
#include "sched.h"
#include "moon.h"

class Robot : public Fl_Gl_Window {

public:
    // this value determines the scaling factor used to draw the cube.
    double size;
    int frame;
    /**************************************************************
    *  Constructor
    **************************************************************/
    	Robot(int x,int y,int w,int h,const char *l=0);

    /************************************************************
     * Sets the x shift of the cube view camera.
     *
     * 	This function is called by the slider in RobotUI and the
     *  initialize button in RobotUI.
     ************************************************************/
    void panx(float x){xshift=x;};

    /************************************************************ 
     *  Sets the y shift of the cube view camera.
     *
     * This function is called by the slider in RobotUI and the
     * initialize button in RobotUI.
     ************************************************************/
    void pany(float y){yshift=y;};

    /********************************************************
     * The widget class draw() override.
     *
     *The draw() function initialize Gl for another round o f drawing
     * then calls specialized functions for drawing each of the
     * entities displayed in the cube view.
     *
     ******************************************************************/
    void draw();   

   
    /*****************************************************************
    *  Trackball routines written by Daniel Keller
    *******************************************************************/
    void     projectToSphere(int x, int y, double *result);
    void     trackballRotate(int x, int y);
    void     adjustRotFromMatrix(GLfloat *rotMatrix); 
    void     adjustRotState(int axis, double degrees); 
    /**************************************************************
    *  Handles FLTK events
    ***************************************************************/
    int	     handle(int event);

    void animateNow();
    void doLights();
    void doMotion(int i);
    //void animate();
    leg *leftLeg;
    arm *leftArm;
    body *marvin;
    sched *leftArmSched, *rightArmSched, *leftLegSched, *rightLegSched;
    // granny bike!
    bike *granny;
    stars *sky;
    moon *planet;
    // number of frames without keyboard input
    int framesNoInput;
    // rotation
    double radians;
    void setLook();

    int cameraRotate;
    double cameraDistA, cameraDistB;
    double cameraRotA, cameraRotB;

    point bikePos;
    void moveBike();
    double bikeDir;

    // location of bike connection points
    point lf;
    point rf;
    point lh;
    point rh;

    // animation key points
    point l1,l2,l3,r1,r2,r3;


    //wheel *frontWheel;
    void setLimb(int a,double i);
    void setTarget(double x, double y, double z);


private:

    /**********************************************************  
     *  Draw the cube boundaries
     *
     *  Draw the faces of the cube using the boxv[] vertices, using
     * GL_LINE_LOOP for the faces. The color is \#defined by CUBECOLOR.
     **********************************************************/
    void drawCube();
    //void drawLeg();    
    float xshift,yshift;

    // is the user currently holding a button down?
    int mouseDown;
    // last drag position
    int oldTurn;

    float boxv0[3];float boxv1[3];
    float boxv2[3];float boxv3[3];
    float boxv4[3];float boxv5[3];
    float boxv6[3];float boxv7[3];

    enum {
   	 X_AXIS = 0,
	    Y_AXIS = 1,
	    Z_AXIS = 2
	};
    /******************************************************
    * These values are used to set up the rotational matrix 
    * for the trackball
    *******************************************************/
    GLfloat rot_values[3];
    GLint vp[4];	//holds viewport
    double old_vector[3];
    double new_vector[3];
    GLfloat the_rotMatrix[16];
};

#define ROBOT_H 1
#endif

//
// End of "$Id: Robot.h,v 1.5 2004/10/17 20:05:06 Administrator Exp $".
//
