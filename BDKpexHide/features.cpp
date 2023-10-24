#include "includes.h"
#include "offset.h"
#include "features.h"

#pragma region config

int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
int SW = SCREEN_WIDTH;
int SH = SCREEN_HEIGHT;
#pragma endregion

DrawInfo d[64];
EntityInfo EA[64];


bool menu::DrawIntro(UINT32 OriginalFlags, int user)
{
	d[0].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 10, const_cast<LPWSTR>(TEXT(L"Yanhax [Update Stalled]")) };
	std::wstring concatted_stdstr = L"Flag Verification: [" + std::to_wstring(OriginalFlags) + L"]Val";
	d[1].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 40, const_cast<LPWSTR>(TEXT(concatted_stdstr.c_str())) };
	d[3].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 70, const_cast<LPWSTR>(TEXT(L"[HOME] to start, [END] to Exit Safely"))};
	if (user == 1)
	{
		d[4].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 105, const_cast<LPWSTR>(TEXT(L"Welcome, <Bobby>")) };
		// bobby
	}
	else if (user == 2)
	{
		// james
		d[5].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 105, const_cast<LPWSTR>(TEXT(L"Welcome, <James>")) };
	}
	else if (user == 3)
	{
		d[6].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 105, const_cast<LPWSTR>(TEXT(L"Welcome, <Eddie>")) }; 
		// eddie
	}
	d[7].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 140, const_cast<LPWSTR>(TEXT(L"YANHAX IS DA BEST")) };
	d[8].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 170, const_cast<LPWSTR>(TEXT(L"YANHAX IS DA BEST")) };
	d[9].stringInfo = { 30, (ULONG)((unsigned int)SCREEN_WIDTH / 5) , 200, const_cast<LPWSTR>(TEXT(L"YANHAX IS DA BEST")) };

	return true;
}
bool menu::DrawMenu(UINT Error, DebugInfo debug)
{
	std::wstring concatted_stdstr = L"PC: " + std::to_wstring(debug.ValidPlayerCount) + L" Count";
	d[0].stringInfo = { 30, (ULONG)30 , 10, const_cast<LPWSTR>(TEXT(concatted_stdstr.c_str())) };
	std::wstring drawCount = L"DC: " + std::to_wstring(debug.DrawingPlayerCount) + L" Count";
	d[1].stringInfo = { 30, (ULONG)30 , 40, const_cast<LPWSTR>(TEXT(drawCount.c_str())) };
	
	std::wstring fpsCount = L"FPS: " + std::to_wstring(debug.fps) + L" :)";
	d[3].stringInfo = { 30, (ULONG)30, 70, const_cast<LPWSTR>(TEXT(fpsCount.c_str())) };
	if (Error == 3)
		d[1].stringInfo = { 30, (ULONG)30 , 10, const_cast<LPWSTR>(TEXT(L"NO BASE Lah"))};
	return true;
}
int R = 255; int G = 0; int B = 0;
void menu::AddMenu(menuInfo* menu)
{
#define LOGO_REGION_Y 40
	menu->SW = SW;
	menu::RainbowCreate(&R, &G, &B);
	d[8].Box = { R, G, B, (int)((unsigned int)menu->SW / 5 - 5), 0, 130, 40, 5 };

	d[0].stringInfo = { 30, (ULONG)((unsigned int)menu->SW / 5) , 5, const_cast<LPWSTR>(TEXT(L"YANHAX")) };
	if (menu->HideMenu)
	{
		d[1].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y, const_cast<LPWSTR>(TEXT(L"H-M [X]")) };
		return;
	}
	else
	{
		d[1].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y, const_cast<LPWSTR>(TEXT(L"H-M [-]")) };
	}
	if (menu->ReadOnly)
	{
		d[2].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 20, const_cast<LPWSTR>(TEXT(L"R-O [X]")) };
	}
	else
	{
		d[2].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 20, const_cast<LPWSTR>(TEXT(L"R-O [-]")) };
	}

	if (menu->aimToggle)
	{
		d[3].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 40, const_cast<LPWSTR>(TEXT(L"A-M [X]")) };
	}
	else
	{
		d[3].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 40, const_cast<LPWSTR>(TEXT(L"A-M [-]")) };
	}

	switch (menu->BoneChoice)
	{
	case 0:
		d[4].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 60, const_cast<LPWSTR>(TEXT(L"B-C <H>")) };
		break;
	case 1:
		d[4].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 60, const_cast<LPWSTR>(TEXT(L"B-C <N>")) };
		break;
	case 2:
		d[4].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 60, const_cast<LPWSTR>(TEXT(L"B-C <C>")) };
		break;
	case 3:
		d[4].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 60, const_cast<LPWSTR>(TEXT(L"B-C <B>")) };
		break;
	}

	if (!menu->SmartAim)
	{
		d[5].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 80, const_cast<LPWSTR>(TEXT(L"S-A [OFF]")) };

	}
	else if (menu->SmartAim == 1)
	{
		d[5].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 80, const_cast<LPWSTR>(TEXT(L"S-A [80%]")) };
	}
	else if (menu->SmartAim == 2)
	{
		d[5].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 80, const_cast<LPWSTR>(TEXT(L"S-A [90%]")) };
	}
	else if (menu->SmartAim == 3)
	{
		d[5].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 80, const_cast<LPWSTR>(TEXT(L"S-A [100%]")) };

	}

	if (menu->GlowMaster)
	{
		d[6].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 100, const_cast<LPWSTR>(TEXT(L"G-M [X]")) };
	}
	else
	{
		d[6].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 100, const_cast<LPWSTR>(TEXT(L"G-M [-]")) };
	}

	if (menu->HealthBar)
	{
		d[7].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 120, const_cast<LPWSTR>(TEXT(L"H-B [X]")) };
	}
	else
	{
		d[7].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 120, const_cast<LPWSTR>(TEXT(L"H-B [-]")) };
	}

	if (!menu->processTimeTog)
	{
		d[8].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 140, const_cast<LPWSTR>(TEXT(L"P-T [OFF]")) };
	}
	else
	{
		std::string PT = "P-T d[" + std::to_string(menu->processTime) + "]dx";
		std::wstring wPT = std::wstring(PT.begin(), PT.end());
		d[8].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 140, const_cast<LPWSTR>(TEXT(wPT.c_str())) };
	}
	std::wstring FV_num = std::to_wstring(menu->FOV);
	std::wstring FV = L"F-V d[" + FV_num + L"]dx";
	d[9].stringInfo = { 20, (ULONG)((unsigned int)menu->SW / 5), LOGO_REGION_Y + 160, const_cast<LPWSTR>(TEXT(FV.c_str())) };
	FV_num = L"     ";
	FV = L"                ";
	/* Draw Arrow */
	d[10].stringInfo = { 20, (ULONG)(((unsigned int)menu->SW / 5) - 20), LOGO_REGION_Y + (ULONG)(menu->ArrowPos * 20), const_cast<LPWSTR>(TEXT(L"->")) };
	/* 11th String Taken for Arrow */
}
namespace FeatureUtils
{
	bool initDrawInfo()
	{
		SecureZeroMemory(d, sizeof(d));
		return true;
	}
	bool callRender()
	{
		memory::render(true, true, true, true, d);
		return true;
	}

}
/*
Vector3 Entity::GetBonePositionHDR(uint64_t entity, Vector3 origin, uint32_t hitbox)
{
	ULONG64 BoneArray = memory::Rpm<ULONG64>(entity + off::m_nForceBone);
	matrix3x4_t Matrix = memory::Rpm<matrix3x4_t>(BoneArray + (GetBoneHDR(entity, hitbox) * sizeof(matrix3x4_t)));
	Vector3 bonePos;
	bonePos.x = Matrix.m[0][3] + origin.x;
	bonePos.y = Matrix.m[1][3] + origin.y;
	bonePos.z = Matrix.m[2][3] + origin.z;
	
	return bonePos;
}
uint32_t Entity::GetBoneHDR(uint64_t entity, uint32_t hitbox)
{
	ULONG64 Model = memory::Rpm<ULONG64>(entity + off::m_pStudioHdr);
	ULONG64 StudioHdr = memory::Rpm<ULONG64>(Model + 0x8);
	ULONG64 HitBoxSet = StudioHdr + memory::Rpm<ULONG>(StudioHdr + 0x34);
	return memory::Rpm<ULONG>(HitBoxSet + memory::Rpm<ULONG>(HitBoxSet + 0x8) + (hitbox * 0x2C));
}
*/
Vector3 Entity::GetBonePositionHDR2(uint64_t entity, Vector3 Origin, uint32_t hitbox)
{
	uint64_t Model = memory::Rpm<uint64_t>(entity + off::m_pStudioHdr);
	uint64_t StudioHdr = memory::Rpm<uint64_t>(Model + 0x8);
	uint16_t HitboxCache = memory::Rpm<uint16_t>(StudioHdr + 0x34);
	uint64_t HitboxArray = StudioHdr + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));
	uint16_t IndexCache = memory::Rpm<uint16_t>(HitboxArray + 4);
	int HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));
	uint16_t Bone = memory::Rpm<uint16_t>(HitboxIndex + HitboxArray + (hitbox * 0x20));
	uint64_t Bones = memory::Rpm<uint64_t>(entity + 0xe98 + 0x50 - 0x8);
	matrix3x4_t BoneMatrix = memory::Rpm<matrix3x4_t>(Bones + (Bone * sizeof(matrix3x4_t)));
	Vector3 bonepos;// = { BoneMatrix._14, BoneMatrix._24, BoneMatrix._34 };
	bonepos.x = BoneMatrix.m[0][3] + Origin.x;
	bonepos.y = BoneMatrix.m[1][3] + Origin.y;
	bonepos.z = BoneMatrix.m[2][3] + Origin.z;
	return bonepos;
}



unsigned int waitTicks = 0; unsigned int TargetWaitTciks = 0;
uintptr_t nextAim = 0;
uintptr_t nextVisUpdate = 0;

typedef struct EntityCache
{
	DWORD64 Entity;
	float OldVisTime;
	bool Visibility;
	bool Valid;
};
EntityCache EntCache[64];

bool Entity::UpdateEntityList(DWORD64 ModuleBase, DebugInfo* debug, menuInfo* menu)
{
	if (!ModuleBase)
		return false;
	DWORD SigonState = memory::Rpm<DWORD>(ModuleBase + off::cl_signOnState);
	if (SigonState != 8)
		return false;
	DWORD64 BaseEntity = memory::Rpm<DWORD64>(ModuleBase + off::cl_entityList);
	if (!BaseEntity)
		return false;
	DWORD64 LocalPlayer = memory::Rpm<DWORD64>(ModuleBase + off::cl_localplayer);
	if (!LocalPlayer)
		return false;
	int myTeamNum = memory::Rpm<DWORD>(LocalPlayer + off::m_iTeamNum);
	Vector3 MyOrigin = memory::Rpm<Vector3>(LocalPlayer + off::m_vecAbsOrigin);
	DWORD64 viewRenderer = memory::Rpm<DWORD64>(ModuleBase + off::cl_viewrender);
	DWORD64 viewMatrix = memory::Rpm<DWORD64>(viewRenderer + off::ViewMatrix);
	MatrixA u = memory::Rpm<MatrixA>(viewMatrix);
	UINT VEC = 0x0;
	
	EntityInfo BestAimEntity;
	SecureZeroMemory(&BestAimEntity, sizeof(EntityInfo));
	
	BestAimEntity.DeltaDistance = (float)menu->FOV;
	for (int i = 0; i < 64; i++)
	{
		DWORD64 Entity = memory::Rpm<DWORD64>(ModuleBase + off::cl_entityList + (i << 5));
		EntCache[i].Entity = Entity;
		if (!Entity)
			continue;
		int Health = memory::Rpm<int>(Entity + off::m_iHealth);
		if (!Health)
		{
			EntCache[i].Entity = NULL;
			EntCache[i].Valid = false;
			continue;
		}
		int TeamNum = memory::Rpm<int>(Entity + off::m_iTeamNum);
		if (TeamNum == myTeamNum)
		{
			EntCache[i].Entity = NULL;
			EntCache[i].Valid = false;
			continue;
		}
		int LifeState = memory::Rpm<int>(Entity + off::m_lifeState);
		int BleedOut = memory::Rpm<int>(Entity + off::m_bleedoutState);
		int ShieldHealth = memory::Rpm<int>(Entity + off::m_shieldHealth);
		int SheildHealthMax = memory::Rpm<int>(Entity + off::m_shieldHealthMax);
		Vector3 EntityOrigin = memory::Rpm<Vector3>(Entity + off::m_vecAbsOrigin);
		if (EntityOrigin.IsZero())
		{
			EntCache[i].Entity = NULL;
			EntCache[i].Valid = false;
			continue;
		}
		debug->ValidPlayerCount = debug->ValidPlayerCount + 1;
		EntCache[i].Entity = Entity;
		EntCache[i].Valid = true;

		Vector3 HeadPos = Entity::GetBonePositionHDR2(Entity, EntityOrigin, menu->BoneChoice);
		//Vector3 ChestPos = Entity::GetBonePositionHDR(Entity, EntityOrigin, 2);
		Vector3 W2sResult = { 0, 0, 0 };
#define InitialBoxSize 5000.0f
		float distance = sqrt(pow(HeadPos.x - MyOrigin.x, 2) + pow(HeadPos.y - MyOrigin.y, 2) + pow(HeadPos.z - MyOrigin.z, 2));
		int HeadBoxSize = InitialBoxSize / distance;
		if (HeadBoxSize <= 3)
			HeadBoxSize = 3;
		
		if (WorldToScreen(ModuleBase, HeadPos, &W2sResult, u) && !LifeState && !BleedOut)
		{
			int R, G, B;
			VEC++;
			if ((VEC + 10) >= 64)
			{
				continue;
			}
			if (EntCache[i].Visibility)
			{
				R = 0; G = 255; B = 255;
			}else
			{
				R = 255; G = 0; B = 0;
			}
			if (W2sResult.x <= SW && W2sResult.x >= 0 && W2sResult.y >= 0 && W2sResult.y <= SH)
			{
				//d[VEC + 10].Box = { R, G, B, (int)W2sResult.x - (int)(HeadBoxSize / 2),(int)W2sResult.y - (int)(HeadBoxSize / 2), HeadBoxSize, HeadBoxSize, 1 };
				debug->DrawingPlayerCount = debug->DrawingPlayerCount + 1;
				
				/* HB Color Calc*/

				int HealthPercentage = (int)(((float)Health + (float)ShieldHealth) / ((float)SheildHealthMax + 100.f) * 100.0f);
				if (HealthPercentage > 100)
					HealthPercentage = 100;
				R = 255; G = 255; B = 255;
				if (HealthPercentage > 50)
				{
					B = 255.f * (HealthPercentage - 50) / 50.f;
					G = 255; R = 255;
				}
				else if (HealthPercentage > 0)
				{
					G = 255.f * (HealthPercentage) / 50.f;
					B = 0; R = 255;
				}
				int HealthBarSize = HeadBoxSize * 8;
				if (HealthBarSize > 100)
					HealthBarSize = 100;
				if (HealthBarSize < 25)
					HealthBarSize = 25;
				if (menu->HealthBar)
				{
					d[VEC + 10].HealthBar = { 0, 0, 0, (int)W2sResult.x - HealthBarSize / 2, (int)W2sResult.y - (int)(HeadBoxSize / 2) - 15,
				HealthBarSize, 6, 1, 100, HealthPercentage, (int)R , (int)G, (int)B, false };
				}

				/* Aimbot */
				float deltaDistance = sqrt(pow((SW / 2) - W2sResult.x, 2) + pow((SH / 2) - W2sResult.y, 2));
				if (deltaDistance < BestAimEntity.DeltaDistance && EntCache[i].Visibility) /* Vis Check */
				{
					BestAimEntity.DeltaDistance = deltaDistance;
					BestAimEntity.id = i; BestAimEntity.BaseAddress = Entity; 
					BestAimEntity.BonePosition = HeadPos;
					BestAimEntity.DistanceAway = distance; BestAimEntity.W2SHead = W2sResult;
				}
			}
		}
		if (menu->GlowMaster && !menu->ReadOnly)
		{
			Entity::GlowByEntityVisibility(Entity, EntCache[i].Visibility);
		}
	}
	
	/* Visibility */
	if (nextVisUpdate < milliseconds_now())
	{
		for (int i = 0; i < 64; i++)
		{
			if (EntCache[i].Entity && EntCache[i].Valid)
			{
				EntCache[i].Visibility = UpdateVisibility(EntCache[i].Entity, i);
			}
			else
			{
				EntCache[i].Visibility = NULL;
			}
		}
		nextVisUpdate = milliseconds_now() + 17;
	}
	
	/* Aim Indicator */
	if (BestAimEntity.BaseAddress && menu->Aim_Direction_Display)
	{
		float Angle = std::atan2(BestAimEntity.W2SHead.y, BestAimEntity.W2SHead.x);
		Angle = Angle + PI;
		

	//	L"🡰 🡲 🡱 🡳 🡴 🡵 🡶 🡷"
	}


	if (BestAimEntity.BaseAddress && nt::GetKey(VK_RBUTTON) && nextAim < milliseconds_now() && menu->aimToggle) /* Have an entity to aim at */
	{
		aimbot::PredictPosition(ModuleBase, LocalPlayer, BestAimEntity.BaseAddress, &BestAimEntity.BonePosition, BestAimEntity.DistanceAway);
		Vector3 Output = { 0, 0, 0 };
		if (WorldToScreen(ModuleBase, BestAimEntity.BonePosition, &Output, u))
		{
			if (Output.x < SW && Output.x > 0 && Output.y < SH && Output.y > 0)
			{
				if (menu->PredictionBox)
				{
					d[9].Box = { 255, 122, 0, (int)Output.x - (int)(6),(int)Output.y - (int)(6), 12, 12, 3 };
				}
				/*
				Vector3 BreathAngle = memory::Rpm<Vector3>(LocalPlayer + off::BreathAngle);
				data::NormalizeAngles(BreathAngle);
				Vector3 AimPunchAngle = memory::Rpm<Vector3>(LocalPlayer + off::AimPunch);
				AimPunchAngle = AimPunchAngle * 0.05f;
				data::NormalizeAngles(AimPunchAngle);
				*/
				Vector3 Delta = { Output.x - (SW / 2), Output.y - (SH / 2) };
				Vector3 Result = Delta * 0.5f;
				if (!menu->SmartAim)
				{
					Result = data::AimBotHumanize(Delta, 8, 14);
				}
				else
				{
					Result = data::AimbotAggressiveHumanize(Delta, menu->SmartAim);
				}
				
				memory::SetMouseEvent((long)Result.x, (long)Result.y, NULL);
			}
		}
		nextAim = milliseconds_now() + 17;
	}
	return true;
}

bool Entity::UpdateVisibility(DWORD64 Entity, int Id)
{
	float VisTime = memory::Rpm<float>(Entity + off::lastVisibleTime);
	bool Visibility = false;
	if (VisTime > EntCache[Id].OldVisTime || VisTime < 0.f && EntCache[Id].OldVisTime > 0.f) {
		Visibility = true;
	}
	EntCache[Id].OldVisTime = VisTime;
	return Visibility;
}


bool ESP::DrawBox(UINT* DrawableCount)
{
#define BOXBASEVALUE 10
	int ValidCount = 0x0;
	for (int i = 0; i < 64; i++)
	{
		if (!EA[i].Valid || !EA[i].DrawAble)
		{continue;}else{ValidCount++;}

		if (EA[i].W2SHead.x <= SW && EA[i].W2SHead.x >= 0 && EA[i].W2SHead.y <= SH && EA[i].W2SHead.y >= 0)
		{
			d[ValidCount + BOXBASEVALUE].Box = { 255, 0, 0, (int)EA[i].W2SHead.x,(int)EA[i].W2SHead.y, 20, 20, 3 };
		}
	}
	return true;
}
bool ESP::Glow()
{
	int ValidCount = 0x0;
	for (int i = 0; i < 64; i++)
	{
		if (!EA[i].Valid)
		{
			continue;
		}
		else { ValidCount++; }
		float CR = 15.0f; float CG = 0.0f; float CB = 15.0f;
		ULONG RU = *((unsigned long*)&CR);
		ULONG GU = *((unsigned long*)&CG);
		ULONG BU = *((unsigned long*)&CB);

		memory::Wpm<ULONG>(EA[i].BaseAddress + off::GlowColorStruct, RU);
		memory::Wpm<ULONG>(EA[i].BaseAddress + off::GlowColorStruct + 4, GU);
		memory::Wpm<ULONG>(EA[i].BaseAddress + off::GlowColorStruct + 8, BU);

		memory::Wpm<ULONG>(EA[i].BaseAddress + off::GlowModeStruct, (ULONG)101);
		memory::Wpm<ULONG>(EA[i].BaseAddress + off::GlowModeStruct + sizeof(uint8_t), (ULONG)101);
		memory::Wpm<ULONG>(EA[i].BaseAddress + off::GlowModeStruct + sizeof(uint8_t) * 2, (ULONG)46);
		memory::Wpm<ULONG>(EA[i].BaseAddress + off::GlowModeStruct + sizeof(uint8_t) * 3, (ULONG)90);

		memory::Wpm<int>(EA[i].BaseAddress + off::GlowContext, (int)1);
		memory::Wpm<int>(EA[i].BaseAddress + off::GlowVisibleType, (int)2);

		//std::cout << skCrypt("[DEBUG] WRITING FINISHED") << std::endl;
		return true;
	}
	return true;
}
Vector3 Entity::ColorCalculation(int CurrentHealth, int MaxHealth)
{
#define	MAX_RED 15.0f
#define MAX_GREEN 15.0f
#define DEFAULT_RED 15.0f
#define DEFAULT_GREEN 0.0f
#define DEFAULT_BLUE 15.0f

	int CurrentTotalHealth = CurrentHealth;
	int EntityTotalMaxHealth = MaxHealth;

	float RedScale = MAX_RED * (1 - ((float)CurrentTotalHealth / (float)EntityTotalMaxHealth));
	float GreenScale = MAX_GREEN * ((float)CurrentTotalHealth / (float)EntityTotalMaxHealth);

	if (RedScale < 0.0f || RedScale > MAX_RED || GreenScale < 0.0f || GreenScale > MAX_GREEN)
	{
		return { DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE };
	}
	else
	{
		return { RedScale, GreenScale, 0.0f };
	}
}


bool Entity::GlowByEntity(DWORD64 Entity, int MaxHealth, int CurrentHealth)
{
	float CR = 15.0f; float CG = 0.0f; float CB = 15.0f;

	Vector3 HColor = ColorCalculation(CurrentHealth, MaxHealth);

	ULONG RU = *((unsigned long*)&HColor.x);
	ULONG GU = *((unsigned long*)&HColor.y);
	ULONG BU = *((unsigned long*)&HColor.z);

	memory::Wpm<ULONG>(Entity + off::GlowColorStruct, RU);
	memory::Wpm<ULONG>(Entity + off::GlowColorStruct + 4, GU);
	memory::Wpm<ULONG>(Entity + off::GlowColorStruct + 8, BU);

	memory::Wpm<ULONG>(Entity + off::GlowModeStruct, (ULONG)101);
	memory::Wpm<ULONG>(Entity + off::GlowModeStruct + sizeof(uint8_t), (ULONG)101);
	memory::Wpm<ULONG>(Entity + off::GlowModeStruct + sizeof(uint8_t) * 2, (ULONG)46);
	memory::Wpm<ULONG>(Entity + off::GlowModeStruct + sizeof(uint8_t) * 3, (ULONG)90);

	memory::Wpm<int>(Entity + off::GlowContext, (int)1);
	memory::Wpm<int>(Entity + off::GlowVisibleType, (int)2);
	return true;
}


bool Entity::GlowByEntityVisibility(DWORD64 Entity, bool Visibility)
{
	ColorStruct color;
	if (Visibility)
	{
		color = { 0.0f, 15.0f, 15.0f };
	}
	else if (!Visibility)
	{
		color = { 15.0f, 0.0f, 15.0f };
	}
	float CR = 15.0f; float CG = 0.0f; float CB = 15.0f;

	memory::Wpm<ColorStruct>(Entity + off::GlowColorStruct, color);
	GlowMode GM = { 101, 101, 46, 90 };
	memory::Wpm<GlowMode>(Entity + off::GlowModeStruct, GM);

	memory::Wpm<int>(Entity + off::GlowContext, (int)1);
	memory::Wpm<int>(Entity + off::GlowVisibleType, (int)2);
	return true;
}

bool Entity::WorldToScreen(DWORD64 ModuleBase, Vector3 vIn, Vector3* vOut, MatrixA Matrix)
{
	
	MatrixA m = Matrix;
	vOut->x = m.m[0] * vIn.x + m.m[1] * vIn.y + m.m[2] * vIn.z + m.m[3];
	vOut->y = m.m[4] * vIn.x + m.m[5] * vIn.y + m.m[6] * vIn.z + m.m[7];
	float w = m.m[12] * vIn.x + m.m[13] * vIn.y + m.m[14] * vIn.z + m.m[15];

	if (w < 0.01)
		return false;
	float invw = 1.0f / w;

	vOut->x *= invw;
	vOut->y *= invw;

	int width, height;

	width = SW;
	height = SH;

	float x = (float)width / 2;
	float y = (float)height / 2;

	x += 0.5 * vOut->x * (float)width + 0.5;
	y -= 0.5 * vOut->y * (float)height + 0.5;

	vOut->x = x;
	vOut->y = y;

	return true;
}


void aimbot::PredictPosition(DWORD64 ModuleBase, DWORD64 LocalPlayer, DWORD64 TargetEnt, Vector3* BonePos, float DistanceToMe)
{
	DWORD64 ActiveWeaponId = memory::Rpm<DWORD64>(LocalPlayer + off::m_latestPrimaryWeapons) & 0xFFFF;
	if (!ActiveWeaponId)
		return;
	DWORD64 CurrentWeapon = memory::Rpm<DWORD64>(ModuleBase + off::cl_entityList + (ActiveWeaponId << 5));
	if (!CurrentWeapon)
		return;
	float bulletSpeed = memory::Rpm<float>(CurrentWeapon + off::m_flProjectileSpeed);
	float bulletGravity = memory::Rpm<float>(CurrentWeapon + off::m_flProjectileScale);
	Vector3 EntityVelocity = memory::Rpm<float>(TargetEnt + off::m_vecAbsOrigin - 0xC);
	if (bulletSpeed > 1.0f)
	{
		float Time = DistanceToMe / bulletSpeed;
		BonePos->z += (700.f * bulletGravity * 0.5f) * (Time * Time);
		Vector3 velocity_delta = EntityVelocity * Time;
		BonePos->x += velocity_delta.x;
		BonePos->y += velocity_delta.y;
		BonePos->z += velocity_delta.z;
	}
}


Vector3 aimbot::CalcAngle(const Vector3& src, const Vector3& dst)
{
	Vector3 angle = { 0, 0, 0 };
	Vector3 delta = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };

	float hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	angle.x = atan(delta.z / hyp) * (float)(180.0 / PI);
	angle.y = atan(delta.y / delta.x) * (float)(180.0 / PI);
	angle.z = 0;
	if (delta.x >= 0.0) angle.y += 180.0f;

	return angle;
}

bool aimbot::NormalizeAngles(Vector3* angle)
{
	while (angle->x > 89.0f)
		angle->x -= 180.f;

	while (angle->x < -89.0f)
		angle->x += 180.f;

	while (angle->y > 180.f)
		angle->y -= 360.f;

	while (angle->y < -180.f)
		angle->y += 360.f;
	return true;

	if (angle->x > 89.0f || angle->x < -89.0f || angle->y > 180.f || angle->y < -180.f)
		return false;
}

/*

BOOL WALLS(DWORD64 ModuleBase)
{
	if (!ModuleBase)
		return false;
	DWORD64 SignOnState = memory::ReadMemory(ModuleBase + off::cl_signOnState, 0x8);
	if (SignOnState != 8)
	{
		//std::cout << "[DEBUG] SIGNONSTATE FAIL: " << SignOnState << std::endl;
		return false;
	}
	DWORD64 EntityList = ModuleBase + off::cl_entityList;
	DWORD64 BaseEntity = memory::ReadMemory(EntityList, 0x8);
	DWORD64 LocalPlayer = memory::ReadMemory(ModuleBase + off::cl_localplayer, 0x8);

	if (!BaseEntity || !LocalPlayer)
	{
		//std::cout << "[DEBUG] LOCALPLAYER OR BASE ENTITY EMPTY" << std::endl;
		return false;
	}

	UINT AmountGlow = 0;

	for (int i = 0; i < 64; i++)
	{
		DWORD64 EntityBase = memory::ReadMemory(EntityList + (i << 5), 0x8);
		if (!EntityBase)
			continue;
		int EntityHealth = memory::ReadMemory(EntityBase + off::m_iHealth, 0x4);
		if (EntityHealth <= 0 && EntityHealth > 100)
			continue;
		if (EntityBase == LocalPlayer)
			continue;
		int MyTeamNum = memory::ReadMemory(LocalPlayer + off::m_iTeamNum, 0x4);
		int EntityTeamNum = memory::ReadMemory(EntityBase + off::m_iTeamNum, 0x4);
		if (MyTeamNum == EntityTeamNum)
			continue;

		Vector3 ColorResult = glow::ColorCalculation(EntityBase, EntityHealth);

		glow::ColorGlow(EntityBase, { ColorResult.x, ColorResult.y, ColorResult.z }, { 101, 101, 46, 90 });
		
AmountGlow++;
	}
	std::cout << skCrypt("[DEBUG]AMOUNT GLOW: ") << AmountGlow << std::endl;

	return true;
}


*/

/*GLOW ONE LINER: 137, 138, 64, 88*/

void data::NormalizeAngles(Vector3& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

Vector3 data::AimbotAggressiveHumanize(Vector3 EnemyDelta, int Aggression)
{
	if (Aggression == 1)
	{
		EnemyDelta = EnemyDelta * 0.8f;
	}
	else if (Aggression == 2)
	{
		EnemyDelta = EnemyDelta;
	}
	return EnemyDelta;
}

Vector3 data::AimBotHumanize(Vector3 EnemyDelta, float AccuracyMin, float AccuracyMax)
{
	Vector3 n = EnemyDelta;
	Vector3 m = { 0, 0, 0 };
	Vector3 L /* Mid Point */ = { (n.x + m.x) / 2, (n.y + m.y) / 2, 0 };
	float c = L.y + ((1 / ((m.y - n.y) / (m.x - n.x))) * L.x);
	float D = sqrt(pow(n.x - m.x, 2) + pow(n.y - m.y, 2));
	Vector3 K = { c * ((m.y - n.y) / (m.x - n.x)), 0 };
	Vector3 J = { L.x, 0 };
	Vector3 V3 = (J.x - L.x, J.y - L.y);
	Vector3 V4 = { K.x - L.x, K.y - L.y };
	float MagU = sqrt(V4.x * V4.x + V4.y * V4.y);
	Vector3 U1 = { V4.x / MagU, V4.y / MagU, 0 };
	int RandomNumber = std::rand() % 100 + 1;
	float E = -D / ((AccuracyMax - AccuracyMin) * (RandomNumber / 100) + AccuracyMin);
	Vector3 VF = { L.x + U1.x * E, L.y + U1.y * E };
	return VF;
}
