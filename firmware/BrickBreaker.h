#include "App.h"

#define  COLOR_GREY       0x202020

class BrickBreaker : public App {
private:
  int lifes;
  int lifes_max;
  int paddle_width;
  int paddle_min;
  int paddle_max;
  int paddle_pos;
  int score;
  
  int ball_x;
  int prev_ball_x;
  int vel_x;
  int ball_y;
  int prev_ball_y;
  int vel_y;

  int powerup_x;
  int powerup_y;
  int powerup_type; // 0 => none on board, 1 => bigger, 2 => smaller, 3 => extra life
  int powerup_percent;
  int powerup_wait;
  int powerup_blink_freq;
  int powerup_blink_count;
  
  float speedup;
  
  int pause;

  unsigned int rows_count;
  byte* blocks;
  color_t block_colors[5];
  int blocks_cnt;
  
  unsigned long currentTime;
  unsigned long prevUpdateTime;
  unsigned long prevControlTime;
  int gameSpeed;
  int gameSpeed_start;
  int base_level;
  int level;
  input_t lastControl;
  unsigned long rumbleUntil;

  void varInit();
  void blocksInit();
  void randomPowerup(int x, int y);
  bool inRange(int x);
  
public:
  BrickBreaker(size_t width, size_t height);
  virtual void handleInput(Input &input);
  virtual void run(unsigned long tick);
  virtual void render(Canvas &canvas);
};
