#include "fft.h"

EngineFFT engine = EngineFFT();

const uint32_t interval = engine.samplingPeriod;
uint32_t lastUpdate = 0;

void setup() {
  engine.displayInit();
  pinMode(AUDIO_IN_PIN, INPUT);
  lastUpdate = micros();
}

void loop() {
  // Acquisition of audio samples
  if (micros() - lastUpdate >= interval) {
    lastUpdate = micros();
    for (uint8_t i = 0; i < SAMPLES; ++i) {
      uint16_t sample = analogRead(AUDIO_IN_PIN);
      engine.re[i] = engine.antiAliasingLowPassFilter(sample);
      engine.im[i] = 0.0;
    }
    
    // Real-Time FFT
    engine.removeDC();
    engine.windowing();
    engine.fft();
    engine.drawBigBars();
  }
}
