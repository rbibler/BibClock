#include "dimmer.h"

int gsValues[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


void dimmerInit() {
  // Pins 12-15 are setup for JTAG at boot. Need to configure them for GPIO
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[12], PIN_FUNC_GPIO);
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[13], PIN_FUNC_GPIO);
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[14], PIN_FUNC_GPIO);
  // Set all TLC5940 pins as output.
  gpio_set_direction(GCLK, GPIO_MODE_OUTPUT);
  gpio_set_direction(BLNK, GPIO_MODE_OUTPUT);
  gpio_set_direction(XLAT, GPIO_MODE_OUTPUT);
  gpio_set_direction(DSCLK, GPIO_MODE_OUTPUT);
  gpio_set_direction(SIN, GPIO_MODE_OUTPUT);
  gpio_set_level(BLNK, LOW);
  gpio_set_level(XLAT, LOW);
  gpio_set_level(DSCLK, LOW);
  init_pwm_channels();
}

/* Uses EPS32 LED PWM functionality.
* config struct is:
*   gpio_num - output
    speed_mode - clock speed
    channel - binds LED channel (1 of 16)
    intr_type - determines if there's an interrupt generated
    timer_sel - binds a hardware timer (defined sepearately)
    duty - duty cycle. Range is 1 to timer resets the counter, and counting
   begins again. BIT_NUM - 1

   TLC5940 operates thusly: Each rising edge of the grayscale clock increments
   the grayscale counter. Each LED channel's GS value is compared to counter. If
   LED channel GS value < counter, LED is turned off. Counter resolution is 4096
   When counter has reached 4095, it turns all channels off and waits for blank
   pulse. A low-high-low pulse on the BLANK pin resets the counter, and counting
   begins again.

   Here he have two PWM channels: GCLK and BLANK. BLANK is configured to pulse
   every 512 GSCLOCK cycles. This reduces the LED's grayscale resolution,
   but also decreases the necessary GCLK frequency. GCLK frequency is 80 * 512
   so that we have a refresh rate of 80 hz for no flicker.

   so:
   GCLK freq: 80 * 512 hz
   GCLK duty: 50%
   GCLK pin: GPIO_13

   BLNK freq: 80 hz
   BLNK duty: 50%
   BLNK pin: GPIO_12


*/
void init_pwm_channels(void) {
  // configure the two LED PWM channels
  ledc_channel_config_t gclk_config = {0}, blnk_config = {0};
  gclk_config.gpio_num = GCLK;
  gclk_config.speed_mode = LEDC_HIGH_SPEED_MODE;
  gclk_config.channel = GCLK_CHANNEL;
  gclk_config.intr_type = LEDC_INTR_DISABLE;
  gclk_config.timer_sel = GCLK_TIMER;
  gclk_config.duty = 512;

  blnk_config.gpio_num = BLNK;
  blnk_config.speed_mode = LEDC_HIGH_SPEED_MODE;
  blnk_config.channel = BLNK_CHANNEL;
  blnk_config.intr_type = LEDC_INTR_DISABLE;
  blnk_config.timer_sel = BLNK_TIMER;
  blnk_config.duty = 512;

  // configure the two timers for use with the channels
  ledc_timer_config_t gclk_timer_config = {0}, blnk_timer_config = {0};
  gclk_timer_config.speed_mode = LEDC_HIGH_SPEED_MODE;
  gclk_timer_config.bit_num = GCLK_RES;
  gclk_timer_config.timer_num = GCLK_TIMER;
  gclk_timer_config.freq_hz = 80 * 512;

  blnk_timer_config.speed_mode = LEDC_HIGH_SPEED_MODE;
  blnk_timer_config.bit_num = BLNK_RES;
  blnk_timer_config.timer_num = BLNK_TIMER;
  blnk_timer_config.freq_hz = 80;

  // Set the configurations values
  ESP_ERROR_CHECK(ledc_channel_config(&gclk_config));
  ESP_ERROR_CHECK(ledc_channel_config(&blnk_config));
  ESP_ERROR_CHECK(ledc_timer_config(&gclk_timer_config));
  ESP_ERROR_CHECK(ledc_timer_config(&blnk_timer_config));
}

// Sets the same GS value for the first six LED channels (channels are in
// reverse order)
void setDimmer(int value) {
  for(int i = 10; i < 16; i++) {
    gsValues[i] = value;
  }
  setGrayscale();
}

// Shifts out gs values for all 16 channels and then immediately latches them
// because I don't want to screw with interrupts after BLANK. The update
// happens so infrequenlty relative to the refresh rate that I don't care
// about any momentary glitching
void setGrayscale(void) {
    for(uint8_t i = 0; i < 16; i++) {
      dimmerShift(gsValues[i]);
    }
    gpio_set_level(XLAT,  HIGH);
    gpio_set_level(XLAT, LOW);
}

// Shifts out a sixteen bit value to the TLC7940, MSB first
void dimmerShift(uint16_t value) {
  for(uint16_t m = 0x0800; m; m >>= 1) {
    gpio_set_level(SIN, ((value & m) ? 1 : 0));
    gpio_set_level(DSCLK, 1);
    gpio_set_level(DSCLK, 0);
  }
}
