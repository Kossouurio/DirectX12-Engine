#include "pch.h"
#include "SystemHUD.h"

void SystemHUD::Render() {
    // Obtenir la résolution de l'écran
	int screenWidth = GameEngine::GetInstance().GetWindow()->mWindowWidth;
	int screenHeight = GameEngine::GetInstance().GetWindow()->mWindowHeight;

    // Position centrale
    int crosshairX = screenWidth / 2;
    int crosshairY = screenHeight / 2;


}
