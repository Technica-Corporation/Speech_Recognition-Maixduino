#include <Arduino.h>

#define NUMPIX 12

uint8_t red[NUMPIX], green[NUMPIX], blue[NUMPIX];
uint8_t brightness = 0;

uint8_t data_pin=3;
uint8_t clock_pin=4;

void set_pin(uint8_t pin, uint8_t value)
{
    // set pin to value
    if (value==0) {
        gpio_set_pin(pin, GPIO_PV_LOW);
    } else {
        gpio_set_pin(pin, GPIO_PV_HIGH);
    }
}

void transfer(uint8_t data)
{
    for (int bit=7; bit>=0; bit--) {
        // set the data on the data pin
        set_pin(data_pin, data >> bit & 1);
        // give a clock pulse on the clock pin
        set_pin(clock_pin, 1);
        set_pin(clock_pin, 0);
    }
}

// adapted from https://github.com/pololu/apa102-arduino/blob/master/APA102.h
void set_low(void)
{
    // set the data pin low
    set_pin(data_pin, 0);
    // set the clock pin low
    set_pin(clock_pin, 0);
}

void start_frame(void)
{
    // start with both the data and clock pin values low
    set_low();
    // the SK9822 first expects 4 times 0000 0000 
    transfer(0);
    transfer(0);
    transfer(0);
    transfer(0);
}

void send_color(uint8_t pix_num)
{
    // set the overall brightness factor
    transfer(0b11100000 | brightness);
    // send the RGB values
    transfer(blue[pix_num]);
    transfer(green[pix_num]);
    transfer(red[pix_num]);
}

void end_frame(void)
{
    // the SK9822 finally expects 4 times 1111 1111
    for (uint8_t i = 0; i < 4; i++) transfer(255);
}

void write_pixels()
{
    // write the color data to the LEDs
    start_frame();
    for (uint8_t i=0; i<NUMPIX; i++) {
        send_color(i);
    }
    end_frame();
}

void lights_out(int brightness, int data_pin, int clock_pin, int pin1, int pin2) {
  gpio_set_drive_mode(data_pin, GPIO_DM_OUTPUT); 
  gpio_set_drive_mode(clock_pin, GPIO_DM_OUTPUT); 

  fpioa_set_function(pin1, FUNC_GPIO3);
  fpioa_set_function(pin2, FUNC_GPIO4);

  for (uint8_t i=0; i<NUMPIX; i++) {
    red[i] = 0;
    green[i] = 0;
    blue[i] = 0;
  }
  write_pixels();
  //usleep(10000);
}