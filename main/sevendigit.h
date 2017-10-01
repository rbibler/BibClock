#include "utils.h"
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"


#define SDAT  GPIO_NUM_5
#define SOE   GPIO_NUM_18
#define SCLK  GPIO_NUM_19

void sevenDigitInit(void);
void shiftInit(void);
void displayDigit(uint8_t digitIndex, uint8_t digitToDisplay, uint8_t dot, bool flipped);
void shiftByte(uint8_t byteToShift);
