#include "utils.h"
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include <driver/gpio.h>

#define MAX7219_TEST        0x0F
#define MAX7219_BRIGHTNESS  0x0a
#define MAX7219_SCAN_LIMIT  0x0B
#define MAX7219_DECODE_MODE 0x09
#define MAX7219_SHUTDOWN    0x0C

#define SEVEN_DIGIT_BLANK   0x10


#define DIN   2
#define CLK   4
#define LOAD  16

void sevenDigitInit(void);
void displayDigit(uint8_t digitIndex, uint8_t digitToDisplay, uint8_t dot, bool flipped);
void shiftByte(uint8_t byteToShift);
void writeToRegister(uint8_t registerToWrite, uint8_t byteToWrite);
