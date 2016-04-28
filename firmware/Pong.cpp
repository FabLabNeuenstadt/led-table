
#include "Pong.h"

#define PLAYER_HEIGHT 3
#define MAX_SCORE 5
#define AUTO_PLAYER_SPEED 200

Pong::Pong(size_t width, size_t height)
  : App(width, height), 
    scorePlayerLeft(0), scorePlayerRight(0),
    positionPlayerLeft(height/2), positionPlayerRight(height/2),
    ballx(width/2),bally(height/2), 
    velocityx(random(1)*2-1), velocityy(0),
    ballBounces(0),
    gameSpeed(180), prevUpdateTime(0), lastAutoPlayerMoveTime(0), rumbleUntil(0), waitUntil(0)
{

}

void Pong::run(unsigned long curTime)
{
  if(waitUntil > curTime)
  {
    return;
  }
  
  if(scorePlayerLeft == MAX_SCORE || scorePlayerRight == MAX_SCORE) 
  {
    ended = true;
    return;
  }

  currentTime = curTime;
  
  // Everytime since the player is movable everytime, not only once per game step
  checkBallHitByPlayer();
  
  if((curTime - lastAutoPlayerMoveTime) > AUTO_PLAYER_SPEED)
  {
    if(moveAutoPlayer()) 
    {
      lastAutoPlayerMoveTime = curTime;
    }
  }
  
  if ((curTime - prevUpdateTime) <gameSpeed-min(160, ballBounces*2))
  {
    return; 
  }
  
  ballx += velocityx;
  bally += velocityy;
  
  checkBallOutOfBounds();
  
  prevUpdateTime = curTime;
}

void Pong::render(Canvas &canvas)
{
  canvas.clear();
 
  // Draw score left
  for(int score=0; score<scorePlayerLeft; ++score) {
    canvas.setPixel(score, 0, DIM(COLOR_BLUE)); 
  }

  // Draw score left
  for(int score=0; score<scorePlayerRight; ++score) {
    canvas.setPixel(width-1-score, height-1, DIM(COLOR_RED)); 
  }

  // Draw ball
  canvas.setPixel(ballx,bally,COLOR_WHITE);
  
  // Draw player left
  for (int y=positionPlayerLeft-PLAYER_HEIGHT/2; y<=positionPlayerLeft+PLAYER_HEIGHT/2; ++y){
    canvas.setPixel(0, y, COLOR_BLUE);
  }
  // Draw player right
  for (int y=positionPlayerRight-PLAYER_HEIGHT/2; y<=positionPlayerRight+PLAYER_HEIGHT/2; ++y){
    canvas.setPixel(width-1, y, COLOR_RED);
  }
  
  canvas.show();
}

void Pong::handleInput(Input &input)
{
  input_t curControl = input.readReal();
  if (curControl == BTN_START){
    ended = true;
  }
  if (curControl != BTN_NONE &&
      ((curControl != lastControl) || (currentTime - prevControlTime > int(gameSpeed/4.0))))
  {
    if(curControl == BTN_DOWN && positionPlayerLeft + (PLAYER_HEIGHT-1) / 2 < height-1)
    {
      ++positionPlayerLeft;
    }
    else if(curControl == BTN_UP && positionPlayerLeft - PLAYER_HEIGHT / 2 > 0)
    {
      --positionPlayerLeft;
    }
	prevControlTime = currentTime;
    lastControl = curControl;
  }
  
  if(currentTime < rumbleUntil) 
  {
    input.startRumble();
  }
  else
  {
    input.stopRumble();
  }
}

void Pong::checkBallOutOfBounds()
{
  if(bally < 0) 
  {
    velocityy = - velocityy;
    bally = 1;
  } else if(bally > height-1) 
  {
    velocityy = - velocityy;
    bally = height-2;
  } 
  if(ballx < 0) 
  {
    velocityx = - velocityx;
    velocityy = 0;
    ballx = width/2;
    bally = height/2;
    ++scorePlayerRight;
    ballBounces = 0;
    waitUntil = currentTime + 2000;
  } 
  else if(ballx > width-1) 
  {
    velocityx = - velocityx;
    velocityy = 0;
    ballx = width/2;
    bally = height/2;
    ++scorePlayerLeft;
    ballBounces = 0;
    waitUntil = currentTime + 2000;
  } 
}

void Pong::checkBallHitByPlayer()
{
  if(ballx == 0) 
  {
    if(bally == positionPlayerLeft)
    {
      velocityx = 1;
      ballx = 2;
      ++ballBounces;
      rumbleUntil = currentTime + 200;
    } 
    else if(bally < positionPlayerLeft && bally >= positionPlayerLeft - PLAYER_HEIGHT / 2) 
    {
      velocityx = 1;
      velocityy = max(-1,velocityy-1); 
      ballx = 2;
      bally = positionPlayerLeft - PLAYER_HEIGHT / 2-1;
      ++ballBounces;
      rumbleUntil = currentTime + 200;
    }    
    else if(bally > positionPlayerLeft && bally <= positionPlayerLeft + (PLAYER_HEIGHT-1) / 2) 
    {
      velocityx = 1;
      velocityy = min(1,velocityy+1); 
      ballx = 2;
      bally = positionPlayerLeft + (PLAYER_HEIGHT-1) / 2+1;
      ++ballBounces;
      rumbleUntil = currentTime + 200;
    }    
  } 
  else if(ballx == width-1)
  {
    if(bally == positionPlayerRight)
    {
      velocityx = -1;
      ballx = width-3;
      ++ballBounces;
    } 
    else if(bally < positionPlayerRight && bally >= positionPlayerRight - PLAYER_HEIGHT / 2) 
    {
      velocityx = -1;
      velocityy = max(-1,velocityy-1); 
      ballx = width-3;
      bally = positionPlayerRight - PLAYER_HEIGHT / 2-1;
      ++ballBounces;
    }    
    else if(bally > positionPlayerRight && bally <= positionPlayerRight + (PLAYER_HEIGHT-1) / 2) 
    {
      velocityx = -1;
      velocityy = min(1,velocityy+1); 
      ballx = width-3;
      bally = positionPlayerRight + (PLAYER_HEIGHT-1) / 2+1;
      ++ballBounces;
    }    
  } 
}

boolean Pong::moveAutoPlayer()
{
  if(bally < positionPlayerRight)
  {
    if(positionPlayerRight - PLAYER_HEIGHT / 2>0) 
    {
      --positionPlayerRight;
      return true;
    }
  } 
  else if(bally > positionPlayerRight) 
  {
    if(positionPlayerRight + (PLAYER_HEIGHT-1) / 2 < height -1)
    {
      ++positionPlayerRight;
      return true;
    }      
  } 
  return false;
}

