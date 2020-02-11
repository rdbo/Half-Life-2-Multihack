#pragma once
#include "includes.h"
#include "colors.h"
#define KEY_MENU VK_INSERT

typedef void(__cdecl* ConMsg)(char* msg);
typedef void(__cdecl* ConColorMsg)(Color* color, char* msg);

//HL2 Namespace

namespace HL2
{
	void DrawMenu();
	void Hook();
	void UnHook();
	DWORD WINAPI Hack(LPVOID lpReserved);
	void CustomCrosshair();
	namespace Offsets
	{
		//client.dll
		const PTR dwForceJump = 0x48BF5C;
		const PTR dwForceAttack = 0x48BF68;
		const PTR bShowCrosshair = 0x489608;
		const PTR r_drawothermodels = 0x45AF90;
		//tier0.dll
		const PTR fConMsg = 0x3A70;
		const PTR fConColorMsg = 0x38F0;
		//engine.dll
		const PTR dwFovBase = 0x446E7C;
		const PTR dwFov = 0x1048;
		const PTR dwPosBase = 0x446E7C;
		const PTR flPos = 0x334;
		//server.dll
		const PTR dwLocalPlayer = 0x634174;
		const PTR fDecAmmo = 0xE71CA; // 2 bytes
		const PTR fDecAmmo2 = 0x3D900B; // 6 bytes
		const PTR fDecAmmo3 = 0x3D94E1; // 6 bytes
		const PTR fDecAmmo4 = 0xF0984; // 2 bytes
		const PTR fDecAmmo5 = 0x3BF4B9; // 2 bytes
		const PTR fVPunch = 0x18324A;
		const PTR fHPunch = 0x183253;
		const PTR asmDecHealth = 0xEB8D9; //jmpSize = 8
		const PTR asmSetDuckTime = 0x186B28; //jmpSize = 7
		const PTR dwDuckTimeBase = 0x634174;
		const PTR flDuckTime = 0x8B0;
		const PTR dwSetSpeedBase = 0x634174;
		const PTR flSetSpeed = 0xDA8;
		//shaderapidx9.dll
		const PTR bShadersLoaded = 0x184778;
		//player
		const PTR dwHealth = 0xE0;
		const PTR bOnAir = 0xB60;
		const PTR dwArmor = 0xD30;
		const PTR strNick = 0xE4C;
		const PTR flAuxPower = 0x10CC;
		const PTR bOnTarget = 0xA88;
	}
}

struct iVec2
{
	int x, y;
};

class Crosshair
{
public:
	iVec2 size;
	float color[3];
};

//Player Class

class Player
{
public:
	char pad[HL2::Offsets::dwHealth];
	DWORD Health;
	char _pad[HL2::Offsets::bOnTarget - (HL2::Offsets::dwHealth + sizeof(Health))];
	bool OnTarget;
	char pad_[HL2::Offsets::bOnAir - (HL2::Offsets::bOnTarget + sizeof(OnTarget))];
	bool isOnAir;
	char pad__[HL2::Offsets::dwArmor - (HL2::Offsets::bOnAir + sizeof(isOnAir))];
	DWORD Armor;
	char pad___[HL2::Offsets::flAuxPower - (HL2::Offsets::dwArmor + sizeof(Armor))];
	float AuxPower;
};

//Data

extern bool showMenu;
extern PTR client;
extern PTR server;
extern PTR engine;
extern PTR tier0;
extern PTR shaderapi;
extern bool shadersLoaded;
extern PTR localPlayerAddr;
extern Player* localPlayer;
extern PTR localPlayerAddr;
extern Player* localPlayer;