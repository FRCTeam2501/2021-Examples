#include <iostream>

#include "ARGB.h"


ARGB::ARGB() {
	ledstring.freq = WS2811_TARGET_FREQ;
	ledstring.dmanum = 10;
	ledstring.channel[0].gpionum = 21;
	ledstring.channel[0].count = LED_COUNT;
	ledstring.channel[0].invert = 0;
	ledstring.channel[0].brightness = 255;
	ledstring.channel[0].strip_type = WS2811_STRIP_GRB;
	ledstring.channel[1].gpionum = 0;
	ledstring.channel[1].count = 0;
	ledstring.channel[1].invert = 0;
	ledstring.channel[1].brightness = 0;

	ws2811_return_t ret = ws2811_init(&ledstring);

	if (ret != WS2811_SUCCESS) {
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		exit(ret);
	}
}

ARGB::~ARGB() {
	for (int x = 0; x < LED_COUNT; x++) {
		ledstring.channel[0].leds[x] = 0;
	}

	ws2811_render(&ledstring);

	ws2811_fini(&ledstring);
}

void ARGB::Set(ws2811_led_t color) {
	for (int x = 0; x < LED_COUNT; x++) {
		ledstring.channel[0].leds[x] = color;
	}

	ws2811_return_t ret = ws2811_render(&ledstring);
	if (ret != WS2811_SUCCESS) {
		fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
		exit(ret);
	}
}

void ARGB::SetBrightness(uint8_t brightness) {
	ledstring.channel[0].brightness = brightness;
}
