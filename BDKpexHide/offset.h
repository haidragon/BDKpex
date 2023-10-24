#pragma once


namespace off
{
	uint64_t cl_signOnState = 0x14329b4;
	uint64_t cl_entityList = 0x1b0e718;
	uint64_t cl_localplayer = 0x1ebf528;
	uint64_t cl_viewrender = 0x76419b0;

	uint64_t ViewMatrix = 0x11a210;

	uint64_t m_iHealth = 0x043c;
	uint64_t m_iTeamNum = 0x044c;
	uint64_t m_iName = 0x0589;
	uint64_t m_iMaxHealth = 0x0578;
	uint64_t m_shieldHealth = 0x0170;
	uint64_t m_shieldHealthMax = 0x0174;
	
	uint64_t m_nForceBone = 0xe98 + 0x50 - 0x8;

	uint64_t GlowColorStruct = 0x1D0;
	uint64_t GlowModeStruct = 0x2c4;
	uint64_t GlowContext = 0x3C8;
	uint64_t GlowVisibleType = 0x3D0;

	uint64_t lastVisibleTime = 0x1a70;

	uint64_t m_pStudioHdr = 0x10f0;
	uint64_t m_vecAbsOrigin = 0x014c;

	uint64_t m_latestPrimaryWeapons = 0x1a14;
	uint64_t m_flProjectileSpeed = 0x1ef0;
	uint64_t m_flProjectileScale = 0x1ef8;
	uint64_t camera_origin = 0x1f28;
	uint64_t m_lifeState = 0x0798;
	uint64_t m_bleedoutState = 0x2718;

	uint64_t BreathAngle = 0x253c - 0x14 - 0x10;
	uint64_t AimPunch = 0x2440;
}
