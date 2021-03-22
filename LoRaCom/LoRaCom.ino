/*
 * Base for TtyGo ESP32 board 
 * with Oled 
 */
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <Arduino.h>
#include <Wire.h> 
#include "SSD1306Wire.h" 
// oled settings
#define OLED_CLASS_OBJ  SSD1306Wire
#define OLED_ADDRESS    0x3C
#define OLED_SDA    4
#define OLED_SCL    15
#define OLED_RST    16
// LoRa settings
#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK  5
#define CONFIG_NSS  18
#define CONFIG_RST  14
#define CONFIG_DIO0 26
#define BAND 868E6  // 868 Mhz Europe
#define LORA_PERIOD 433  // unused
String buffer_in = "";
String buffer_out = "";
SSD1306Wire display(OLED_ADDRESS, OLED_SDA, OLED_SCL);
// see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
// on how to create xbm files 
  #define LoRa_Logo_width 109
  #define LoRa_Logo_height 64
  const uint8_t LoRa_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x05, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xA9, 0x5A, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 
  0xA6, 0x51, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x9A, 0x01, 0xA8, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x40, 0x0A, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x16, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x6A, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x90, 0x01, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x14, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x80, 0xA6, 
  0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0xA8, 0x02, 
  0xE0, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x80, 
  0x56, 0x1B, 0xC0, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7F, 
  0x00, 0x80, 0x01, 0x40, 0xE0, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x3F, 0x00, 0x00, 0x00, 0x20, 0xC0, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0x1F, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0F, 
  0xE0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0xF8, 0x01, 
  0xE0, 0x0F, 0xC0, 0x1F, 0xC0, 0x3F, 0x58, 0x02, 0x80, 0x3F, 0x00, 0x00, 
  0xFF, 0x0F, 0xE0, 0x0F, 0xC0, 0x1F, 0xF0, 0xFF, 0x51, 0x02, 0x80, 0x7F, 
  0x00, 0x80, 0xFF, 0x3F, 0xC0, 0x1F, 0x80, 0x3F, 0xF8, 0xFF, 0x83, 0x02, 
  0x80, 0x3F, 0x00, 0xC0, 0xFF, 0x7F, 0xC0, 0x0F, 0xC0, 0x3F, 0xFC, 0xFF, 
  0x03, 0x00, 0x80, 0x3F, 0x00, 0xE0, 0xFF, 0xFF, 0xE0, 0x1F, 0xC0, 0x1F, 
  0xFE, 0xF0, 0x07, 0x00, 0x00, 0x3F, 0x00, 0xF0, 0x0F, 0xFE, 0xE0, 0x0F, 
  0xE0, 0x1F, 0x7E, 0xE0, 0x07, 0x00, 0x80, 0x3F, 0x00, 0xF0, 0x07, 0xFC, 
  0xC1, 0xDF, 0xFE, 0x1F, 0x7C, 0xE0, 0x07, 0x00, 0x80, 0x3F, 0x00, 0xF0, 
  0x03, 0xF8, 0xC1, 0xFF, 0xFF, 0x0F, 0x00, 0xE0, 0x07, 0x00, 0x80, 0x7F, 
  0x00, 0xF0, 0x03, 0xF8, 0xE1, 0xFF, 0xFF, 0x07, 0x00, 0xFE, 0x07, 0x00, 
  0x80, 0x3F, 0x00, 0xF0, 0x03, 0xF8, 0xC1, 0xFF, 0xFF, 0x03, 0xE0, 0xFF, 
  0x07, 0x00, 0x80, 0x3F, 0x00, 0xF8, 0x01, 0xF8, 0xE1, 0xFF, 0xFF, 0x00, 
  0xF8, 0xFF, 0x07, 0x00, 0x00, 0x3F, 0x00, 0xF8, 0x03, 0xF8, 0xC3, 0xBF, 
  0xFE, 0x00, 0xFC, 0xFF, 0x07, 0x00, 0x80, 0x7F, 0x00, 0xF0, 0x01, 0xF8, 
  0xE1, 0x0F, 0xFE, 0x01, 0xFE, 0xE1, 0x07, 0x00, 0x80, 0x3F, 0x00, 0xF8, 
  0x03, 0xF8, 0xC1, 0x0F, 0xFC, 0x03, 0x7E, 0xE0, 0x07, 0x00, 0x80, 0x3F, 
  0x00, 0xF0, 0x03, 0xF8, 0xE1, 0x0F, 0xFC, 0x03, 0x3F, 0xE0, 0x07, 0x00, 
  0x80, 0x7F, 0x24, 0xF0, 0x07, 0xFC, 0xC1, 0x1F, 0xF8, 0x07, 0x3F, 0xE0, 
  0x07, 0x00, 0x80, 0xFF, 0xFF, 0xF1, 0x07, 0xFE, 0xE0, 0x0F, 0xF0, 0x07, 
  0x3F, 0xF0, 0x07, 0x00, 0x80, 0xFF, 0xFF, 0xE1, 0xFF, 0xFF, 0xE0, 0x0F, 
  0xF0, 0x0F, 0xFF, 0xFC, 0x07, 0x00, 0x80, 0xFF, 0xFF, 0xC1, 0xFF, 0x7F, 
  0xC0, 0x0F, 0xE0, 0x0F, 0xFE, 0xFF, 0x07, 0x00, 0x80, 0xFF, 0xFF, 0x81, 
  0xFF, 0x3F, 0xC0, 0x0F, 0xE0, 0x1F, 0xFE, 0xFF, 0x07, 0x00, 0x80, 0xFF, 
  0xFF, 0x01, 0xFF, 0x1F, 0xE0, 0x1F, 0xC0, 0x3F, 0xFC, 0xCF, 0x07, 0x00, 
  0x00, 0xBB, 0xEB, 0x01, 0xFC, 0x07, 0x40, 0x0B, 0x80, 0x17, 0xF0, 0x87, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x85, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x94, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 
  0x00, 0xA0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x40, 0x19, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0x95, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x05, 0x00, 0x00, 0x14, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 
  0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 
  0x00, 0x40, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xA8, 0x25, 0x95, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x20, 0x59, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x6C, 0x16, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  #define WiFi_Logo_width 60
  #define WiFi_Logo_height 36
  const uint8_t WiFi_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x83, 0x01, 0x80, 0xFF, 0xFF, 0xFF,
  0x01, 0x00, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x00,
  0xC0, 0xFF, 0xFF, 0x7C, 0x00, 0x60, 0x0C, 0x00, 0xC0, 0x31, 0x46, 0x7C,
  0xFC, 0x77, 0x08, 0x00, 0xE0, 0x23, 0xC6, 0x3C, 0xFC, 0x67, 0x18, 0x00,
  0xE0, 0x23, 0xE4, 0x3F, 0x1C, 0x00, 0x18, 0x00, 0xE0, 0x23, 0x60, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xE0, 0x03, 0x60, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x07, 0x60, 0x3C, 0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C,
  0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00, 0xE0, 0x8F, 0x71, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x08, 0x00,
  0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x1F,
  0x00, 0x00, 0x06, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  #define btlogo_Logo_width 104
  #define btlogo_Logo_height 64
  const uint8_t btlogo_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 
  0xFC, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xF8, 0x00, 0x00, 
  0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0xFC, 0x00, 
  0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x7E, 
  0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 
  0x3F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0xF8, 0x00, 
  0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x1F, 0x00, 0x00, 0x00, 0xF8, 
  0x00, 0x80, 0x1F, 0x70, 0x00, 0x00, 0x00, 0x3C, 0x7F, 0x00, 0x00, 0x0E, 
  0xF0, 0x01, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0x00, 0x00, 
  0x1F, 0xF0, 0x03, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x3C, 0xFC, 0x00, 
  0x00, 0x3F, 0xE0, 0x03, 0xE0, 0x03, 0xFE, 0x00, 0x00, 0x00, 0x3C, 0xF8, 
  0x03, 0x00, 0x7E, 0xC0, 0x07, 0xE0, 0x03, 0x7E, 0x00, 0x00, 0x00, 0x3C, 
  0xF0, 0x03, 0x00, 0x7E, 0xC0, 0x07, 0xE0, 0x01, 0x3F, 0x00, 0x80, 0x01, 
  0x3C, 0xE0, 0x07, 0x00, 0xF8, 0x80, 0x0F, 0xF0, 0x01, 0x1F, 0x00, 0x80, 
  0x03, 0x3C, 0xC0, 0x0F, 0x00, 0xF8, 0x81, 0x0F, 0xF0, 0x81, 0x0F, 0x08, 
  0xC0, 0x07, 0x3C, 0xE0, 0x07, 0x10, 0xF0, 0x01, 0x0F, 0xF8, 0x80, 0x0F, 
  0x3C, 0xC0, 0x0F, 0x3C, 0xF0, 0x03, 0x3C, 0xE0, 0x01, 0x0F, 0xF8, 0xC0, 
  0x07, 0x3E, 0x80, 0x1F, 0x3C, 0xF8, 0x01, 0x7C, 0xE0, 0x03, 0x1F, 0xF8, 
  0xC0, 0x07, 0x3F, 0x00, 0x7F, 0x3C, 0xFC, 0x00, 0xFC, 0xC0, 0x03, 0x1E, 
  0x78, 0xC0, 0x03, 0x1F, 0x00, 0xFE, 0x3C, 0x7E, 0x00, 0xF8, 0xC1, 0x07, 
  0x1E, 0x78, 0xE0, 0x83, 0x0F, 0x00, 0xFC, 0x3D, 0x3F, 0x00, 0xF0, 0xC1, 
  0x07, 0x3E, 0x7C, 0xE0, 0x81, 0x0F, 0x00, 0xF8, 0xBF, 0x1F, 0x00, 0xE0, 
  0xC1, 0x07, 0x3E, 0x7C, 0xE0, 0xC1, 0x07, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 
  0xE0, 0x83, 0x07, 0x3E, 0x7C, 0xE0, 0xC1, 0x07, 0x00, 0xE0, 0xFF, 0x07, 
  0x00, 0xE0, 0x83, 0x07, 0x3E, 0x3C, 0xE0, 0xC1, 0x07, 0x00, 0x80, 0xFF, 
  0x03, 0x00, 0xC0, 0x83, 0x0F, 0x3C, 0x3C, 0xE0, 0xC1, 0x03, 0x00, 0x00, 
  0xFF, 0x01, 0x00, 0xC0, 0x83, 0x0F, 0x3C, 0x3C, 0xE0, 0xC1, 0x03, 0x00, 
  0x00, 0xFE, 0x00, 0x00, 0xC0, 0x07, 0x0F, 0x3C, 0x3C, 0xE0, 0xC1, 0x03, 
  0x00, 0x00, 0xFE, 0x00, 0x00, 0xC0, 0x07, 0x0F, 0x3C, 0x3C, 0xE0, 0xC1, 
  0x03, 0x00, 0x00, 0xFF, 0x01, 0x00, 0xC0, 0x87, 0x0F, 0x3C, 0x3C, 0xE0, 
  0xC1, 0x03, 0x00, 0x80, 0xFF, 0x03, 0x00, 0xC0, 0x83, 0x0F, 0x3C, 0x3C, 
  0xE0, 0xC1, 0x07, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0xC0, 0x83, 0x0F, 0x3C, 
  0x7C, 0xE0, 0xC1, 0x07, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0xE0, 0x83, 0x07, 
  0x3E, 0x7C, 0xE0, 0x81, 0x07, 0x00, 0xF0, 0xBF, 0x3F, 0x00, 0xE0, 0x81, 
  0x07, 0x3E, 0x78, 0xE0, 0x83, 0x0F, 0x00, 0xF8, 0x3D, 0x7F, 0x00, 0xF0, 
  0xC1, 0x07, 0x3E, 0x78, 0xE0, 0x03, 0x0F, 0x00, 0xFC, 0x3C, 0xFC, 0x00, 
  0xF0, 0xC1, 0x07, 0x3E, 0xF8, 0xC0, 0x07, 0x3F, 0x00, 0x7E, 0x3C, 0xFC, 
  0x01, 0xF8, 0xC0, 0x07, 0x1E, 0xF8, 0xC0, 0x07, 0x3E, 0x00, 0x3F, 0x3C, 
  0xF0, 0x03, 0xFC, 0xE0, 0x03, 0x1E, 0xF8, 0xC0, 0x07, 0x3E, 0x80, 0x1F, 
  0x3C, 0xE0, 0x07, 0x7C, 0xE0, 0x03, 0x1F, 0xF0, 0x80, 0x0F, 0x18, 0xC0, 
  0x0F, 0x3C, 0xC0, 0x0F, 0x38, 0xF0, 0x01, 0x0F, 0xF0, 0x01, 0x0F, 0x00, 
  0xC0, 0x07, 0x3C, 0xE0, 0x07, 0x00, 0xF0, 0x01, 0x0F, 0xE0, 0x01, 0x1F, 
  0x00, 0x80, 0x03, 0x3C, 0xF0, 0x03, 0x00, 0xF8, 0x80, 0x0F, 0xE0, 0x03, 
  0x3E, 0x00, 0x00, 0x01, 0x3C, 0xF8, 0x01, 0x00, 0xFC, 0xC0, 0x07, 0xE0, 
  0x03, 0x7E, 0x00, 0x00, 0x00, 0x3C, 0xFC, 0x00, 0x00, 0x7E, 0xC0, 0x07, 
  0xC0, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0x00, 0x00, 0x3F, 0xE0, 
  0x07, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x3C, 0x3F, 0x00, 0x00, 0x3F, 
  0xE0, 0x03, 0x80, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0xBC, 0x1F, 0x00, 0x00, 
  0x1E, 0xF0, 0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0x00, 
  0x00, 0x00, 0xF8, 0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x0F, 
  0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0xFC, 
  0x03, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 
  0xFC, 0x01, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0xF8, 0x00, 0x00, 
  0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0xF0, 0x00, 
  0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xE0, 
  0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  };
 void setup() {
  Serial.begin(115200);
  //while (!Serial);
  Serial.println("init Oled");
  if (OLED_RST > 0) 
    {
      pinMode(OLED_RST, OUTPUT);
      digitalWrite(OLED_RST, HIGH);
      delay(100);
      digitalWrite(OLED_RST, LOW);
      delay(100);
      digitalWrite(OLED_RST, HIGH);
    }
    // SSD1306Wire display(0x3c, SDA,SCL);  
    Serial.println("init display");
    display.init();
    //display.flipScreenVertically();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    int Width = display.getWidth();
    int Height = display.getHeight();
    Serial.print(Width + "x");
    Serial.println(Height);
    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    } 
    // boot logos sequence  
    display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.display();
    delay(1500);
    display.clear();
    display.drawXbm(12, 0, btlogo_Logo_width, btlogo_Logo_height, btlogo_Logo_bits);
    display.display();
    delay(1500);
    display.clear();
    display.drawXbm(9, 0, LoRa_Logo_width, LoRa_Logo_height, LoRa_Logo_bits);
    display.display();
    delay(2000);
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128, "in : ");
    display.drawStringMaxWidth(0, 32, 128, "out: ");
    display.display();
 }
 
void loop() 
{
  // look en LoRa recept
  if (LoRa.parsePacket()) 
    {
        buffer_in = "";
        buffer_out = "[OK]";
        display.clear();
        while (LoRa.available()) 
          {
            buffer_in += (char)LoRa.read();
          }
    }
    //Serial.println(buffer_in);
    // look incoming message to send from serial
    // in future can use BT message / wifi etc ... 
    // serial is for proof of concept and educational
  if (buffer_out !="[OK]") 
  {
    while (Serial.available() > 0) {          
        buffer_out += String(char(Serial.read()));   
      }  
  } 
  if (buffer_in == "[OK]") 
  { 
    Serial.println(buffer_in);
    buffer_in="";  
    buffer_out=""; 
    display.drawString(0, 0, "in :");
    display.drawString(0, 32, "out:");
    display.display(); 
  }
  if (buffer_in !="" || buffer_out!="" ) 
  {     
    display.clear(); 
    display.drawString(0, 0, "in :");
    display.drawString(0, 32, "out:");
  }   
  if (buffer_in != "") 
  {
    Serial.println("< " + buffer_in); // push to serial incoming LoRa message
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128, "in : " + buffer_in);
    buffer_in="";
  }
  if (buffer_out !="")
  {
    LoRa.beginPacket();// push to LoRa incommiong serial message
    LoRa.print(buffer_out);
    LoRa.endPacket();
    Serial.println("> "+ buffer_out);
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    if ( buffer_out != "[OK]") {
      display.drawStringMaxWidth(0, 32, 128, "out: " + buffer_out);
    }
    buffer_out="";
  }
  display.display();  
}
