#include <windows.h>
#include <iostream>

BOOL BeepFail()
{
	Beep(300, 200);
	Beep(250, 200);
	Beep(200, 200);
	Beep(150, 200);
	Beep(100, 200);
	std::terminate();
	return true;
}

BOOL BeepSuccess()
{
	Beep(100, 200);
	Beep(150, 200);
	Beep(200, 200);
	Beep(250, 200);
	Beep(300, 200);
	return true;
}

double clockToMilliseconds(clock_t ticks)
{
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

uintptr_t milliseconds_now()
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
}