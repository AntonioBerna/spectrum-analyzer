#pragma once

#include "settings.h"

class EngineFFT {
public:
  EngineFFT(void);
  void displayInit(uint32_t baudrate = BAUD_RATE);
  float antiAliasingLowPassFilter(uint16_t sample);
  void removeDC(void);
  void windowing(void);    // only Hann Window type
  void fft(void);          // only forward direction
  void drawBigBars(void);

  uint32_t samplingPeriod; // microseconds
  double re[SAMPLES];
  double im[SAMPLES];

private:
  void swap(double *x, double *y);
  void createMagnitudes(void);
  double getPeak(void);
  
  uint8_t levels;
  uint8_t fontSize;
  Adafruit_SSD1306 display;
  float lastFilteredValue;

  const float alpha = 0.15f;
  const uint16_t maxBars = SCREEN_WIDTH / (BAR_WIDTH + BAR_SPACING);
};
