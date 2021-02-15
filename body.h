#ifndef BODY_H

/**
   This is the body of the robot,
   it takes an origin (its arse), and 4 points
   for hands and feet.

   Every frame this takes new targets for the hands and feet, 
   updates the IK, and draws the whole robot when requested
*/

#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"
#include "piston.h"
#include "arm.h"
#include "leg.h"
#include "head.h"

class body
{
 public:
  // limbs
  arm *leftArm, *rightArm;
  leg *leftLeg, *rightLeg;
  robotHead *face;
  int frame;
  // arms is the location the arms and head are attached
  // connections points
  point arms, ll, rl, bottom;

  body(point arse, double size)
    {
      // relative position is the arse
      bottom = arms = ll = rl = arse;

      // set joint positions with the body
      ll.add(0.0,0.0, 0.3);
      rl.add(0.0,0.0,-0.3);
      arms.add(0.0,1.7,0.0);
      bottom.add(-0.2,0.2,0);

      // set each limb up
      leftArm  = new arm(arms, LEFT , 1.0);
      rightArm = new arm(arms, RIGHT, 1.0);
      leftLeg  = new leg(ll  , LEFT , 1.4);
      rightLeg = new leg(rl  , RIGHT, 1.4);
      // LEFT is unused here
      face = new robotHead (arms, LEFT, 1.0);
    }


  /**
     This draws the interconnecting pistons
   */
  void drawBody()
    {
      point rArmpit = rightArm ->getHook(2);
      point lArmpit = leftArm ->getHook(2);
      point lArmpit2 = leftArm ->getHook(1);
      point rArmpit2 = rightArm ->getHook(1);

      point hb1 = leftArm -> getHook(4);
      point hb2 = rightArm ->getHook(4);
      point hh1 = face -> getHook(3);
      point hh2 = face -> getHook(4);
      point hh3 = face -> getHook(5);

      piston p;

      //base
      p.drawPiston(bottom, ll, 0.2, 0.2, 3);
      p.drawPiston(ll, rl, 0.2, 0.2, 3);
      p.drawPiston(rl, bottom, 0.2,0.2,-4);

      // trunk
      p.drawPiston(arms,bottom, 0.3,0.8,-8);
      p.drawPiston(lArmpit, rl, 0.1,0.8,8);
      p.drawPiston(rArmpit, ll, 0.1,0.8,-12);
      p.drawPiston(ll, lArmpit2, 0.1,0.8,0);
      p.drawPiston(rl, rArmpit2, 0.1,0.8,4);

      // head attach one & two
      p.drawPiston(arms, hh1, 0.1,0.0,-1);
      p.drawPiston(hb1, hh2, 0.1,0.6,3);
      p.drawPiston(hb2, hh3, 0.1,0.6,5);

    }

  /**
     sets the frame
   */
  void setChest(point c)
    {
      arms = c;
      leftArm -> setPos(c);
      rightArm -> setPos(c);
      face -> setPos(c);
    }

/*   void setBob(double d) */
/*     { */
/*       bob = d; */
/*     } */

  void setTargets(point laT, point raT, point llT, point rlT, point hT)
    {
      leftArm  -> setTarget(laT);
      rightArm -> setTarget(raT);
      leftLeg  -> setTarget(llT);
      leftArm  -> setTarget(rlT);
      face     -> setTarget(hT);
    }

  /**
     This draws the body
  */
  void draw()
    {
      drawBody();
      // start with each limb
      leftArm ->draw();
      rightArm->draw();
      leftLeg ->draw();
      rightLeg->draw();
      face ->draw();
    }

};
#define BODY_H 1
#endif
