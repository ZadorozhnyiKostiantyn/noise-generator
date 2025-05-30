#include "config.h"

//
// ─── I2S CONFIGURATION STRUCTS ─────────────────────────────────────────────────
//


i2s_config_t i2s_speaker_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,                            // Note, this will be changed later
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,       // high interrupt priority
    .dma_buf_count = 8,                             // 8 buffers
    .dma_buf_len = 1024,                            // 1K per buffer, so 8K of buffer space
    .use_apll=0,
    .tx_desc_auto_clear= true, 
    .fixed_mclk=-1    
};

// i2s speaker pins
i2s_pin_config_t i2s_speaker_pins = {
    .bck_io_num = 27,                                 // The bit clock connectiom, goes to pin 27 of ESP32
    .ws_io_num = 25,                                  // Word select, also known as word select or left right clock
    .data_out_num = 25, //25                               // Data out from the ESP32, connect to DIN on 38357A
    .data_in_num = I2S_PIN_NO_CHANGE                  // we are not interested in I2S data into the ESP32
};

// i2s config for reading from I2S
i2s_config_t i2s_mic_Config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};