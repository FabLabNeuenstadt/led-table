/* LedTable for Teensy3
 *
 * Written by: Klaas De Craemer, 2013
 * https://sites.google.com/site/klaasdc/led-table
 * 
 * Basic animation with fading stars
 */

#include "Stars.h"

#define  NB_NEWSTARS  2

Stars::Stars(size_t width, size_t height) 
  : App(width, height), prevUpdateTime(millis()), needsRendering(true), createNewStars(false)
{
  Serial.println("Stars created");
}

void Stars::handleInput(Input& input)
{
  input_t curControl = input.read();
  if (curControl == BTN_START){
    ended = true;
  }
}

void Stars::run(unsigned long curTime)
{
  static int wait = 80;
  if(curTime > prevUpdateTime + wait) 
  {
    if ((curTime - prevStarsCreationTime) > 500)
    {
      createNewStars = true;
      prevStarsCreationTime = curTime;
    }
    prevUpdateTime = curTime;
    needsRendering = true;
  } else {
    needsRendering = false;
  }
}

void Stars::render(Canvas& canvas)
{
  if(needsRendering)
  {
    canvas.dimLeds(0.97);
    
    if(createNewStars) {
      //Create new stars if enough time has passed since last time
      for (int i=0; i<NB_NEWSTARS; i++)
      {
        boolean positionOk = false;
        int x = 0, y = 0;
        //Generate random positions until valid
        while (!positionOk){
/*
          x = random(width);
          y = random(height);
*/
          if (canvas.getPixel(x,y) == 0)
          {
            positionOk = true;
          }
          if (x<width) {
			  x++;
		  } else {
			  x = 0;
			  y++;
		  }
        }
        canvas.setPixel(x, y, COLOR_WHITE);
      }
      createNewStars = false;
    }
  }
}

