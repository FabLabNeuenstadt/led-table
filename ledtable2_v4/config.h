#define  FIELD_WIDTH       52
#define  FIELD_HEIGHT      11
#define HORIZONTAL_LEFT_TO_RIGHT

#define NUMBER_CONCURRENT_APPS 4

#define LEDS_CHANNEL_1 312
#define LEDS_CHANNEL_2 260
#define LEDS_CHANNEL_3 0
#define LEDS_CHANNEL_4 0
#define LEDS_CHANNEL_5 0
#define LEDS_CHANNEL_6 0
#define LEDS_CHANNEL_7 0
#define LEDS_CHANNEL_8 0

#define NUMBER_ALL_LEDS LEDS_CHANNEL_1+LEDS_CHANNEL_2+LEDS_CHANNEL_3+LEDS_CHANNEL_4+LEDS_CHANNEL_5+LEDS_CHANNEL_6+LEDS_CHANNEL_7+LEDS_CHANNEL_8

#define USE_OCTOWS2811 // Select either OCTOWS2811 or
//#define USE_FAST_LED   // FAST_LED as library to control the LED strips

/*
 * Some defines used by the FAST_LED library
 */
#define FAST_LED_CHANNEL_1_CHIPSET WS2811
//#define FAST_LED_CHANNEL_1_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_1_DATA_PIN  2
#define FAST_LED_CHANNEL_1_CLOCK_PIN 6

#define FAST_LED_CHANNEL_2_CHIPSET WS2811
//#define FAST_LED_CHANNEL_2_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_2_DATA_PIN  14
#define FAST_LED_CHANNEL_2_CLOCK_PIN 6

#define FAST_LED_CHANNEL_3_CHIPSET WS2801
#define FAST_LED_CHANNEL_3_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_3_DATA_PIN  7
#define FAST_LED_CHANNEL_3_CLOCK_PIN 6

#define FAST_LED_CHANNEL_4_CHIPSET WS2801
#define FAST_LED_CHANNEL_4_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_4_DATA_PIN  7
#define FAST_LED_CHANNEL_4_CLOCK_PIN 6

#define FAST_LED_CHANNEL_5_CHIPSET WS2801
#define FAST_LED_CHANNEL_5_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_5_DATA_PIN  7
#define FAST_LED_CHANNEL_5_CLOCK_PIN 6

#define FAST_LED_CHANNEL_6_CHIPSET WS2801
#define FAST_LED_CHANNEL_6_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_6_DATA_PIN  7
#define FAST_LED_CHANNEL_6_CLOCK_PIN 6

#define FAST_LED_CHANNEL_7_CHIPSET WS2801
#define FAST_LED_CHANNEL_7_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_7_DATA_PIN  7
#define FAST_LED_CHANNEL_7_CLOCK_PIN 6

#define FAST_LED_CHANNEL_8_CHIPSET WS2801
#define FAST_LED_CHANNEL_8_CHIPSET_WITH_CLOCK  // Comment if you use a chipset without clock
#define FAST_LED_CHANNEL_8_DATA_PIN  7
#define FAST_LED_CHANNEL_8_CLOCK_PIN 6

/*
 * Define to disable the USB Input
 */
//#define DISABLE_USB

//#define USE_CONSOLE_OUTPUT //Uncomment if you want to be able to display the led array on the serial console (for debugging)
//#define USE_CONSOLE_INPUT //Uncomment if you want to be able to play the game from the console

