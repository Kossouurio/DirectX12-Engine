#pragma once
//#include "UtilsImport.h"
//#include <unordered_map> //a virer
//#include <Windows.h>

class Input {
private:
    static std::map<int, bool> keyState;
    static std::map<int, bool> prevKeyState; 
    static int lastMouseX;
    static int lastMouseY;

	static int rawDeltaX;
	static int rawDeltaY;

    static bool ignoreNextMouseUpdate;

public:
    static void SetKeyPressed(int key, bool pressed);
    static bool IsKeyPressed(int key);
    static bool IsKeyReleased(int key); 

    static float GetMouseDeltaX();
    static float GetMouseDeltaY();
	static void ProcessRawInput(LPARAM lParam);
	static void EnableRawMouse(HWND hwnd);
    static void UpdateMousePosition(int x, int y);

	static void CheckInput();
    static void ResetInput();
};
 