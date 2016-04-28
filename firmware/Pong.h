#include "App.h"

class Pong : public App {

private:
  int scorePlayerLeft;
  int scorePlayerRight;
  
  int positionPlayerLeft;
  int positionPlayerRight;
  
  int ballx;
  int previousBallx;
  int bally;
  int previousBally;
  int velocityx;
  int velocityy;
  int ballBounces;
  
  int gameSpeed;
  
  input_t lastControl;
  
  unsigned long currentTime;
  unsigned long prevUpdateTime;
  unsigned long prevControlTime;
  unsigned long lastAutoPlayerMoveTime;
  unsigned long rumbleUntil;
  unsigned long waitUntil;
  
  void checkBallOutOfBounds();
  void checkBallHitByPlayer();
  boolean moveAutoPlayer();
public:
  Pong(size_t width, size_t height);
  virtual void handleInput(Input &input);
  virtual void run(unsigned long tick);
  virtual void render(Canvas &canvas);
};
