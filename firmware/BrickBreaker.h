#include "App.h"

#define  COLOR_GREY       0x808080

class BrickBreaker : public App {
private:
  int lives;
  int paddle_width;
  int paddle_pos;
  int score;
  
  int ball_x;
  int prev_ball_x;
  int vel_x;
  int ball_y;
  int prev_ball_y;
  int vel_y;

  int pause;

  unsigned int rows_count;
  byte* blocks;
  color_t block_colors[5];
  int blocks_cnt;
  
  unsigned long currentTime;
  unsigned long prevUpdateTime;
  unsigned long prevControlTime;
  int gameSpeed;
  input_t lastControl;
  unsigned long rumbleUntil;

  void varInit();
  void blocksInit();

public:
  BrickBreaker(size_t width, size_t height);
  virtual void handleInput(Input &input);
  virtual void run(unsigned long tick);
  virtual void render(Canvas &canvas);
};
