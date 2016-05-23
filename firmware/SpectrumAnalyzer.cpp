// LED Audio Spectrum Analyzer Display
// 
// Creates an impressive LED light show to music input
//   using Teensy 3.1 with the OctoWS2811 adaptor board
//   http://www.pjrc.com/store/teensy31.html
//   http://www.pjrc.com/store/octo28_adaptor.html
//
// Line Level Audio Input connects to analog pin A3
//   Recommended input circuit:
//   http://www.pjrc.com/teensy/gui/?info=AudioInputAnalog
//
// This example code is in the public domain.

#include "SpectrumAnalyzer.h"
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

/*
// The display size and color to use
const unsigned int matrix_width = 12;
const unsigned int matrix_height = 12;


// OctoWS2811 objects
const int ledsPerPin = matrix_width * matrix_height / 8;
DMAMEM int displayMemory[ledsPerPin*6];
int drawingMemory[ledsPerPin*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerPin, displayMemory, drawingMemory, config);
// Audio library objects
AudioInputAnalog         adc1(A3);       //xy=99,55
AudioAnalyzeFFT1024      fft;            //xy=265,75
AudioConnection          patchCord1(adc1, fft);

*/


SpectrumAnalyzer::SpectrumAnalyzer(size_t width, size_t height)
    : App(width, height)
{

  int helperArray[60] = {
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,
  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,
  5,  5,  6,  6,  6,  7,  7,  7,  8,  8,
  9,  9, 10, 10, 11, 12, 12, 13, 14, 15,
  15, 16, 17, 18, 19, 20, 22, 23, 24, 25
  };


	thresholdVertical = new float [height];
	frequencyBinsHorizontal = new int [width];
	for (int i=0; i < width; i++) {
	  frequencyBinsHorizontal[i] = helperArray[i];
	}
	adc1 = new AudioInputAnalog(A3);
	patchCord1 = new AudioConnection(*adc1, *fft);
  // the audio library needs to be given memory to start working
  AudioMemory(12);

  // compute the vertical thresholds before starting
  computeVerticalLevels();
}


/*

// Run setup once
void setup() {

  // turn on the display
  leds.begin();
  leds.show();
}

// A simple xy() function to turn display matrix coordinates
// into the index numbers OctoWS2811 requires.  If your LEDs
// are arranged differently, edit this code...
unsigned int xy(unsigned int x, unsigned int y) {
  if ((y & 1) == 0) {
    // even numbered rows (0, 2, 4...) are left to right
    return y * matrix_width + x;
  } else {
    // odd numbered rows (1, 3, 5...) are right to left
    return y * matrix_width + matrix_width - 1 - x;
  }
}
*/

void SpectrumAnalyzer::run(unsigned long curtime) {
}

void SpectrumAnalyzer::render(Canvas &canvas) {
  unsigned int x, y, freqBin;
  float level;
  if (fft->available()) {
	  canvas.clear();
    // freqBin counts which FFT frequency data has been used,
    // starting at low frequency
    freqBin = 0;

    for (x=0; x < width; x++) {
      // get the volume for each horizontal pixel position
      level = fft->read(freqBin, freqBin + frequencyBinsHorizontal[x] - 1);

      // uncomment to see the spectrum in Arduino's Serial Monitor
      // Serial.print(level);
      // Serial.print("  ");

      for (y=0; y < height; y++) {
        // for each vertical pixel, check if above the threshold
        // and turn the LED on or off
        if (level >= thresholdVertical[y]) {
			if (y < 3) {
				canvas.setPixel(x, y, COLOR_GREEN);
			} else if (y < 8) {
				canvas.setPixel(x, y, COLOR_YELLOW);
			} else {
				canvas.setPixel(x, y, COLOR_RED);
			}
        } else {
          //canvas.setPixel(x, y, 0x000000);
        }
      }
      // increment the frequency bin count, so we display
      // low to higher frequency from left to right
      freqBin = freqBin + frequencyBinsHorizontal[x];
    }
    // after all pixels set, show them all at the same instant
    //leds.show();
    // Serial.println();
  }
  
}

void SpectrumAnalyzer::handleInput(Input &input) {
    input_t curControl = input.readReal();
    if (curControl == BTN_START) {
        ended = true;
    }
}

// Run once from setup, the compute the vertical levels
void SpectrumAnalyzer::computeVerticalLevels() {
  unsigned int y;
  float n, logLevel, linearLevel;

  for (y=0; y < height; y++) {
    n = (float)y / (float)(height - 1);
    logLevel = pow10f(n * -1.0 * (dynamicRange / 20.0));
    linearLevel = 1.0 - n;
    linearLevel = linearLevel * linearBlend;
    logLevel = logLevel * (1.0 - linearBlend);
    thresholdVertical[y] = (logLevel + linearLevel) * maxLevel;
  }
}
