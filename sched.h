#ifndef SCHED_H
/**
   A schedular, this takes an animation type and a destination and updates
   the location of the point returned every frame. It allows animation
   destinations and types to be cached
*/
#include "include.h"
#include "point.h"
#include "joint.h"
#include "root.h"
#include "point.h"

// these are the types of animation
// use instant to move their immediately
#define INSTANT 1
#define ANGULAR 2
#define LINEAR  3


typedef struct Event 
{
  struct Event *next;
  // the point in question
  point *p;
  // frames to get to destination
  int fTG;
  // frames to stay at destination
  int fTS;
  // type of movement (one of above constants)
  int type;
} Event;

class sched
{

  point currentPos;
  // start point, the locaiton this movement started from
  point sP;
  // this is the point that we default to, when 
  // we have nothing else to do
  point *def;
  int animFrame;
  // the start of our queue
  Event *start;

 public:

  // creates an event
  Event *mkEvent()
    {
      Event *result;
      result = (Event *)malloc(sizeof(Event));
      result -> next = NULL;
      return result;
    }

  // adds a created event to the end of the list
  void addEvent(Event *in)
    {
      Event *e;
      if (start == NULL)
	{
	  start = in;
	  return;
	}
      e = start;
      while (e -> next != NULL)
	{
	  e = e-> next;
	}
      e -> next = in;
    }

  // removes the front event from the queue
  void delEvent()
    {
      Event *e;
      e = start;
      start = start -> next;
      // release the space back to the system
      free (e);
    }

  sched()
    {
    }

  // the current position of the point
  sched(point *cP)
    {
      currentPos = *cP;
      def = cP;
      start = NULL;
      animFrame = 0;
    }

  // removes all events from list
  void clear()
    {
      while (start != NULL)
	{
	  delEvent();
	}
    }

  // adds an event to the current queue
  void add(point *pp, int framesToGet, int framesToStay, int type)
    {
      Event *e;
      e = mkEvent();
      addEvent(e);
      e -> fTG = framesToGet;
      e -> fTS = framesToStay;
      e -> p  = pp;
      e -> type = type;
      sP = currentPos;
    }

  // called once a frame, updates point
  void frame()
    {
      if (start == NULL)
	{
	  // nothing better to do, currentPos is the default point
	  currentPos = *def;
	}
      else
	{
	  // we are working on the top item in the list
	  animFrame++;
	  if (animFrame <= start -> fTG)
	    {
	      // moving to position
	      switch (start -> type)
		{
		case INSTANT:
		  currentPos = start->p;
		  break;
		case LINEAR:
		  //
		  currentPos.setBetween(sP,start->p, (double)animFrame/start -> fTG);
		  break;
		case ANGULAR:
		  // sets current pos, so that animation has constant angular 
		  // velocity around 0,0,0
		  break;
		}
	    }
	  else  if (animFrame < start -> fTG + start -> fTS)
	    {
	      // stay at position - do nothing!
	      currentPos = *(start -> p);
	    }
	  else
	    {
	      // done! remove from start of queue, restart 
	      animFrame = 0;
	      sP = currentPos;
	      delEvent();
	      frame();
	    }
	}
    }

  // returns the position
  point getPoint()
    {
      return currentPos;
    }

};
#define SCHED_H 1
#endif
