/* LedTable for Teensy3
 *
 * Written by: Klaas De Craemer, 2013
 * https://sites.google.com/site/klaasdc/led-table
 * 
 * Menu system for the LED table
 */

#include "Menu.h"

#include "SnakeGame.h"

#define MINSELECTION  1
#define MAXSELECTION  4

#define TEXTSPEED  110

Menu::Menu(size_t width, size_t height) 
  : App(width, height), curSelection(MINSELECTION), prevUpdateTime(0), textPosition(0), runSelection(false)
{
  
}

void Menu::handleInput(Input& input)
{
  Serial.println("Handle input");
  uint8_t curControl = input.read();
  if (curControl != BTN_NONE){
    if (curControl == BTN_LEFT){
      curSelection--;
      selectionChanged = true;
    } else if (curControl == BTN_RIGHT){
      curSelection++;
      selectionChanged = true;
    } else if (curControl == BTN_START){
      runSelection = true;
    }
    if(selectionChanged) {
      textPosition = 0; 
    }
    checkSelectionRange();
  }
}

void Menu::run(unsigned long tick)
{
  if(tick - prevUpdateTime > TEXTSPEED)
  {
    ++textPosition;
    prevUpdateTime = tick;
  }
}

void Menu::render(Canvas& canvas)
{
  char* curSelectionText;
  int curSelectionTextLength;
  canvas.clear();
  
  //Show menu system and wait for input
  switch (curSelection){
    case 1:
      curSelectionText = "1 Rainbow";
      curSelectionTextLength = 9;
      break;
    case 2:
      curSelectionText = "2 Tetris";
      curSelectionTextLength = 8;
      break;
    case 3:
      curSelectionText = "3 Snake";
      curSelectionTextLength = 7;
      break;
    case 4:
      curSelectionText = "4 Stars";
      curSelectionTextLength = 7;
      break;
  }
  boolean finished = canvas.printText(curSelectionText, curSelectionTextLength, textPosition, (height-8)/2, COLOR_RED);
  if(finished) {
    textPosition = 0;
  }
}
  

void Menu::controlFlow(AppController& appController)
{
  if(runSelection) {
    Serial.println("Run Selection true");
    switch(curSelection) {
      case 1:
        appController.startRainbow();
        break;
      case 2:
        appController.startTetris();
        break;
      case 3:
        appController.startSnake();
        break;
      case 4:
        appController.startStars();
        break;
    }
  }
}
  
void Menu::checkSelectionRange(){
  if (curSelection>MAXSELECTION){
    curSelection = MINSELECTION;
  } else if (curSelection<MINSELECTION){
    curSelection = MAXSELECTION;
  }
}
