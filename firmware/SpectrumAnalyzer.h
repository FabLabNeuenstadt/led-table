#include "App.h"
#include <Audio.h>

class SpectrumAnalyzer : public App {
private:
	// These parameters adjust the vertical thresholds
	const float maxLevel = 0.5;      // 1.0 = max, lower is more "sensitive"
	const float dynamicRange = 40.0; // total range to display, in decibels
	const float linearBlend = 0.3;   // useful range is 0 to 0.7

	const unsigned int myColor = 0x400020;

	AudioInputAnalog*         adc1;       //xy=99,55
	AudioAnalyzeFFT1024*      fft;            //xy=265,75
	AudioConnection*          patchCord1;

	// This array holds the volume level (0 to 1.0) for each
	// vertical pixel to turn on.  Computed in setup() using
	// the 3 parameters above.
	float* thresholdVertical;

// This array specifies how many of the FFT frequency bin
// to use for each horizontal pixel.  Because humans hear
// in octaves and FFT bins are linear, the low frequencies
// use a small number of bins, higher frequencies use more.
	int* frequencyBinsHorizontal;

	void computeVerticalLevels();
	
public:
  SpectrumAnalyzer(size_t width, size_t height);
  virtual void handleInput(Input &input);
  virtual void run(unsigned long tick);
  virtual void render(Canvas &canvas);

};
