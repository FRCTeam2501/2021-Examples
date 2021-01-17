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


#define ARRAY_SIZE(stuff)       (sizeof(stuff) / sizeof(stuff[0]))

// defaults for cmdline options
#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                21
#define DMA                     10
//#define STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

#define WIDTH                   9
#define HEIGHT                  1
#define LED_COUNT               (WIDTH * HEIGHT)

int width = WIDTH;
int height = HEIGHT;
int led_count = LED_COUNT;

ws2811_t ledstring;

ws2811_led_t *matrix;

static uint8_t running = 1;

void matrix_render(void) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			ledstring.channel[0].leds[(y * width) + x] = matrix[y * width + x];
		}
	}
}

void matrix_raise(void) {
	for (int y = 0; y < (height - 1); y++) {
		for (int x = 0; x < width; x++) {
			// This is for the 8x8 Pimoroni Unicorn-HAT where the LEDS in subsequent
			// rows are arranged in opposite directions
			matrix[y * width + x] = matrix[(y + 1)*width + width - x - 1];
		}
	}
}

void matrix_clear(void) {
	for (int y = 0; y < (height); y++) {
		for (int x = 0; x < width; x++) {
			matrix[y * width + x] = 0;
		}
	}
}

int dotspos[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
ws2811_led_t dotcolors[] = {
	0x00400000,  // red
	0x00402000,  // orange
	0x00404000,  // yellow
	0x00004000,  // green
	0x00004010,  // lightblue
	0x00000040,  // blue
	0x00200020,  // purple
	0x00400020,  // pink
};

void matrix_bottom(void) {
	for (int i = 0; i < (int)(ARRAY_SIZE(dotspos)); i++) {
		dotspos[i]++;
		if (dotspos[i] > (width - 1)) {
			dotspos[i] = 0;
		}

		matrix[dotspos[i] + (height - 1) * width] = dotcolors[i];
	}
}

static void ctrl_c_handler(int signum) {
	(void) (signum);
	running = 0;
	std::cout << "\nStopped\n";
}

static void setup_handlers(void) {
	struct sigaction sa;
	sa.sa_handler = ctrl_c_handler;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}


int main(int argc, char *argv[]) {
	ledstring.freq = TARGET_FREQ;
	ledstring.dmanum = DMA;
	ledstring.channel[0].gpionum = GPIO_PIN;
	ledstring.channel[0].count = LED_COUNT;
	ledstring.channel[0].invert = 0;
	ledstring.channel[0].brightness = 255;
	ledstring.channel[0].strip_type = STRIP_TYPE;
	ledstring.channel[1].gpionum = 0;
	ledstring.channel[1].count = 0;
	ledstring.channel[1].invert = 0;
	ledstring.channel[1].brightness = 0;

	ws2811_return_t ret;

	matrix = (ws2811_led_t *) malloc(sizeof(ws2811_led_t) * width * height);

	setup_handlers();

	if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
	{
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		return ret;
	}
	std::cout << "Started\n";

	while (running)
	{
		matrix_raise();
		matrix_bottom();
		matrix_render();

		if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
		{
			fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
			break;
		}

		// 15 frames /sec
		usleep(1000000 / 15);
	}

	matrix_clear();
	matrix_render();
	ws2811_render(&ledstring);

	ws2811_fini(&ledstring);

	printf ("\n");
	return ret;
}
