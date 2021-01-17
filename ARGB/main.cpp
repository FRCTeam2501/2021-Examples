#include <iostream>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>


#include "ws2811/clk.h"
#include "ws2811/gpio.h"
#include "ws2811/dma.h"
#include "ws2811/pwm.h"

#include "ws2811/ws2811.h"


constexpr uint8_t LED_COUNT = 9;

ws2811_t ledstring;

static bool running = true;

void matrix_raise() {
	for (int x = 0; x < LED_COUNT; x++) {
		ledstring.channel[0].leds[x] = ledstring.channel[0].leds[LED_COUNT + LED_COUNT - x - 1];
	}
}

void matrix_clear() {
	for (int x = 0; x < LED_COUNT; x++) {
		ledstring.channel[0].leds[x] = 0;
	}
}

int dotspos[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
ws2811_led_t dotcolors[] = {
	0x00ff0000,  // red
	0x00ff8000,  // orange
	0x00ffff00,  // yellow
	0x0000ff00,  // green
	0x0000ff40,  // lightblue
	0x000000ff,  // blue
	0x00800080,  // purple
	0x00ff0080,  // pink
};

void matrix_bottom() {
	for (int i = 0; i < 8; i++) {
		dotspos[i]++;
		if (dotspos[i] > (LED_COUNT - 1)) {
			dotspos[i] = 0;
		}

		ledstring.channel[0].leds[dotspos[i]] = dotcolors[i];
	}
}

static void ctrl_c_handler(int signum) {
	(void) (signum);
	running = false;
	std::cout << "\nStopped\n";
}

static void setup_handlers(void) {
	struct sigaction sa;
	sa.sa_handler = ctrl_c_handler;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}


int main(int argc, char *argv[]) {
	ledstring.freq = WS2811_TARGET_FREQ;
	ledstring.dmanum = 10;
	ledstring.channel[0].gpionum = 21;
	ledstring.channel[0].count = LED_COUNT;
	ledstring.channel[0].invert = 0;
	ledstring.channel[0].brightness = 32;
	ledstring.channel[0].strip_type = WS2811_STRIP_GRB;
	ledstring.channel[1].gpionum = 0;
	ledstring.channel[1].count = 0;
	ledstring.channel[1].invert = 0;
	ledstring.channel[1].brightness = 0;

	ws2811_return_t ret;

	setup_handlers();

	if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		return ret;
	}
	std::cout << "Started\n";

	while (running) {
		matrix_raise();
		matrix_bottom();

		if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
		{
			fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
			break;
		}

		// 15 frames /sec
		usleep(1000000 / 15);
	}

	matrix_clear();
	ws2811_render(&ledstring);

	ws2811_fini(&ledstring);

	printf ("\n");
	return ret;
}
