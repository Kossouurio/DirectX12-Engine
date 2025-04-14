#include "pch.h"
#include "Input.h"

std::map<int, bool> Input::keyState;
std::map<int, bool> Input::prevKeyState;

int Input::lastMouseX = 0;
int Input::lastMouseY = 0;


void Input::SetKeyPressed(int key, bool pressed) {
    prevKeyState[key] = keyState[key];
    keyState[key] = pressed;

    static std::map<int, std::string> keyNames = {
        {37, "LEFT"},
        {39, "RIGHT"},
        {32, "SPACE"},
        {38, "UP"},
        {40, "DOWN"},
        {87, "W"},
        {65, "A"},
        {83, "S"},
        {68, "D"}
    };

    std::string keyStr = keyNames.count(key) ? keyNames[key] : "UNKNOWN";

    HEAD("INPUT") << "Key [" << keyStr.c_str() << "] set to " << (pressed ? "PRESSED" : "RELEASED") << ENDL;

}

bool Input::IsKeyPressed(int key) {
    return keyState[key];
}

bool Input::IsKeyReleased(int key) {
    return prevKeyState[key] && !keyState[key];
}


void Input::UpdateMousePosition(int x, int y) {
    lastMouseX = x;
    lastMouseY = y;
}

float Input::GetMouseDeltaX() {
    static int prevX = lastMouseX;
    int deltaX = lastMouseX - prevX;
    prevX = lastMouseX;
    return static_cast<float>(deltaX);
}

float Input::GetMouseDeltaY() {
    static int prevY = lastMouseY;
    int deltaY = lastMouseY - prevY;
    prevY = lastMouseY;
    return static_cast<float>(deltaY);
}
