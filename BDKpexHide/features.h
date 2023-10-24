#pragma once


typedef struct EntityInfo
{
	DWORD64 BaseAddress;
	Vector3 BonePosition;
	float oldVisTime;
	BOOL Valid;
	BOOL Visible;
	BOOL DrawAble;
	Vector3 W2SResult;
	float DeltaDistance;
	float DistanceAway;
	int id;
	int Health;
	int TeamNum;
	Vector3 HeadPos;
	Vector3 StomachPos;
	Vector3 NeckPos;
	Vector3 W2SHead;
	Vector3 W2SStom;
	Vector3 W2SNeck;
};

namespace glow
{

}
namespace ABT
{
	Vector3 GetEntityBonePosition(DWORD64 EntityBase, int TargetBoneId);
}

