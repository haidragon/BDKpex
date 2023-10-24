// dllmain.cpp : Defines the entry point for the DLL application.
#include "includes.h"
#include "blyat.h"
DWORD KeyPressPollingRate = 144;
DWORD TickSleepTime = 100;

uint32_t OriginalAPCFlag = 0x0;

typedef struct LongAssStruct
{
	BYTE b[20];
};

DWORD WINAPI InternalThread(PVOID)
{
	Beep(500, 500);
	/*
	HWND consoleWnd = GetConsoleWindow();
	if (consoleWnd == NULL)
	{
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, skCrypt("CONIN$"), "r", stdin);
		freopen_s(&stream, skCrypt("CONOUT$"), "w", stdout);
		freopen_s(&stream, skCrypt("CONOUT$"), "w", stderr);
		consoleWnd = GetConsoleWindow();
		HMENU hmenu = GetSystemMenu(consoleWnd, FALSE);
		EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
	}
	*/
	int User = 0x0;
	if (!Verification::CheckUser(&User))
	{
		std::terminate();
		return false;
	}

	//std::cout << skCrypt("BDKPEX LEECH v0.5.1 BETA [ENCRYPTED]") << std::endl;
	
	LoadLibraryA(skCrypt("win32u.dll"));
	LoadLibraryA(skCrypt("user32.dll"));

	if (!initialization::init())
	{
		//std::cout << skCrypt("[CRITICAL ERROR] INITIALISATION FAILED") << std::endl; 
		BeepFail();
	}

	DWORD PID = 0x0; DWORD64 MODULEBASE = 0x0;
	Beep(500, 300);
	//std::cout << skCrypt("[MESSAGE] PLEASE ENTER LOBBY, PRESS [HOME] TO CONTINUE") << std::endl;

	//blyatcyka
	//BlyatFull
	PlaySoundA((LPCSTR)blyatcyka, NULL, SND_MEMORY | SND_ASYNC);
	OriginalAPCFlag = memory::ClearAPCFlag();

	while (!nt::GetKey(VK_HOME))
	{
		FeatureUtils::initDrawInfo();
		menu::DrawIntro(OriginalAPCFlag, User);
		FeatureUtils::callRender();
		if (nt::GetKey(VK_END))
		{
			memory::RestoreAPCFlag(OriginalAPCFlag);
			std::terminate();
		}
	}

	if (!process::init(skCrypt("r5apex.exe"), NULL, &MODULEBASE, &PID))
	{
		memory::RestoreAPCFlag(OriginalAPCFlag);
		BeepFail();
	}
	
	clock_t current_ticks, delta_ticks;
	int fps = 0;
	menuInfo menu;
	SecureZeroMemory(&menu, sizeof(menu));
	menu::loadDefaultConfig(&menu);
	while (!nt::GetKey(VK_END))
	{
		current_ticks = clock();

		UINT RealTimeError = 0x0;
		FeatureUtils::initDrawInfo();
		if (!MODULEBASE || !PID)
			RealTimeError = 3;
		DebugInfo debug;
		SecureZeroMemory(&debug, sizeof(debug));
		debug.fps = fps;

		menu::GetInputs(&menu);
		menu::AddMenu(&menu);

		if (!RealTimeError)
			Entity::UpdateEntityList(MODULEBASE, &debug, &menu);
		FeatureUtils::callRender();

		delta_ticks = clock() - current_ticks;
		if (delta_ticks > 0)
			fps = CLOCKS_PER_SEC / delta_ticks;
		menu.processTime = delta_ticks;
	}
	memory::RestoreAPCFlag(OriginalAPCFlag);
	std::terminate();
	return true;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		HANDLE thread = CreateThread(NULL, NULL, InternalThread, NULL, NULL, NULL);
		if (thread)
			CloseHandle(thread);
	}
	return TRUE;
}
