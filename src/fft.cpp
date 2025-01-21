#include "fft.h"

EngineFFT::EngineFFT(void) {
  this->levels = log2(SAMPLES);
  this->samplingPeriod = round(1000000 * (1.0 / SAMPLE_RATE)); // T[us] = 1 / f => T * 10^(-6) = 1 / f => T = 10^6 / f
  this->fontSize = 1;
  this->lastFilteredValue = 0.0f;
}

void EngineFFT::displayInit(uint32_t baudrate) {
  Serial.begin(baudrate);
  Wire.begin();

  this->display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  if (!this->display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  this->display.setTextColor(WHITE);
  this->display.clearDisplay();
  this->display.display();
}

float EngineFFT::antiAliasingLowPassFilter(uint16_t sample) {
  // Normalize the sample value to a range between 0 and 1
  float currentValue = sample / ((1 << ADC_RESOLUTION) - 1.0f);

  // IIR Low-Pass Filter
  float filteredValue = alpha * currentValue + (1.0f - alpha) * this->lastFilteredValue;
  this->lastFilteredValue = filteredValue;

  return filteredValue;
}

void EngineFFT::removeDC(void) {
  double mean = 0.0;
  for (uint16_t i = 0; i < SAMPLES; ++i) {
    mean += this->re[i];
  }
  mean /= SAMPLES;
  for (uint16_t i = 0; i < SAMPLES; ++i) {
    this->re[i] -= mean;
  }
}

void EngineFFT::windowing(void) {
  // See conventions: https://en.wikipedia.org/wiki/Window_function#Examples_of_window_functions
  double samplesMinusOne = SAMPLES - 1.0;
  for (uint16_t i = 0; i < (SAMPLES >> 1); ++i) {
    double ratio = i / samplesMinusOne;

    // Hann Window factor
    double weighingFactor = 0.5 * (1.0 - cos(TWO_PI * ratio));

    // Applying Hann Window to samples
    this->re[i] *= weighingFactor;
    this->re[SAMPLES - (i + 1)] *= weighingFactor;
  }
}

void EngineFFT::swap(double *x, double *y) {
  double temp = *x;
  *x = *y;
  *y = temp;
}

void EngineFFT::fft(void) {
  // Bit Reversal Algorithm
  uint16_t j = 0;
  for (uint16_t i = 0; i < (SAMPLES - 1); ++i) {
    if (i < j) this->swap(&this->re[i], &this->re[j]);
    uint16_t k = SAMPLES >> 1;
    while (k <= j) {
      j -= k;
      k >>= 1;
    }
    j += k;
  }

  // Compute the FFT with Cooley–Tukey Algorithm

  // W_l = (re_W_l + j * im_W_l)
  double re_W_l = -1.0;
  double im_W_l = 0.0;
  
  // size of the next sub-DFT
  // after log2(samples) iterations => next_size = samples = N
  uint16_t next_size = 1;
  
  for (uint8_t l = 0; l < this->levels; ++l) { // levels = log2(samples)
    // size of the current sub-DFT
    // after log2(samples) iterations => curr_size = samples / 2 = N / 2
    uint16_t curr_size = next_size;
    next_size <<= 1;

    // Twiddle Factor: W_{N}^{k} = exp(-j * (2 * pi * k) / N)
    double re_W = 1.0;
    double im_W = 0.0;
    for (j = 0; j < curr_size; ++j) { // foreach sub-DFT
      for (uint16_t k = j; k < SAMPLES; k += next_size) { // Butterfly Operations
        uint16_t i = k + curr_size; // i = k + N / 2

        // W_{N}^{k} * O_k = (a + j * b) * (c + j * d)
        //                 = (a * c - b * d) + j * (a * d + b * c)
        //                 = re_temp + j * im_temp
        double re_temp = re_W * this->re[i] - im_W * this->im[i];
        double im_temp = re_W * this->im[i] + im_W * this->re[i];
        
        // X_{k + N / 2} = E_k - W_{N}^{k} * O_k
        // => X_i = E_k - (re_temp + j * im_temp)
        // => Re[X_i] = Re[E_k] - re_temp
        // => Im[X_i] = Im[E_k] - im_temp
        this->re[i] = this->re[k] - re_temp;
        this->im[i] = this->im[k] - im_temp;
        
        // X_k = E_k + W_{N}^{k} * O_k
        this->re[k] += re_temp;
        this->im[k] += im_temp;
      }

      // Calculation of the opposite twiddle factor to apply
      // butterfly operations to the next iteration
      double temp = re_W * re_W_l - im_W * im_W_l;
      im_W = re_W * im_W_l + im_W * re_W_l;
      re_W = temp;
    }

    // Before start the next level, we need to calculate
    // the square root of the twiddle factor
    im_W_l = -sqrt((1.0 - re_W_l) / 2.0);
    re_W_l = sqrt((1.0 + re_W_l) / 2.0);
  }
}

void EngineFFT::createMagnitudes(void) {
  for (uint16_t i = 0; i < SAMPLES; ++i) {
    this->re[i] = sqrt(sq(this->re[i]) + sq(this->im[i]));
  }
}

double EngineFFT::getPeak(void) {
  double peak = 0.0;
  for (uint16_t i = 0; i < SAMPLES; ++i) {
    peak = (peak > this->re[i]) ? peak : this->re[i];
  }
  return peak;
}

void EngineFFT::drawBigBars(void) {
  this->createMagnitudes();
  double peak = this->getPeak();
   
  this->display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

  for (uint16_t i = 0; i < maxBars && i < SAMPLES; ++i) {
    double magnitude = this->re[i];
    double logMagnitude = magnitude > 0.0 ? log(magnitude + 1.0) : 0.0;
    double normalizedHeight = (logMagnitude / log(peak + 1.0)) * SCREEN_HEIGHT;
    uint16_t barHeight = static_cast<uint16_t>(normalizedHeight);

    uint16_t x = i * (BAR_WIDTH + BAR_SPACING);
    uint16_t y = SCREEN_HEIGHT - barHeight;

    this->display.fillRect(x, y, BAR_WIDTH, barHeight, WHITE);
  }

  this->display.display();
}
