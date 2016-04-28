#include "BrickBreaker.h"
#include "Arduino.h"

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
      powerup_percent(7),
      lifes_max(5), paddle_min(2), paddle_max(6),
      pause(1),
      rows_count(5),
      speedup(0.85),
      powerup_blink_freq(50),
      powerup_blink_count(0),
      prevUpdateTime(0), gameSpeed(600)
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
    base_level = 0;
    level = 0;

}

void BrickBreaker::blocksInit()
{
    for (int i = 0; i < rows_count * width; i++) {
        *(blocks + i) = 1;
    }
    blocks_cnt = rows_count * width;
    // We only ever get faster and faster
    gameSpeed_start = gameSpeed;
    level = base_level;
}

bool BrickBreaker::inRange(int x)
{
	// Helper function to help staying inside our array
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

    if ((curTime - prevUpdateTime) <gameSpeed) {
        return;
    }
    // Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;

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
        if (powerup_wait) {
            powerup_wait--;
        } else {
            if (powerup_y == height - 1) {
                powerup_type = 0;
            } else {
                powerup_y++;
            }
        }
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
        if ((vel_x > 0) && (ball_x + vel_x == paddle_pos)) {
            vel_x = -1;
            vel_y = -1;
            rumbleUntil = currentTime + 100;
        } else if ((vel_x < 0) && (ball_x + vel_x == paddle_pos + paddle_width - 1)) {
            vel_x = 1;
            vel_y = -1;
        }
    }

    // Check for death
    if (ball_y == height - 1) {
        lifes--;
        if (lifes < 0) {
            ended = true;
            return;
        }
        //Reset Ball and paddle, remove powerup and sleep for a second
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

        powerup_type = 0;
        pause = 1;

        delay(1000);
        return;
    }

    // Check for hit brick(s)
    next_x = ball_x + vel_x;
    next_y = ball_y + vel_y;

    if ((next_y) && (next_y < 1 + rows_count + 1)) {
        /* We might hit something
         * Checking for next_y (instead of next_y>1) may seem on first
         * glance like a off-by-one-error. It isn't.
         */

        int offset = 2;
        int hit_something = 0;
        /* There are some really weird edge cases (such as removing up
         * to 3 blocks in one step or strange things happening at the
         * edge of the field) which force us to do this with a while
         * loop. Trust me, I spent a few hours trying to find and
         * squashing all the problematic edge cases.
         */
        while (
            ((inRange(((next_y - offset) * width) + ball_x)) && (*(blocks + ((next_y - offset) * width) + ball_x))) ||
            ((inRange(((ball_y - offset) * width) + next_x)) && (*(blocks + ((ball_y - offset) * width) + next_x))) ||
            ((inRange(((next_y - offset) * width) + next_x)) && (*(blocks + ((next_y - offset) * width) + next_x)))
        ) {

            if ((inRange(((next_y - offset) * width) + ball_x)) &&
				(*(blocks + ((next_y - offset) * width) + ball_x))) {
                // We hit a block above/below
                *(blocks + ((next_y - offset) * width) + ball_x) = 0;
                blocks_cnt--;
                score++;
                hit_something = 1;
                randomPowerup(ball_x, next_y);
                vel_y = -vel_y;
                next_y = ball_y + vel_y;
                // Since nothing has changed in the x direction, we
                // shouldn't need to recheck bounds
            }

            if ((inRange(((ball_y - offset) * width) + next_x)) &&
				(*(blocks + ((ball_y - offset) * width) + next_x))) {
                // We hit a block by the side
                *(blocks + ((ball_y - offset) * width) + next_x) = 0;
                blocks_cnt--;
                score++;
                hit_something = 1;
                randomPowerup(next_x, ball_y);
                vel_x = -vel_x;
                next_x = ball_x + vel_x;
                // Recheck bounds
                if ((ball_x == 0) && (vel_x < 0)) {
                    vel_x = 1;
                    next_x = ball_x + vel_x;
                }
                if ((ball_x == width - 1) && (vel_x > 0)) {
                    vel_x = -1;
                    next_x = ball_x + vel_x;
                }
            }

            if ((inRange(((next_y - offset) * width) + next_x)) &&
				(*(blocks + ((next_y - offset) * width) + next_x))) {
                // We hit a block straight diagonally ahead
                *(blocks + ((next_y - offset) * width) + next_x) = 0;
                blocks_cnt--;
                score++;
                hit_something = 1;
                randomPowerup(next_x, next_y);
                vel_x = -vel_x;
                next_x = ball_x + vel_x;
                vel_y = -vel_y;
                next_y = ball_y + vel_y;
                // Recheck bounds
                if ((ball_x == 0) && (vel_x < 0)) {
                    vel_x = 1;
                    next_x = ball_x + vel_x;
                }
                if ((ball_x == width - 1) && (vel_x > 0)) {
                    vel_x = -1;
                    next_x = ball_x + vel_x;
                }
            }
        }

        if (hit_something) {
            /* Adjust the speed in 5 adjustments with a different base
             * speed for each solved level
             */
            if (blocks_cnt > (float)(4.0/5.0)*rows_count * width) {
                level = base_level;
            } else if (blocks_cnt > (float)(3.0/5.0)*rows_count * width) {
                level = base_level + 1;
            } else if (blocks_cnt > (float)(2.0/5.0)*rows_count * width) {
                level = base_level + 2;
            } else if (blocks_cnt > (float)(1.0/5.0)*rows_count * width) {
                level = base_level + 3;
            }
            gameSpeed = int((float)gameSpeed_start * pow(speedup,level));
        }

    }

    // Again, check for bounds left, right, up
    if ((ball_x == 0) && (vel_x < 0)) {
        vel_x = 1;
    }
    if ((ball_x == width - 1) && (vel_x > 0)) {
        vel_x = -1;
    }
    if ((ball_y == 0) && (vel_y < 0)) {
        vel_y = 1;
    }

    // Check if level is solved
    if (blocks_cnt == 0) {
        // Start new level
        pause = 1;
        base_level++;
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

    // Move ball
    ball_x += vel_x;
    ball_y += vel_y;

}

void BrickBreaker::randomPowerup(int x, int y)
{
	/* Here we chose a random powerup.
	 * We only chose one if no other one is on the playing field
	 * and if we are lucky enough.
	 * 
	 * Also, there can ever only be usable powerups, so for instance
	 * we won't see an extra life if we are already at the maximum.
	 */
    if (!powerup_type && (random(100) < powerup_percent)) {
        powerup_x = x;
        powerup_y = y;
        powerup_wait = 1;

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

    //Paint paddle
    for (int i = paddle_pos; i < paddle_pos + paddle_width; i++) {
        canvas.setPixel(i, height - 1, COLOR_WHITE);
    }

    //Paint lifes
    for (int i = 0; i < lifes; i++) {
        canvas.setPixel(i*2, 0, COLOR_GREY);
    }

    //Paint ball
    canvas.setPixel(ball_x, ball_y, COLOR_WHITE);

    //Paint blocks
    for (int y = 2; y < 2 + rows_count; y++) {
        int offset = 2;
        for (int x = 0; x < width; x++) {
            if (*(blocks + ((y - offset) * width) + x)) {
                canvas.setPixel(x,y,block_colors[(y - offset) % 5]);
            }
        }
    }

    //Paint blinking powerup
    if (powerup_type) {
        powerup_blink_count++;
        if (powerup_blink_count > 2* powerup_blink_freq) {
            powerup_blink_count = 0;
        }
        
        switch(powerup_type) {
        case 1:
            // Make paddle bigger => green
            if (powerup_blink_count < powerup_blink_freq) {
                canvas.setPixel(powerup_x, powerup_y, 0x002200);
            }
            break;
        case 2:
            // Make paddle smaller => red
            if (powerup_blink_count < powerup_blink_freq) {
                canvas.setPixel(powerup_x, powerup_y, 0x220000);
            }
            break;
        case 3:
            // Extra life => grey
            if (powerup_blink_count < powerup_blink_freq) {
                canvas.setPixel(powerup_x, powerup_y, COLOR_GREY);
            }
            break;
        }
    }
}

void BrickBreaker::handleInput(Input &input)
{
	/*
	 * Check buttons and set paddle position while we are waiting to
	 * draw the next move
	 * 
	 * We use readReal() so that we can move more smoothly without
	 * repeatedly having to press the left/right button
	 */

    input_t curControl = input.readReal();
    if (curControl == BTN_START) {
        ended = true;
    }

    if (((curControl == BTN_LEFT) || (curControl == BTN_RIGHT)) &&
            ((curControl != lastControl) || (currentTime - prevControlTime > int(gameSpeed/4.0)))) {

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
        prevControlTime = currentTime;
        lastControl = curControl;
    }

    if(currentTime < rumbleUntil) {
        input.startRumble();
    } else {
        input.stopRumble();
    }
}
