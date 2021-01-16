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
	int fd = -1;
	bool *btns;
	uint8_t btnCount;
	int16_t *axes;
	uint8_t axisCount;

	void init(std::string deviceID);

 public:
	Joystick(std::string deviceID);

	Joystick(uint8_t deviceID);

	Joystick();

	bool IsOpen();

	bool Update();

	uint8_t GetButtonCount();

	bool GetButton(uint8_t button);

	uint8_t GetAxisCount();

	int16_t GetRawAxis(uint8_t axis);

	double GetAxis(uint8_t axis);
};
