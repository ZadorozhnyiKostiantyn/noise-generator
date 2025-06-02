#include <driver/i2s.h>
#include <driver/ledc.h>

//
// ─── BUTTONS ────────────────────────────────────────────────────────────────────
//

// Button GPIO pins
#define BUTTON_YELLOW 13 // Yellow button
#define BUTTON_GREEN 12  // Green button
#define BUTTON_BLUE 14   // Blue button
#define BUTTON_RED 27    // Red button

// Button masks for bitwise operations
#define YELLOW_MASK 0b0001                                            // Mask for yellow button
#define GREEN_MASK 0b0010                                             // Mask for green button
#define BLUE_MASK 0b0100                                              // Mask for blue button
#define RED_MASK 0b1000                                               // Mask for red button
#define BUTTON_MASK (YELLOW_MASK | GREEN_MASK | BLUE_MASK | RED_MASK) // Combined mask for all buttons

// Button states
#define BUTTON_PRESSED LOW   // Logic level when button is pressed
#define BUTTON_RELEASED HIGH // Logic level when button is released

// Button timing settings
#define BUTTON_DEBOUNCE_DELAY 50     // Debounce delay in milliseconds
#define BUTTON_LONG_PRESS_DELAY 1000 // Long press threshold in milliseconds

//
// ─── LEDS ──────────────────────────────────────────────────────────────────────
//

// LED GPIO pins
#define LED_YELLOW 15 // Yellow LED
#define LED_GREEN 2   // Green LED
#define LED_BLUE 4    // Blue LED
#define LED_RED 16    // Red LED

//
// ─── AUDIO SAMPLING ────────────────────────────────────────────────────────────
//

// Global sample rate
#define SAMPLE_RATE 16000 // 16 kHz sample rate

//
// ─── I2S SPEAKER CONFIGURATION ─────────────────────────────────────────────────
//

// I2S speaker pin configuration
#define I2S_SPEAKER_BCK_IO_NUM GPIO_NUM_32   // Bit clock (BCLK)
#define I2S_SPEAKER_DATA_OUT_NUM GPIO_NUM_27 // Data out

// Ultrasound speaker
#define ULTRASOUND_PIN GPIO_NUM_26
#define ULTRASOUND_CHANNEL 1
#define ULTRASOUND_FREQUENCY 18000 // 21000
#define ULTRASOUND_RESOLUTION 8    // 8-bit resolution for ultrasound

//
// ─── SD CARD CONFIGURATION ─────────────────────────────────────────────────────
//

// SPI pins for SD card module
#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_SCK GPIO_NUM_18
#define PIN_NUM_MOSI GPIO_NUM_23
#define PIN_NUM_CS GPIO_NUM_5

//
// ─── I2S MICROPHONE CONFIGURATION ──────────────────────────────────────────────
//

// Microphone
#define ANALOG_MIC_PIN ADC1_CHANNEL_0
#define RECORD_DURATION_SECONDS 15 // Duration for recording in seconds
#define BITS_PER_SAMPLE 16

#define NUM_BYTES_TO_READ_FROM_FILE 1024
#define I2S_PORT I2S_NUM_0

#define BUFFER_SIZE 4000 // 2 × 4000 = 8000 вибірок
#define NUM_BUFFERS 2

//
// ─── I2S CONFIG OBJECTS ────────────────────────────────────────────────────────
//

// I2S config for internal ADC input
// i2s config for using the internal ADC
extern i2s_config_t i2s_speaker_config;
// i2s config for reading from of I2S
extern i2s_config_t i2s_mic_Config;
// i2s microphone pins
extern i2s_pin_config_t i2s_mic_pins;
// i2s speaker pins
extern i2s_pin_config_t i2s_speaker_pins;

// are you using an I2S microphone - comment this out if you want to use an analog mic and ADC input
#define USE_I2S_MIC_INPUT
// are you using an I2S amplifier - comment this out if you want to use the built in DAC
#define USE_I2S_SPEAKER_OUTPUT
#define GPIO_BUTTON GPIO_NUM_27