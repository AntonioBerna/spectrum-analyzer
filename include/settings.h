#pragma once

#include <Arduino.h>
#include <Wire.h>

// External Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Serial Communication Variables
constexpr uint32_t BAUD_RATE = 115200;
constexpr uint8_t ADC_RESOLUTION = 12;

// Audio Input Variables
constexpr uint8_t AUDIO_IN_PIN = 34;

// Adafruit OLED Display Variables
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C

// FFT Variables
constexpr uint16_t SAMPLE_RATE = 22050; // 22.05kHz (using Nyquist-Shannon Theorem)
constexpr uint16_t SAMPLES = 128;
constexpr uint8_t BAR_WIDTH = 2;
constexpr uint8_t BAR_SPACING = 2;
