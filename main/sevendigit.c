#include "sevendigit.h"

uint8_t digitBitsStandard[] = {
  //abcdefg
  0b1111110,          // 0
  0b0110000,          // 1
  0b1101101,          // 2
  0b1111001,          // 3
  0b0110011,          // 4
  0b1011011,          // 5
  0b1011111,          // 6
  0b1110000,          // 7
  0b1111111,          // 8
  0b1111011,          // 9
  0b0000000,          // A
  0b0000000,          // B
  0b0000000,          // C
  0b0000000,          // D
  0b0000000,          // E
  0b0000000,          // F
  0b0000000,          // Blank
};
uint8_t digitBitsFlipped[] = {
  //gdefabcp
  0b01111110,        // 0
  0b00110000,         // 1
  0b11101100,
  0b11111000,
  0b10110010,
  0b11011010,         // 5
  0b11011110,         // 6
  0b01110000,         // 7
  0b11111110,         // 8
  0b11110010,         // 9
  0b10111110,         // A
  0b11110010,         // B
  0b01111000,         // C
  0b01100110,         // D
  0b11111000,         // E
  0b10111000,         // F
  0b00000000          // blank

};



void sevenDigitInit(void) {
  gpio_set_direction(DIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
  gpio_set_direction(LOAD, GPIO_MODE_OUTPUT);

  writeToRegister(MAX7219_TEST, 0x01);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  writeToRegister(MAX7219_TEST, 0x00);
  writeToRegister(MAX7219_DECODE_MODE, 0x00);
  writeToRegister(MAX7219_BRIGHTNESS, 0x0F);
  writeToRegister(MAX7219_SCAN_LIMIT, 0x03);
  writeToRegister(MAX7219_SHUTDOWN, 0x01);
}

void writeToRegister(uint8_t registerToWrite, uint8_t data) {
  gpio_set_level(LOAD, LOW);
  shiftByte(registerToWrite);
  shiftByte(data);
  gpio_set_level(LOAD, HIGH);
}

void displayDigit(uint8_t digitIndex, uint8_t digitToDisplay, uint8_t dot, bool flipped) {
  uint8_t num = flipped ? digitBitsFlipped[digitToDisplay] : digitBitsStandard[digitToDisplay];
  num = flipped ? ((num & ~0x01) | (dot & 1)) : (num | (dot << 7));
  writeToRegister(digitIndex + 1, num);
}

void shiftByte(uint8_t byteToWrite) {
  for(int i = 0; i < 8; i++) {
    gpio_set_level(CLK, LOW);
    gpio_set_level(DIN, (byteToWrite >> (7 - i)) & 1);
    gpio_set_level(CLK, HIGH);
  }
}
