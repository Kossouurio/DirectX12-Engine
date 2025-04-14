#include "pch.h"
#include "Input.h"

std::map<int, bool> Input::keyState;
std::map<int, bool> Input::prevKeyState;

int Input::lastMouseX = 0;
int Input::lastMouseY = 0;

int Input::rawDeltaX = 0;
int Input::rawDeltaY = 0;


bool Input::ignoreNextMouseUpdate = false;



void Input::SetKeyPressed(int key, bool pressed) {
    prevKeyState[key] = keyState[key];
    keyState[key] = pressed;

	if (debugMode && logSystemInput) {
        static std::map<int, std::string> keyNames = {
        {37, "LEFT"},
        {39, "RIGHT"},
        {38, "UP"},
        {40, "DOWN"},

        {32, "SPACE"},
        {17, "CTRL"},

        {90, "Z"},
        {81, "Q"},
        {83, "S"},
        {68, "D"}
        };

        std::string keyStr = keyNames.count(key) ? keyNames[key] : "UNKNOWN";

      //  HEAD("INPUT") << "Key [" << keyStr.c_str() << "] set to " << (pressed ? "PRESSED" : "RELEASED") << ENDL;

	}

    
}

bool Input::IsKeyPressed(int key) {
    return keyState[key];
}

bool Input::IsKeyReleased(int key) {
    return prevKeyState[key] && !keyState[key];
}

void Input::UpdateMousePosition(int x, int y) {
    static bool firstUpdate = true;
    static int prevX = x, prevY = y;

    if (firstUpdate) {
        prevX = x;
        prevY = y;
        firstUpdate = false;
    }

    lastMouseX = x;
    lastMouseY = y;
}

float Input::GetMouseDeltaX() {
    static float smoothDeltaX = 0.0f;
    float alpha = 0.3f;

    float deltaX = rawDeltaX;
    rawDeltaX = 0;

    smoothDeltaX = (alpha * deltaX) + ((1 - alpha) * smoothDeltaX);
    return smoothDeltaX;
}

float Input::GetMouseDeltaY() {
    static float smoothDeltaY = 0.0f;
    float alpha = 0.3f;

    float deltaY = rawDeltaY;
    rawDeltaY = 0;

    smoothDeltaY = (alpha * deltaY) + ((1 - alpha) * smoothDeltaY);
    return smoothDeltaY;
}


void Input::ProcessRawInput(LPARAM lParam) {
    RAWINPUT raw;
    UINT rawSize = sizeof(RAWINPUT);

    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));

    if (raw.header.dwType == RIM_TYPEMOUSE) {
        rawDeltaX = raw.data.mouse.lLastX;
        rawDeltaY = raw.data.mouse.lLastY;
    }
}



void Input::EnableRawMouse(HWND hwnd) {
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;  // Souris
    rid.usUsage = 0x02;      // Device specifique : souris
    rid.dwFlags = RIDEV_INPUTSINK;  // Capture en arriere-plan
    rid.hwndTarget = hwnd;

    RegisterRawInputDevices(&rid, 1, sizeof(rid));
}




void Input::CheckInput() {
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        Input::SetKeyPressed(KEY_UP, true);
    }
    else {
        Input::SetKeyPressed(KEY_UP, false);
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        Input::SetKeyPressed(KEY_DOWN, true);
    }
    else {
        Input::SetKeyPressed(KEY_DOWN, false);
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        Input::SetKeyPressed(KEY_LEFT, true);
    }
    else {
        Input::SetKeyPressed(KEY_LEFT, false);
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        Input::SetKeyPressed(KEY_RIGHT, true);
    }
    else {
        Input::SetKeyPressed(KEY_RIGHT, false);
    }


    if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        Input::SetKeyPressed(KEY_CTRL, true);
    }
    else {
        Input::SetKeyPressed(KEY_CTRL, false);
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        Input::SetKeyPressed(KEY_SPACE, true);
    }
    else {
        Input::SetKeyPressed(KEY_SPACE, false);
    }


    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        Input::SetKeyPressed(RMB, true);
    }
    else {
        Input::SetKeyPressed(RMB, false);
    }

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		Input::SetKeyPressed(LMB, true);
	}
	else {
		Input::SetKeyPressed(LMB, false);
	}


    if (GetAsyncKeyState(0x5A) & 0x8000) {
        Input::SetKeyPressed(KEY_Z, true);
    }
    else {
        Input::SetKeyPressed(KEY_Z, false);
    }

    if (GetAsyncKeyState(0x51) & 0x8000) {
        Input::SetKeyPressed(KEY_Q, true);
    }
    else {
        Input::SetKeyPressed(KEY_Q, false);
    }

    if (GetAsyncKeyState(0x53) & 0x8000) {
        Input::SetKeyPressed(KEY_S, true);
    }
    else {
        Input::SetKeyPressed(KEY_S, false);
    }

    if (GetAsyncKeyState(0x44) & 0x8000) {
        Input::SetKeyPressed(KEY_D, true);
    }
    else {
        Input::SetKeyPressed(KEY_D, false);
    }
	if (GetAsyncKeyState(0x45) & 0x8000) {
		Input::SetKeyPressed(KEY_E, true);
	}
	else {
		Input::SetKeyPressed(KEY_E, false);
	}
	if (GetAsyncKeyState(0x50) & 0x8000) {
		Input::SetKeyPressed(KEY_P, true);
	}
	else {
		Input::SetKeyPressed(KEY_P, false);
	}
  
}

void Input::ResetInput() {
    Input::SetKeyPressed(KEY_LEFT, false);
    Input::SetKeyPressed(KEY_RIGHT, false);
    Input::SetKeyPressed(KEY_UP, false);
    Input::SetKeyPressed(KEY_DOWN, false);

    Input::SetKeyPressed(KEY_SPACE, false);
    Input::SetKeyPressed(KEY_CTRL, false);

	Input::SetKeyPressed(RMB, false);
	Input::SetKeyPressed(LMB, false);

    Input::SetKeyPressed(KEY_Z, false);
    Input::SetKeyPressed(KEY_Q, false);
    Input::SetKeyPressed(KEY_S, false);
    Input::SetKeyPressed(KEY_D, false);
    Input::SetKeyPressed(KEY_E, false);
    Input::SetKeyPressed(KEY_P, false);
}
