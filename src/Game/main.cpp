//#include "pch.h"
////#include "GameEngine.h"
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
//    GameEngine engine;
//    engine.Init(hInstance);
//    engine.Run();
//    engine.CleanUp();
//    return 0;
//}
//




//VRAIE VERSION 

#include "pch.h"
//#include "GameEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    GameEngine::GetInstance().Init(hInstance);
    GameEngine::GetInstance().Run();
    GameEngine::GetInstance().CleanUp();
    return 0;
}