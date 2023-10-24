#pragma once
#include <Windows.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <tlhelp32.h>
#include <string>
#include <cstdint>
#include <math.h>
#include "ObfuscateCHAR.h"
#include "ObfuscateWCHAR.h"
#include "Syskey.h"
#include "vector.h"

typedef struct Vec3
{
	float x, y, z;
};

typedef struct VMatrix
{
	float m[16];
};

typedef struct DrawStringInfo
{
	DWORD size;
	DWORD X;
	DWORD Y;
	LPWSTR string;
};

typedef struct DrawHealthBarInfo
{
	int R, G, B, X, Y, W, H, T;
	int MaxiumHealth, CurrentHealth;
	int BarR, BarG, BarB;
	BOOL DownState;
};
typedef struct BoxInfo
{
	int R, G, B, X, Y, W, H, T;
};

typedef struct DrawInfo
{
	BoxInfo Box;
	DrawHealthBarInfo HealthBar;
	DrawStringInfo stringInfo;
	BoxInfo FillBoxInfo;
};
typedef struct BobbyCommuteStructure
{
	BOOL TestCommunication;

	DWORD Operation;
	DWORD64 Address;
	DWORD64 Buffer;
	DWORD PID;
	DWORD size;
	const char* name;
	const WCHAR* wcharName;
	DrawInfo MBox[64];

	BOOL drawBar;
	BOOL drawBox;
	BOOL drawText;
	BOOL drawFillBox;
	Vec3 vector;
	VMatrix matrix;

	LPWSTR ConsoleOut;
};

typedef struct DebugInfo
{
	UINT ValidPlayerCount;
	UINT DrawingPlayerCount;
	int fps;
};

typedef struct ColorStruct
{
	float R, G, B;
};

typedef struct GlowMode
{
	signed char GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

typedef struct _NF_MOUSE_REQUEST
{
	long x;
	long y;
	unsigned short button_flags;
}NF_MOUSE_REQUEST, * PNF_MOUSE_REQUEST;

typedef struct menuInfo
{
	bool HideMenu;
	long ArrowPos;
	bool aimToggle;
	int BoneChoice;
	int SmartAim;
	bool SuperLegit;
	bool ReadOnly;
	bool GlowMaster;
	bool PredictionBox;
	int processTime;
	bool processTimeTog;
	int Players;
	int PlayersRendered;
	bool HealthBar;
	int SW;
	int SH;
	int FOV;
	bool Aim_Direction_Display;
};


namespace process
{
	BOOL TestCommunication();
	BOOL init(const char* process_name, const char* module_name, DWORD64* ModuleBase, DWORD* ProcId);
}

BOOL BeepFail();
double clockToMilliseconds(clock_t ticks);


DWORD PassProcessId();
DWORD64 PassModuleBase();
namespace memory
{
	bool ReadMemory(DWORD64 address, PVOID pBuffer, DWORD size);
	template<typename T>
	inline T Rpm(uint64_t address)
	{

		T buffer;
		ReadMemory(address, &buffer, sizeof(T));
		return buffer;

	}
	
	BOOL WriteMemory(DWORD64 address, PVOID buffer, DWORD Size);

	template<typename T>
	inline void Wpm(uint64_t address, T buffer)
	{
		WriteMemory(address, &buffer, sizeof(T));
	}
	
	
	BOOL render(BOOL drawBar, BOOL drawText, BOOL drawBox, BOOL drawFill, DrawInfo DrawList[64]);
	uint32_t ClearAPCFlag();
	bool RestoreAPCFlag(uint32_t buffer);
	bool SetMouseEvent(long x, long y, unsigned short bu);
}

namespace initialization
{
	BOOL init();
}
namespace FeatureUtils
{
	BOOL InitEntityList();
	bool initDrawInfo();
	bool callRender();
}
namespace highlight
{
	BOOL ArrayDependentHighlight();
}

namespace menu
{
	bool DrawIntro(UINT32 OriginalFlags, int user);

	bool DrawMenu(UINT Error, DebugInfo debug);
	void ToggleVariable(bool* var);
	void GetInputs(menuInfo* menu);
	void AddMenu(menuInfo* menu);
	void loadDefaultConfig(menuInfo* menu);
	bool RainbowCreate(int* R, int* G, int* B) /* 255, 0, 0*/;

}

namespace Verification
{
	bool CheckUser(int* user);
}

namespace Entity
{
	bool UpdateEntityList(DWORD64 ModuleBase, DebugInfo* debug, menuInfo* menu);
	//Vector3 GetBonePositionHDR(uint64_t entity, Vector3 origin, uint32_t hitbox);
	uint32_t GetBoneHDR(uint64_t entity, uint32_t hitbox);
	bool WorldToScreen(DWORD64 ModuleBase, Vector3 vIn, Vector3* vOut, MatrixA Matrix);
	bool GlowByEntity(DWORD64 Entity, int MaxHealth, int CurrentHealth);
	bool UpdateVisibility(DWORD64 Entity, int Id);
	Vector3 ColorCalculation(int CurrentHealth, int MaxHealth);
	bool GlowByEntityVisibility(DWORD64 Entity, bool Visibility);
	bool UpdateVisibilityList(DWORD64* Entity, int* Id);
	Vector3 GetBonePositionHDR2(uint64_t entity, Vector3 Origin, uint32_t hitbox);

}
namespace ESP
{
	bool DrawBox(UINT* DrawableCount);
	bool Glow();
}


namespace aimbot
{
	void PredictPosition(DWORD64 ModuleBase, DWORD64 LocalPlayer, DWORD64 TargetEnt, Vector3* BonePos, float DistanceToMe);
	Vector3 CalcAngle(const Vector3& src, const Vector3& dst);
	bool NormalizeAngles(Vector3* angle);
}

namespace data
{
	Vector3 AimBotHumanize(Vector3 EnemyDelta, float AccuracyMin, float AccuracyMax);
	Vector3 AimbotAggressiveHumanize(Vector3 EnemyDelta, int Aggression);
	void NormalizeAngles(Vector3& angle);

}
uintptr_t milliseconds_now();

