#include "BrickBreaker.h"

BrickBreaker::BrickBreaker(size_t width, size_t height)
  : App(width, height), 
    lifes(3),
    paddle_width(3),
    paddle_pos(0),
    score(0),
    ball_x(0), ball_y(height - 2),
    prev_ball_x(1), prev_ball_y(0),
    vel_x(1), vel_y(-1),
    powerup_type(0),
    powerup_percent(5),
    lifes_max(5), paddle_min(2), paddle_max(6),
    pause(1),
    rows_count(5),

    prevUpdateTime(0), gameSpeed(380)
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
	
	ball_x = random(0,width+1);
	if (random(2)) {
		vel_x = 1;
	} else {
		vel_x = -1;
	}
	
	randomSeed(analogRead(0));

}

void BrickBreaker::blocksInit()
{
	for (int i = 0; i < rows_count * width; i++) {
		*(blocks + i) = 1;
	}
	blocks_cnt = rows_count * width;
}

bool BrickBreaker::inRange(int x) {
	return ((x >= 0) && (x < (rows_count * width)));
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
		ball_y = height - 2;
		if (random(0,1)) {
			vel_x = 1;
		} else {
			vel_x = -1;
		}
		vel_y = -1;
	}
	
	// Did we just catch a powerup?
	if (powerup_type && (powerup_y == height - 1) && (powerup_x >= paddle_pos) && (powerup_x < paddle_pos + paddle_width)) {
		switch(powerup_type) {
			case 1:
				// Grow
				if (paddle_width < paddle_max) {
					paddle_width++;
				}
				break;
			case 2:
				// Shrink
				if (paddle_width > paddle_min) {
					paddle_width--;
				}
				break;
			case 3:
				// Extra Life
				if (lifes < lifes_max) {
					lifes++;
				}
				break;
		}
		rumbleUntil = currentTime + 100;
		powerup_type = 0;
	}
	
	// If moving down and at the bottom, check for paddle hit
	if ((ball_y == height - 2) && (vel_y > 0)) {
		if ((ball_x >= paddle_pos) && (ball_x < paddle_pos + paddle_width)) {
			// Hit!
			vel_y = -1;
			rumbleUntil = currentTime + 100;
		}
	}
	
	// Check for death
	if (ball_y == height - 1) {
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
		//Reset Ball and paddle
		paddle_width = 3;
		if ((paddle_pos + paddle_width - 1) > width) {
			paddle_pos = width - paddle_width + 1;
		}
		ball_x = paddle_pos + int(paddle_width / 2);
		ball_y = height - 2;
		if (random(0,1)) {
			vel_x = 1;
		} else {
			vel_x = -1;
		}
		vel_y = -1;
		pause = 1;
		return;
	}
	
	// Check for hit brick(s)
	next_x = ball_x + vel_x;
	next_y = ball_y + vel_y;
  
	if ((next_y > 1) && (next_y < 1 + rows_count + 1)) {
		// We might hit something
		int offset = 2;
		int hit_something = 0;
		if ((inRange(((next_y - offset) * width) + ball_x)) && (*(blocks + ((next_y - offset) * width) + ball_x))) {
			// Hit a block above/below
			*(blocks + ((next_y - offset) * width) + ball_x) = 0;
			blocks_cnt--;
			score++;
			hit_something = 1;
			vel_y = -vel_y;
			next_y = ball_y + vel_y;
			randomPowerup(ball_x, next_y);
		}
		if ((inRange(((ball_y - offset) * width) + next_x)) && (*(blocks + ((ball_y - offset) * width) + next_x))) {
			// Hit a block by the side
			*(blocks + ((ball_y - offset) * width) + next_x) = 0;
			blocks_cnt--;
			score++;
			hit_something = 1;
			vel_x = -vel_x;
			next_x = ball_x + vel_x;
			randomPowerup(next_x, ball_y);
		}
		if ((!hit_something) && (inRange(((next_y - offset) * width) + next_x)) && (*(blocks + ((next_y - offset) * width) + next_x))) {
			// Hit a block straight ahead
			*(blocks + ((next_y - offset) * width) + next_x) = 0;
			blocks_cnt--;
			score++;
			hit_something = 1;
			vel_x = -vel_x;
			next_x = ball_x + vel_x;
			vel_y = -vel_y;
			next_y = ball_y + vel_y;
			randomPowerup(next_x, next_y);
		}
	}

	// Check for bounds left, right, up
	if ((ball_x == 0) && (vel_x < 0)) {
		vel_x = 1;
	}
	if ((ball_x == width - 1) && (vel_x > 0)) {
		vel_x = -1;
	}
	if ((ball_y == 0) && (vel_y < 0)) {
		vel_y = 1;
	}

	
	// Move powerup
	if (powerup_type) {
		if (powerup_y == height - 1) {
			powerup_type = 0;
		} else {
			powerup_y++;
		}
	}
	
	// Move ball
	ball_x += vel_x;
	ball_y += vel_y;
	
}

void BrickBreaker::randomPowerup(int x, int y)
{
	if (!powerup_type && (random(100) < powerup_percent)) {
		powerup_x = x;
		powerup_y = y-1;
		
		if (lifes >= lifes_max) {
			// No more extra lifes
			if (paddle_width <= paddle_min) {
				// No more shrinkers
				powerup_type = 1;
			} else if (paddle_width >= paddle_max) {
				// No more growers
				powerup_type = 2;
			} else {
				if (random(2)) {
					powerup_type = 1;
				} else {
					powerup_type = 2;
				}
			}
		} else {
			if (paddle_width <= paddle_min) {
				// No more shrinkers
				if (random(2)) {
					powerup_type = 1;
				} else {
					powerup_type = 3;
				}
			} else if (paddle_width >= paddle_max) {
				// No more growers
				if (random(2)) {
					powerup_type = 2;
				} else {
					powerup_type = 3;
				}
			} else {
				powerup_type = random(3)+1;
			}
		}
	}
}

void BrickBreaker::render(Canvas &canvas)
{
	canvas.clear();

	//Paint ball
	canvas.setPixel(ball_x, ball_y, COLOR_WHITE);

	//Paint paddle
	for (int i = paddle_pos; i < paddle_pos + paddle_width; i++) {
		canvas.setPixel(i, height - 1, COLOR_WHITE);
	}
  
	//Paint lifes
	for (int i = 0; i < lifes; i++) {
		canvas.setPixel(i*2, 0, COLOR_GREY);
	}
  
  	//Paint blocks
	for (int y = 2; y < 2 + rows_count; y++) {
		int offset = 2;
		for (int x = 0; x < width; x++) {
			if (*(blocks + ((y - offset) * width) + x)) {
				canvas.setPixel(x,y,block_colors[(y - offset) % 5]);
			}
		}		
	}
	
	//Paint powerup
	if (powerup_type) {
		switch(powerup_type) {
			case 1:
				// Make paddle bigger => green
				canvas.setPixel(powerup_x, powerup_y, COLOR_GREEN);
				break;
			case 2:
				// Make paddle smaller => red
				canvas.setPixel(powerup_x, powerup_y, COLOR_RED);
				break;
			case 3:
				// Extra life => grey
				canvas.setPixel(powerup_x, powerup_y, COLOR_GREY);
				break;
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
		((curControl != lastControl) || (currentTime - prevControlTime > int(gameSpeed/4)))) {
		if (curControl == BTN_LEFT) {
			pause = 0;
			if (paddle_pos > 0) {
				paddle_pos--;
			}
		} else {
			pause = 0;
			if ((paddle_pos + paddle_width) < (width)) {
				paddle_pos++;
			}
		}
	}
  prevControlTime = currentTime;
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
