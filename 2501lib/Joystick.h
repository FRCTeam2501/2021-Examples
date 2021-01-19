#pragma once


// Gamepad button & axis mapping
namespace GAMEPAD {
	namespace BUTTONS {
		constexpr uint8_t A = 0;
		constexpr uint8_t B = 1;
		constexpr uint8_t X = 3;
		constexpr uint8_t Y = 4;
		constexpr uint8_t LT = 6;
		constexpr uint8_t RT = 7;
		constexpr uint8_t LB = 8;
		constexpr uint8_t RB = 9;
		constexpr uint8_t SELECT = 10;
		constexpr uint8_t START = 11;
		constexpr uint8_t LA = 13;
		constexpr uint8_t RA = 14;
	}
	namespace AXES {
		constexpr uint8_t LX = 0;
		constexpr uint8_t LY = 1;
		constexpr uint8_t RX = 2;
		constexpr uint8_t RY = 3;
		constexpr uint8_t DX = 6;
		constexpr uint8_t DY = 7;

		// Note: while the bumpers can be read as axis, the library is inconsistent at reading their value.  Your best bet is to use them as buttons.
		constexpr uint8_t RB = 4;
		constexpr uint8_t LB = 5;
	}
}


class Joystick {
 private:
	// Linux file descriptor for the joystick
	int fd = -1;
	// Array of booleans to store the current read state of the buttons
	bool *btns;
	// Array of booleans to store the last read state of the buttons
	bool *lastBtns;
	// Number of buttons available on this joystick
	uint8_t btnCount;
	// Array of integers to store the current read state of the axes
	int16_t *axes;
	// Number of axes available on this joystick
	uint8_t axisCount;

	// Internal function used to open fd and initialize the joystick
	void init(std::string deviceID);

 public:
	/**
	 * @brief Construct a new Joystick object
	 * 
	 * @param deviceID Path to joystick, ex `/dev/input/js0'
	 */
	Joystick(std::string deviceID);

	/**
	 * @brief Construct a new Joystick object
	 * 
	 * @param deviceID Integer of joystick, ex `0` for `/dev/input/js0'
	 */
	Joystick(uint8_t deviceID);

	/**
	 * @brief Construct a new Joystick object with path `/dev/input/js0'
	 */
	Joystick();

	/**
	 * @brief Check if a joystick is correctly opened.
	 */
	bool IsOpen();

	/**
	 * @brief Update joystick
	 */
	void Update();

	/**
	 * @brief Get the Button Count
	 * 
	 * @return uint8_t Number of buttons available on the joystick
	 */
	uint8_t GetButtonCount();

	/**
	 * @brief Get the status of the button
	 * 
	 * @param button Number of button to check
	 */
	bool GetButton(uint8_t button);

	/**
	 * @brief Get if the button has changed since last loop
	 * 
	 * @param button Number of button to check
	 */
	bool HasButtonChanged(uint8_t button);

	/**
	 * @brief Get the Axis Count
	 * 
	 * @return uint8_t Number of axes available on the joystick
	 */
	uint8_t GetAxisCount();

	/**
	 * @brief Get the raw axis value
	 * 
	 * @param axis Number of axis to check
	 * @return int16_t Raw value of axis
	 */
	int16_t GetRawAxis(uint8_t axis);

	/**
	 * @brief Get the axis value
	 * 
	 * @param axis Number of axis to check
	 * @return double Value of axis
	 */
	double GetAxis(uint8_t axis);
};
