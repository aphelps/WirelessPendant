/*******************************************************************************
 * Wireless pendants
 */

#define DEBUG_LEVEL DEBUG_HIGH
#include "Debug.h"

#include "EEPROM.h"
#include <RS485_non_blocking.h>
#include <SoftwareSerial.h>

#include "SPI.h"
#include "FastLED.h"

#include "GeneralUtils.h"
#include "EEPromUtils.h"
#include "HMTLTypes.h"
#include "PixelUtil.h"
#include "Wire.h"
#include "MPR121.h"
#include "SerialCLI.h"

#include "Socket.h"
#include "RS485Utils.h"
#include "XBee.h"
#include "XBeeSocket.h"

#include "HMTLProtocol.h"
#include "HMTLMessaging.h"
#include "HMTLPrograms.h"
#include "TimeSync.h"

/******/

config_hdr_t config;
output_hdr_t *outputs[HMTL_MAX_OUTPUTS];
config_hdr_t readconfig;
config_max_t readoutputs[HMTL_MAX_OUTPUTS];

config_pixels_t pixel_output;
config_value_t value_output;
config_rs485_t rs485_output;

boolean has_value = false;
boolean has_pixels = false;
boolean has_rs485 = false;

int configOffset = -1;

PixelUtil pixels;

RS485Socket rs485;
#define SEND_BUFFER_SIZE 64 // The data size for transmission buffers
byte rs485_data_buffer[RS485_BUFFER_TOTAL(SEND_BUFFER_SIZE)];

void setup() {
  Serial.begin(9600);

  DEBUG2_PRINTLN("***** HMTL Bringup *****");

  int32_t outputs_found = hmtl_setup(&config, readoutputs,
                                     outputs, NULL, HMTL_MAX_OUTPUTS,
                                     NULL,
                                     NULL,
                                     &pixels,
                                     NULL, // MPR121
                                     NULL,
                                     &value_output, // Value
                                     &configOffset);

  DEBUG4_VALUE("Config size:", configOffset - HMTL_CONFIG_ADDR);
  DEBUG4_VALUELN(" end:", configOffset);
  DEBUG4_COMMAND(hmtl_print_config(&config, outputs));

  if (outputs_found & (1 << HMTL_OUTPUT_VALUE)) {
    has_value = true;
  }

  if (outputs_found & (1 << HMTL_OUTPUT_PIXELS)) {
    for (unsigned int i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelRGB(i, 0, 0, 0);
    }
    pixels.update();
    has_pixels = true;

    FastLED.setBrightness(32); // XXX - Pixels?
  }
}

void setXY(const uint8_t x, const uint8_t y, const uint32_t color) {
  uint8_t led = (x % 4) + (y % 4) * 4;
  pixels.setPixelRGB(led, color);
}

#define PERIOD 100
unsigned long last_change = 0;
int cycle = 0;
void loop() {
  unsigned long now = millis();

  /*
   * Change the display mode periodically
   */
  if (now - last_change > PERIOD) {
    static uint8_t x = 0, y = 0;
    setXY(x, y, pixel_wheel(cycle));
    pixels.update();

    switch (random8() % 4) {
      case 0:
        x += 1;
        break;
      case 1:
        x -= 1;
        break;
      case 2:
        y += 1;
        break;
      case 3:
        y -= 1;
        break;
    }

    cycle++;
    last_change = now;
  }
}

