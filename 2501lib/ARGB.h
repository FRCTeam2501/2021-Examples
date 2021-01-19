#pragma once

#include "ws2811/clk.h"
#include "ws2811/gpio.h"
#include "ws2811/dma.h"
#include "ws2811/pwm.h"
#include "ws2811/ws2811.h"


namespace COLORS {
	constexpr ws2811_led_t RED = 0x00ff0000;
	constexpr ws2811_led_t ORANGE = 0x00ff8000;
	constexpr ws2811_led_t YELLOW = 0x00ffff00;
	constexpr ws2811_led_t GREEN = 0x0000ff00;
	constexpr ws2811_led_t LIGHT_BLUE = 0x0000ff40;
	constexpr ws2811_led_t BLUE = 0x000000ff;
	constexpr ws2811_led_t PURPLE = 0x00800080;
	constexpr ws2811_led_t PINK = 0x00ff0080;
}


class ARGB {
 private:
	// Count of LEDs on the string
	const static uint8_t LED_COUNT = 9;

	ws2811_t ledstring;

 public:
	/**
	 * @brief Construct a new ARGB object
	 */
	ARGB();

	/**
	 * @brief Destroy the ARGB object
	 */
	~ARGB();

	/**
	 * @brief Set the color of the ARGB object
	 * 
	 * @param color ws2811_led_t to set the color to
	 */
	void Set(ws2811_led_t color);

	/**
	 * @brief Set the Brightness
	 * 
	 * @param brightness 0 to 255
	 */
	void SetBrightness(uint8_t brightness);
};
