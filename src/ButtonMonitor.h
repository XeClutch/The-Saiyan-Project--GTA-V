#pragma once

//// Includes
#include "stdafx.h"
#include <xinputdefs.h>

//// Classes
class ButtonMonitor
{
private:
	/// Variables
	XINPUT_STATE ctrl;
	bool BACK_B;
	bool BUMPER_LEFT_B;
	bool BUMPER_RIGHT_B;
	bool DPAD_DOWN_B;
	bool DPAD_LEFT_B;
	bool DPAD_RIGHT_B;
	bool DPAD_UP_B;
	bool GUIDE_B;
	bool MAIN_A_B;
	bool MAIN_B_B;
	bool MAIN_X_B;
	bool MAIN_Y_B;
	bool START_B;
	bool THUMBSTICK_LEFT_B;
	bool THUMBSTICK_RIGHT_B;
	bool TRIGGER_LEFT_B;
	bool TRIGGER_RIGHT_B;

public:
	/// Variables
	bool BACK;
	bool BACK_JP;
	bool BACK_JR;
	bool BUMPER_LEFT;
	bool BUMPER_LEFT_JP;
	bool BUMPER_LEFT_JR;
	bool BUMPER_RIGHT;
	bool BUMPER_RIGHT_JP;
	bool BUMPER_RIGHT_JR;
	bool DPAD_DOWN;
	bool DPAD_DOWN_JP;
	bool DPAD_DOWN_JR;
	bool DPAD_LEFT;
	bool DPAD_LEFT_JP;
	bool DPAD_LEFT_JR;
	bool DPAD_RIGHT;
	bool DPAD_RIGHT_JP;
	bool DPAD_RIGHT_JR;
	bool DPAD_UP;
	bool DPAD_UP_JP;
	bool DPAD_UP_JR;
	bool GUIDE;
	bool GUIDE_JP;
	bool GUIDE_JR;
	bool MAIN_A;
	bool MAIN_A_JP;
	bool MAIN_A_JR;
	bool MAIN_B;
	bool MAIN_B_JP;
	bool MAIN_B_JR;
	bool MAIN_X;
	bool MAIN_X_JP;
	bool MAIN_X_JR;
	bool MAIN_Y;
	bool MAIN_Y_JP;
	bool MAIN_Y_JR;
	bool START;
	bool START_JP;
	bool START_JR;
	bool THUMBSTICK_LEFT;
	bool THUMBSTICK_LEFT_JP;
	bool THUMBSTICK_LEFT_JR;
	bool THUMBSTICK_RIGHT;
	bool THUMBSTICK_RIGHT_JP;
	bool THUMBSTICK_RIGHT_JR;
	bool TRIGGER_LEFT;
	bool TRIGGER_LEFT_JP;
	bool TRIGGER_LEFT_JR;
	bool TRIGGER_RIGHT;
	bool TRIGGER_RIGHT_JP;
	bool TRIGGER_RIGHT_JR;

	/// Methods
	void ButtonMonitor::Refresh(DWORD Controller = 0)
	{
		if (!XInputGetState(Controller, &ctrl))
		{
			BACK = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) > 0);
			BACK_JP = (BACK && !BACK_B);
			BACK_JR = (!BACK && BACK_B);
			BACK_B = BACK;
			BUMPER_LEFT = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0);
			BUMPER_LEFT_JP = (BUMPER_LEFT && !BUMPER_LEFT_B);
			BUMPER_LEFT_JR = (!BUMPER_LEFT && BUMPER_LEFT_B);
			BUMPER_LEFT_B = BUMPER_LEFT;
			BUMPER_RIGHT = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0);
			BUMPER_RIGHT_JP = (BUMPER_RIGHT && !BUMPER_RIGHT_B);
			BUMPER_RIGHT_JR = (!BUMPER_RIGHT && BUMPER_RIGHT_B);
			BUMPER_RIGHT_B = BUMPER_RIGHT;
			DPAD_DOWN = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) > 0);
			DPAD_DOWN_JP = (DPAD_DOWN && !DPAD_DOWN_B);
			DPAD_DOWN_JR = (!DPAD_DOWN && DPAD_DOWN_B);
			DPAD_DOWN_B = DPAD_DOWN;
			DPAD_LEFT = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) > 0);
			DPAD_LEFT_JP = (DPAD_LEFT && !DPAD_LEFT_B);
			DPAD_LEFT_JR = (!DPAD_LEFT && DPAD_LEFT_B);
			DPAD_LEFT_B = DPAD_LEFT;
			DPAD_RIGHT = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) > 0);
			DPAD_RIGHT_JP = (DPAD_RIGHT && !DPAD_RIGHT_B);
			DPAD_RIGHT_JR = (!DPAD_RIGHT && DPAD_RIGHT_B);
			DPAD_RIGHT_B = DPAD_RIGHT;
			DPAD_UP = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) > 0);
			DPAD_UP_JP = (DPAD_UP && !DPAD_UP_B);
			DPAD_UP_JR = (!DPAD_UP && DPAD_UP_B);
			DPAD_UP_B = DPAD_UP;
			GUIDE = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_BIGBUTTON) > 0);
			GUIDE_JP = (GUIDE && !GUIDE_B);
			GUIDE_JR = (!GUIDE && GUIDE_B);
			GUIDE_B = GUIDE;
			MAIN_A = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_A) > 0);
			MAIN_A_JP = (MAIN_A && !MAIN_A_B);
			MAIN_A_JR = (!MAIN_A && MAIN_A_B);
			MAIN_A_B = MAIN_A;
			MAIN_B = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_B) > 0);
			MAIN_B_JP = (MAIN_B && !MAIN_B_B);
			MAIN_B_JR = (!MAIN_B && MAIN_B_B);
			MAIN_B_B = MAIN_B;
			MAIN_X = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_X) > 0);
			MAIN_X_JP = (MAIN_X && !MAIN_X_B);
			MAIN_X_JR = (!MAIN_X && MAIN_X_B);
			MAIN_X_B = MAIN_X;
			MAIN_Y = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_Y) > 0);
			MAIN_Y_JP = (MAIN_Y && !MAIN_Y_B);
			MAIN_Y_JR = (!MAIN_Y && MAIN_Y_B);
			MAIN_Y_B = MAIN_Y;
			START = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_START) > 0);
			START_JP = (START && !START_B);
			START_JP = (!START && START_B);
			START_B = START;
			THUMBSTICK_LEFT = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) > 0);
			THUMBSTICK_LEFT_JP = (THUMBSTICK_LEFT && !THUMBSTICK_LEFT_B);
			THUMBSTICK_LEFT_JR = (!THUMBSTICK_LEFT && THUMBSTICK_LEFT_B);
			THUMBSTICK_LEFT_B = THUMBSTICK_LEFT;
			THUMBSTICK_RIGHT = ((ctrl.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) > 0);
			THUMBSTICK_RIGHT_JP = (THUMBSTICK_RIGHT && !THUMBSTICK_RIGHT_B);
			THUMBSTICK_RIGHT_JR = (!THUMBSTICK_RIGHT && THUMBSTICK_RIGHT_B);
			THUMBSTICK_RIGHT_B = THUMBSTICK_RIGHT;
			TRIGGER_LEFT = ctrl.Gamepad.bLeftTrigger;
			TRIGGER_LEFT_JP = (TRIGGER_LEFT && !TRIGGER_LEFT_B);
			TRIGGER_LEFT_JR = (!TRIGGER_LEFT && TRIGGER_LEFT_B);
			TRIGGER_LEFT_B = TRIGGER_LEFT;
			TRIGGER_RIGHT = ctrl.Gamepad.bRightTrigger;
			TRIGGER_RIGHT_JP = (TRIGGER_RIGHT && !TRIGGER_RIGHT_B);
			TRIGGER_RIGHT_JR = (!TRIGGER_RIGHT && TRIGGER_RIGHT_B);
			TRIGGER_RIGHT_B = TRIGGER_RIGHT;
		}
	}
};