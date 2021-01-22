#pragma once

#include "ws2811/clk.h"
#include "ws2811/gpio.h"
#include "ws2811/dma.h"
#include "ws2811/pwm.h"
#include "ws2811/ws2811.h"


namespace COLORS {
	/**
	 * @brief FIRST Colors
	 */
	constexpr ws2811_led_t	DENIM		= 0x001560BD;
	constexpr ws2811_led_t	FIRST_BLUE	= 0x000066B3;
	constexpr ws2811_led_t	FIRST_RED	= 0x00ED1C24;
	

	/**
	 * @brief Standard Colors
	 */
	constexpr ws2811_led_t	RED			= 0x00FF0000;
	constexpr ws2811_led_t	ORANGE		= 0x00FF8000;
	constexpr ws2811_led_t	YELLOW		= 0x00FFFF00;
	constexpr ws2811_led_t	GREEN		= 0x0000FF00;
	constexpr ws2811_led_t	LIGHT_BLUE	= 0x0000FF40;
	constexpr ws2811_led_t	BLUE		= 0x000000FF;
	constexpr ws2811_led_t	PURPLE		= 0x00800080;
	constexpr ws2811_led_t	PINK		= 0x00FF0080;
	constexpr ws2811_led_t	BLACK		= 0x00000000;
}


class ARGB {
 private:
	// Count of LEDs on the string
	uint8_t ledCount;

	// Library object to use the LEDs
	ws2811_t ledString;

 public:
	/**
	 * @brief Construct a new ARGB object
	 */
	ARGB(uint8_t ledCount);

	/**
	 * @brief Destroy the ARGB object
	 */
	~ARGB();

	/**
	 * @brief Set the color of the LED strip
	 * 
	 * @param color ws2811_led_t to set the color to
	 */
	void SetAll(ws2811_led_t color);

	/**
	 * @brief Set the color of an individual LED
	 * 
	 * @param index index of the LED to set
	 * @param color ws2811_led_t to set the color to
	 */
	void Set(uint8_t index, ws2811_led_t color);

	/**
	 * @brief Set the Brightness
	 * 
	 * @param brightness 0 to 255
	 */
	void SetBrightness(uint8_t brightness);
	
	/**
	 * @brief Render the changes
	 */
	void Render();
};
