#include "BrickBreaker.h"

BrickBreaker::BrickBreaker(size_t width, size_t height)
  : App(width, height), 
    lifes(3),
    paddle_width(3),
    paddle_pos(0),
    score(0),
    ball_x(0), ball_y(1),
    prev_ball_x(1), prev_ball_y(0),
    vel_x(1), vel_y(1),
    powerup_active(0),
    
    pause(1),
    rows_count(5),

    prevUpdateTime(0), gameSpeed(180)
{
	varInit();
	blocksInit();
}

void BrickBreaker::varInit()
{
	// Initialize array of blocks
	blocks = new byte[rows_count * (width+1)];
	// Initialize colors
	block_colors[0] = 0xFF0000;
	block_colors[1] = 0xFFFF00;
	block_colors[2] = 0x00FF00;
	block_colors[3] = 0x00FFFF;
	block_colors[4] = 0x0000FF;
	
	ball_x = random(0,width);
	if (random(0,1)) {
		vel_x = 1;
	} else {
		vel_x = -1;
	}
}

void BrickBreaker::blocksInit()
{
	for (int i = 0; i < rows_count * (width+1); i++) {
		*(blocks + i) = 1;
	}
	blocks_cnt = rows_count * (width+1);
}

void BrickBreaker::run(unsigned long curTime)
{
	int next_x;
	int next_y;
	currentTime = curTime;
	
	if (pause) {
		return;
	}
	
	if ((curTime - prevUpdateTime) <gameSpeed)
	{
		return; 
	}
	// Once enough time  has passed, proceed. The lower this number, the faster the game is
	prevUpdateTime = curTime;

	// Check if level is solved
	if (blocks_cnt == 0) {
		//TODO: Show current score
		
		// Start new level
		pause = 1;
		blocksInit();

		ball_x = random(0,width);
		ball_y = 1;
		if (random(0,1)) {
			vel_x = 1;
		} else {
			vel_x = -1;
		}
		
	}
	
	// Check for bounds left, right, up
	if ((ball_x) == 0 && (vel_x < 0)) {
		vel_x = 1;
	}
	if ((ball_x == width) && (vel_x > 0)) {
		vel_x = -1;
	}
	if ((ball_y == height) && (vel_y > 0)) {
		vel_y = -1;
	}
	
	// If moving down and at the bottom, check for paddle hit
	if ((ball_y == 1) && (vel_y < 0)) {
		next_x = ball_x + vel_x;
		if ((next_x > paddle_pos) && (next_x < paddle_pos + paddle_width)) {
			// Hit!
			vel_y = 1;
			rumbleUntil = currentTime + 100;
		}
	}
	
	// Check for death
	if (ball_y == 0) {
		lifes--;
		if (lifes < 0) {
			//TODO: Game over, show score

/*			char curSelectionText[100];
			string Text = "GAME OVER! Score: " + score;
			Text.toCharArray(curSelectionText, Text.length());

			boolean finished = canvas.printText(curSelectionText, Text.length(), textPosition, (height-8)/2, COLOR_RED);
			if(finished) {
				textPosition = 0;
			}
*/
			ended = true;
			return;
		}
		//Reset Ball
		ball_x = paddle_pos + int(paddle_width / 2);
		vel_x = 1;
		vel_y = 1;
		pause = 1;
	}
	
	// Check for hit brick(s)
	next_x = ball_x + vel_x;
	next_y = ball_y + vel_y;
	if ((next_y > height - 2 - rows_count) && (next_y < height - 1)) {
		// We might hit something
		int offset = height - 1 - rows_count;
		int hit_something = 0;
		if (*(blocks + (next_y - offset) * (width+1) + ball_x)) {
			// Hit a block above
			*(blocks + (next_y - offset) * (width+1) + ball_x) = 0;
			blocks_cnt--;
			score++;
			hit_something = 1;
			vel_y = -vel_y;
		}
		if (*(blocks + (ball_y - offset) * (width+1) + next_x)) {
			// Hit a block by the side
			*(blocks + (ball_y - offset) * (width+1) + next_x) = 0;
			blocks_cnt--;
			score++;
			hit_something = 1;
			vel_x = -vel_x;
		}
		if ((!hit_something) && (*(blocks + (next_y - offset) * (width+1) + next_x))) {
			// Hit a block straight ahead
			*(blocks + (next_y - offset) * (width+1) + next_x) = 0;
			blocks_cnt--;
			score++;
			hit_something = 1;
			vel_x = -vel_x;
			vel_y = -vel_y;
		}
	}
	
	// Move ball
	ball_x += vel_x;
	ball_y += vel_y;
}

void BrickBreaker::render(Canvas &canvas)
{
	canvas.clear();

	//Paint ball
	canvas.setPixel(ball_x, ball_y, COLOR_WHITE);

	//Paint paddle
	for (int i = paddle_pos; i < paddle_pos + paddle_width; i++) {
		canvas.setPixel(i, 0, COLOR_WHITE);
	}
  
	//Paint lifes
	for (int if= 0; i < lifes; i++) {
		canvas.setPixel(i*2, height, COLOR_GREY);
	}
  
	//Paint blocks
	for (int y = height - 1 - rows_count; y < height - 1; y++) {
		int offset = height - 1 - rows_count;
		for (int x = 0; x < width + 1; x++) {
			if (*(blocks + (y - offset) * (width+1) + x)) {
				canvas.setPixel(x,y,block_colors[(y - offset) % 5]);
			}
		}		
	}
}

void BrickBreaker::handleInput(Input &input)
{
	//Check buttons and set paddle position while we are waiting to draw the next move
	input_t curControl = input.read();
	if (curControl == BTN_START){
		ended = true;
	}
	if (((curControl == BTN_LEFT) || (curControl == BTN_RIGHT)) && 
		((curControl != lastControl) || (currentTime - prevControlTime > int(gameSpeed/2)))) {
		if (curControl == BTN_LEFT) {
			pause = 0;
			if (paddle_pos > 0) {
				paddle_pos--;
			}
		} else {
			pause = 0;
			if ((paddle_pos + paddle_width - 1) < width) {
				paddle_pos++;
			}
		}
		prevControlTime = currentTime;
	}
	lastControl = curControl;
	
  if(currentTime < rumbleUntil) 
  {
    input.startRumble();
  }
  else
  {
    input.stopRumble();
  }
}
