#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "ws2811/ws2811.h"



static bool running = true;

static void ctrl_c_handler(int signum) {
	(void) (signum);
	running = false;

	std::cout << "Got Ctrl+C\n";
}

static void setup_handlers(void) {
	struct sigaction sa;
	sa.sa_handler = ctrl_c_handler;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}


constexpr uint8_t LEN = 9;


int main() {
	ws2811_t ledstring;
	ledstring.freq = WS2811_TARGET_FREQ;
	ledstring.dmanum = 10;
	ledstring.channel[0].gpionum = 21;
	ledstring.channel[0].count = LEN;
	ledstring.channel[0].invert = 0;
	ledstring.channel[0].brightness = 255;
	ledstring.channel[0].strip_type = WS2811_STRIP_GBR;
	ledstring.channel[1].gpionum = 0;
	ledstring.channel[1].count = 0;
	ledstring.channel[1].invert = 0;
	ledstring.channel[1].brightness = 0;

	ws2811_return_t ret;
	if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		return ret;
	}

	setup_handlers();
	std::cout << "Setup\n";


	while(running) {
		std::cout << "1\n";
									// 0xWWRRGGBB
		ledstring.channel[0].leds[0] = 0x00000000;
		std::cout << "2\n";
		if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
			fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
			break;
		}
		std::cout << "3\n";
		// 15 frames /sec
		usleep(1000000U / 15U);
		std::cout << "4\n";
	}

	ledstring.channel[0].leds[0] = 0x00000000;
	ws2811_render(&ledstring);

	ws2811_fini(&ledstring);

	printf ("\n");
	return ret;
}