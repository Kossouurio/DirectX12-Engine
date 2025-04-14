#include "pch.h"
#include "Logs.h"


bool MouseCamera = true;

bool debugMode = false;
bool debugMove = false;

bool logFrame = false;
bool logBuffer = false;

bool logComponent = true;

bool logSystemManager = true;
bool logSystemTransform = false;
bool logSystemPhysics = true;
bool logSystemRender = false;
bool logSystemInput = false;
bool logSystemCollider = false;
bool logSystemHealth = false;
bool logSystemCamera = false;
bool logSystemJump = false;
bool logSystemParticle = false;


Logs* Logs::Get() {
	static Logs log = Logs();
	return &log;
}

void Logs::Init()
{
	InitializeCriticalSection(&mCriticalSection);
	mStream = std::ofstream("logs.txt");
	mIsInit = true;
}

void Logs::UnInit()
{
	mStream.close();
	DeleteCriticalSection(&mCriticalSection);
	mIsInit = false;
}

Logs& Logs::Print(const char* header, const char* message)
{
	if (debugMode) {
		if (!mIsInit)
			return *this;

		int vSizeHeader = strlen(header);
		int vSizeMessage = 0;
		if (message != "") {
			vSizeMessage = strlen(message);
		}

		char* mess = new char[6 + vSizeHeader + vSizeMessage];

		mess[0] = '[';
		for (int i = 0; i < vSizeHeader; i++) {
			mess[1 + i] = header[i];
		}
		mess[1 + vSizeHeader] = ']';
		mess[1 + vSizeHeader + 1] = ' ';
		mess[1 + vSizeHeader + 2] = ':';
		mess[1 + vSizeHeader + 3] = ' ';
		mess[1 + vSizeHeader + 4] = '\0';

		for (int i = 0; i < vSizeMessage; i++) {
			mess[1 + vSizeHeader + 4 + i] = message[i];
		}

		Print(mess);

		delete[] mess;

		return *this;
	}
}


void Logs::Print(const char* message)
{
	if (debugMode) {
		if (!mIsInit)
			return;
		int vSizeMessage = ToolBox::StrLength(message);
		if (mCriticalSection.LockCount == 0)
			EnterCriticalSection(&mCriticalSection);
		std::cout << message;
		OutputDebugStringA(message);
		mStream.write(message, vSizeMessage);
	}
}

Logs& Logs::operator<<(const char* _other)
{
	Print(_other);
	return *this;
}

Logs& Logs::operator<<(int _other)
{
	Print(std::to_string(_other).c_str());
	return *this;
}

Logs& Logs::operator<<(float _other)
{
	Print(std::to_string(_other).c_str());
	return *this;
}



