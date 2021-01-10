/*
* newtest.c
*
* Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
*
*     1.  Redistributions of source code must retain the above copyright notice, this list of
*         conditions and the following disclaimer.
*     2.  Redistributions in binary form must reproduce the above copyright notice, this list
*         of conditions and the following disclaimer in the documentation and/or other materials
*         provided with the distribution.
*     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
*         or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


static char VERSION[] = "XX.YY.ZZ";

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
	0x00800000,  // red
	0x00804000,  // orange
	0x00808000,  // yellow
	0x00008000,  // green
	0x00008020,  // lightblue
	0x00000080,  // blue
	0x00400040,  // purple
	0x00800040,  // pink
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
}

static void setup_handlers(void) {
	struct sigaction sa;
	sa.sa_handler = ctrl_c_handler;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}


void parseargs(int argc, char **argv, ws2811_t *ws2811) {
	int index;
	int c;

	static struct option longopts[] = {
		{"help", no_argument, 0, 'h'},
		{"dma", required_argument, 0, 'd'},
		{"gpio", required_argument, 0, 'g'},
		{"invert", no_argument, 0, 'i'},
		{"strip", required_argument, 0, 's'},
		{"height", required_argument, 0, 'y'},
		{"width", required_argument, 0, 'x'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	while (1) {
		index = 0;
		c = getopt_long(argc, argv, "cd:g:his:vx:y:", longopts, &index);

		if (c == -1)
			break;

		switch (c)
		{
		case 0:
			/* handle flag options (array's 3rd field non-0) */
			break;

		case 'h':
			fprintf(stderr, "%s version %s\n", argv[0], VERSION);
			fprintf(stderr, "Usage: %s \n"
				"-h (--help)    - this information\n"
				"-s (--strip)   - strip type - rgb, grb, gbr, rgbw\n"
				"-d (--dma)     - dma channel to use (default 10)\n"
				"-i (--invert)  - invert pin output (pulse LOW)\n"
				"-v (--version) - version information\n"
				, argv[0]);
			exit(-1);

		case 'D':
			break;

		case 'i':
			ws2811->channel[0].invert=1;
			break;

		case 'd':
			if (optarg) {
				int dma = atoi(optarg);
				if (dma < 14) {
					ws2811->dmanum = dma;
				} else {
					printf ("invalid dma %d\n", dma);
					exit (-1);
				}
			}
			break;

		case 's':
			if (optarg) {
				if (!strncasecmp("rgb", optarg, 4)) {
					ws2811->channel[0].strip_type = WS2811_STRIP_RGB;
				}
				else if (!strncasecmp("rbg", optarg, 4)) {
					ws2811->channel[0].strip_type = WS2811_STRIP_RBG;
				}
				else if (!strncasecmp("grb", optarg, 4)) {
					ws2811->channel[0].strip_type = WS2811_STRIP_GRB;
				}
				else if (!strncasecmp("gbr", optarg, 4)) {
					ws2811->channel[0].strip_type = WS2811_STRIP_GBR;
				}
				else if (!strncasecmp("brg", optarg, 4)) {
					ws2811->channel[0].strip_type = WS2811_STRIP_BRG;
				}
				else if (!strncasecmp("bgr", optarg, 4)) {
					ws2811->channel[0].strip_type = WS2811_STRIP_BGR;
				}
				else if (!strncasecmp("rgbw", optarg, 4)) {
					ws2811->channel[0].strip_type = SK6812_STRIP_RGBW;
				}
				else if (!strncasecmp("grbw", optarg, 4)) {
					ws2811->channel[0].strip_type = SK6812_STRIP_GRBW;
				}
				else {
					printf ("invalid strip %s\n", optarg);
					exit (-1);
				}
			}
			break;

		case 'v':
			fprintf(stderr, "%s version %s\n", argv[0], VERSION);
			exit(-1);

		case '?':
			/* getopt_long already reported error? */
			exit(-1);

		default:
			exit(-1);
		}
	}
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

	sprintf(VERSION, "%d.%d.%d", 1, 1, 0);

	parseargs(argc, argv, &ledstring);

	matrix = (ws2811_led_t *) malloc(sizeof(ws2811_led_t) * width * height);

	setup_handlers();

	if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
	{
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		return ret;
	}

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
