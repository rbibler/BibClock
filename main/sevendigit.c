#include "sevendigit.h"
#include "driver/gpio.h"

uint8_t digitPins[] = {GPIO_NUM_2, GPIO_NUM_4, GPIO_NUM_16, GPIO_NUM_17};
uint8_t digitBitsStandard[] = {
  //gfedcba
  0b1000000,
  0b1111001,
  0b0100100,
  0b0110000,
  0b0011001,
  0b0010010,
  0b0000010,
  0b1111000,
  0b0000000,
  0b0010000,
  0xFF
};
uint8_t digitBitsFlipped[] = {
  //gfedfed
  0b11000000,            // 0
  0b11111001,            // 1
  0b01100100,            // 2
  0b01110000,            // 3
  0b01011001,            // 4
  0b01010010,            // 5
  0b01000010,            // 6
  0b11111000,            // 7
  0b01000000,            // 8
  0b01011000,             // 9
  0xFF
};

void shiftInit(void) {
  gpio_set_direction(SDAT, GPIO_MODE_OUTPUT);
  gpio_set_direction(SOE, GPIO_MODE_OUTPUT);
  gpio_set_direction(SCLK, GPIO_MODE_OUTPUT);
}

void sevenDigitInit(void) {
  shiftInit();
  for(int i = 0; i < 4; i++) {
    gpio_set_direction(digitPins[i], GPIO_MODE_OUTPUT);
  }
}

void displayDigit(uint8_t digitIndex, uint8_t digitToDisplay, uint8_t dot, bool flipped) {
  for(int i = 0; i < 4; i++) {
    if(i == digitIndex) {
      gpio_set_level(digitPins[i], HIGH);
    } else {
      gpio_set_level(digitPins[i], LOW);
    }
  }
  uint8_t num = flipped ? digitBitsFlipped[digitToDisplay] : digitBitsStandard[digitToDisplay];
  num = flipped ? ((num & ~0x40) | dot << 6) : (num | (dot << 7));
  shiftByte(num);
}

void shiftByte(uint8_t byteToShift) {
  gpio_set_level(SOE, HIGH);
  for(int i = 0; i < 8; i++) {
    gpio_set_level(SDAT, (byteToShift >> (7 - i)) & 1);
    gpio_set_level(SCLK, HIGH);
    gpio_set_level(SCLK, LOW);
  }
  gpio_set_level(SCLK, HIGH);
  gpio_set_level(SCLK, LOW);
  gpio_set_level(SOE, LOW);
}
