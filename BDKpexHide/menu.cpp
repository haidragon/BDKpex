#include "includes.h"


namespace conskey
{
	bool K_UP = false;
	bool K_DOWN = false;
	bool K_LEFT = false;
	bool K_RIGHT = false;
}

void menu::loadDefaultConfig(menuInfo* menu)
{
	menu->aimToggle = false;
	menu->ArrowPos = 0;
	menu->BoneChoice = 0;
	menu->GlowMaster = false;
	menu->HealthBar = true;
	menu->PredictionBox = true;
	menu->processTimeTog = false;
	menu->ReadOnly = true;
	menu->SmartAim = false;
	menu->SuperLegit = false;
	menu->FOV = GetSystemMetrics(SM_CXSCREEN) / 12;
}

bool Red_Direct = true;
bool Blue_Direct = false;
#define RIA 1
bool menu::RainbowCreate(int* R, int* G, int* B) /* 255, 0, 0*/
{
	*G = 255;

	/* Increase Red */

	if (Red_Direct) /* forward */
	{
		if (*R <= (255 - RIA))
			*R = *R + RIA;
		else
			Red_Direct = false;
	}
	else /* backwards */
	{
		if (*R >= RIA)
			*R = *R - RIA;
		else
			Red_Direct = true;
	}


	if (Blue_Direct) /* forward */
	{
		if (*B <= (255 - RIA))
			*B = *B + RIA;
		else
			Blue_Direct = false;
	}
	else /* backwards */
	{
		if (*B >= RIA)
			*B = *B - RIA;
		else
			Blue_Direct = true;
	}


	if (*R > 255)
		*R = 255;
	if (*G > 255)
		*G = 255;
	if (*B > 255)
		*B = 255;

	if (*R < 0)
		*R = 0;
	if (*G < 0)
		*G = 0;
	if (*B < 0)
		*B = 0;

	return true;
}


void menu::GetInputs(menuInfo* menu)
{
#define MAX_MENU_LENGTH 8
	unsigned int TravResult = 0;
	if (nt::GetKey(VK_UP) && !conskey::K_UP)
	{
		conskey::K_UP = true;
		TravResult = 1;
	}
	else if (!nt::GetKey(VK_UP))
		conskey::K_UP = false;

	if (nt::GetKey(VK_DOWN) && !conskey::K_DOWN)
	{
		conskey::K_DOWN = true;
		TravResult = 2;
	}
	else if (!nt::GetKey(VK_DOWN))
		conskey::K_DOWN = false;
	
	if (nt::GetKey(VK_LEFT) && !conskey::K_LEFT)
	{
		conskey::K_LEFT = true;
		TravResult = 3;
	}
	else if (!nt::GetKey(VK_LEFT))
		conskey::K_LEFT = false;

	if (nt::GetKey(VK_RIGHT) && !conskey::K_RIGHT)
	{
		conskey::K_RIGHT = true;
		TravResult = 4;
	}
	else if (!nt::GetKey(VK_RIGHT))
		conskey::K_RIGHT = false;

	if (!TravResult)
		return;

	if ((TravResult == 1 || TravResult == 2) && menu->HideMenu)
	{
		menu->ArrowPos = 0;
		return;
	}

	switch (TravResult)
	{
	case 1: /* UP*/
		if (menu->ArrowPos >= 1)
			menu->ArrowPos--;
		break;
	case 2: /* DOWN*/
		if (menu->ArrowPos < MAX_MENU_LENGTH)
			menu->ArrowPos++;
		break;
	case 3: /* LEFT*/
	{
		switch (menu->ArrowPos)
		{
		case 0:
			ToggleVariable(&menu->HideMenu);
			break;
		case 1:
			ToggleVariable(&menu->ReadOnly);
			if(menu->ReadOnly)
				menu->GlowMaster = false;
			break;
		case 2:
			ToggleVariable(&menu->aimToggle);
			break;
		case 3:
			/* 0, 1, 2, 3*/
			if (menu->BoneChoice > 0)
				menu->BoneChoice--;
			break;
		case 4:
			if (menu->SmartAim > 0)
				menu->SmartAim--;
			break;
		case 5:
			if (!menu->ReadOnly)
			{
				ToggleVariable(&menu->GlowMaster);
			}
			else
				menu->GlowMaster = false;
			break;
		case 6:
			ToggleVariable(&menu->HealthBar);
			break;
		case 7:
			ToggleVariable(&menu->processTimeTog);
			break;
		case 8:
			if (menu->FOV > 1)
				menu->FOV = menu->FOV - 10;
			break;
		}
	}
		break;
	case 4: /* RIGHT*/
	{
		switch (menu->ArrowPos)
		{
		case 0:
			ToggleVariable(&menu->HideMenu);
			break;
		case 1:
			ToggleVariable(&menu->ReadOnly);
			if (menu->ReadOnly)
				menu->GlowMaster = false;
			break;
		case 2:
			ToggleVariable(&menu->aimToggle);
			break;
		case 3:
			/* 0, 1, 2, 3*/
			if (menu->BoneChoice < 3)
				menu->BoneChoice++;
			break;
		case 4:
			if (menu->SmartAim < 3)
				menu->SmartAim++;
			break;
		case 5:
			ToggleVariable(&menu->GlowMaster);
			break;
		case 6:
			ToggleVariable(&menu->HealthBar);
			break;
		case 7:
			ToggleVariable(&menu->processTimeTog);
			break;
		case 8:
			if (menu->FOV < 500)
				menu->FOV = menu->FOV + 10;
			break;
		}
	}
		break;
	}
}

void menu::ToggleVariable(bool* var)
{
	if (*var)
		*var = false;
	else
		*var = true;
}