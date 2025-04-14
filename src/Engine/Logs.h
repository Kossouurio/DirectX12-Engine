#pragma once

//#include "UtilsImport.h"

#define PRINT(Header, Message) Logs::Get()->Print(Header, Message)
#define HEAD(Header) Logs::Get()->Print(Header, "")
#define OUTPUT *Logs::Get()
#define ENDL "\n";
#define ENDL2 "\n\n";
#define LINE "----------------------------------------------------------------------------------------------------------------------------"

extern bool MouseCamera;

extern bool debugMode;  
extern bool debugMove;
extern bool logFrame;
extern bool logBuffer;
extern bool logComponent;
extern bool logSystemManager;
extern bool logSystemTransform;
extern bool logSystemPhysics;
extern bool logSystemRender;
extern bool logSystemInput;
extern bool logSystemCollider;
extern bool logSystemHealth;
extern bool logSystemCamera;
extern bool logSystemJump;
extern bool logSystemParticle;


class Logs
{
private:
	std::ofstream mStream;

	bool mIsInit = false;
	CRITICAL_SECTION mCriticalSection;

	void Print(const char* message);
public:
	Logs() = default;
	virtual ~Logs() {};
	static Logs* Get();

	void Init();
	void UnInit();

	Logs& Print(const char* header, const char* message);

	Logs& operator<<(const char* _other);
	Logs& operator<<(float _other);
	Logs& operator<<(int _other);
};

