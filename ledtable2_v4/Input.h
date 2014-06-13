#ifndef _INPUT_H_
#define _INPUT_H_

/* *** Game commonly used defines ** */
#define  DIR_UP    1
#define  DIR_DOWN  2
#define  DIR_LEFT  3
#define  DIR_RIGHT 4

/* *** USB controller button defines and input method *** */
#define  BTN_NONE  0
#define  BTN_UP    1
#define  BTN_DOWN  2
#define  BTN_LEFT  3
#define  BTN_RIGHT  4
#define  BTN_START  5

#include <Arduino.h>


typedef uint8_t input_t;

void UsbTask();

class Input {
public:
  input_t read();
};

#endif
