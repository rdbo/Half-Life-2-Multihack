#include "hack.h"
#include "phook.h"
#include "draw.h"
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

//Game data
PTR client;
PTR server;
PTR engine;
PTR tier0;
PTR shaderapi;
bool shadersLoaded;
PTR localPlayerAddr;
Player* localPlayer;
float* SetSpeed;
bool* showCrosshair;
PTR* rDrawOtherModels;
char oDecAmmo[2];
char oDecAmmo2[6];
char oDecAmmo3[6];
char oDecAmmo4[2];
char oDecAmmo5[2];
char oVPunch[4];
char oHPunch[5];
Crosshair crosshair;
bool hl2_init = false;
//Values
PTR localPlayerHealthAddr;
PTR HealthValue = 100;
PTR ArmorValue = 100;
PTR speedValue = 320;
float AuxValue = 100;
int* zeroPtr;

//Menu Variables
bool inf_health = false;
bool inf_armor = false;
bool inf_aux = false;
bool inf_ammo = false;
bool hitkill = false;
bool triggerbot = false;
bool brk_speed = false;
bool no_ducktime = false;
bool no_punch = false;
bool wireframe = false;
bool conlog = false;
bool xhair = false;
bool autobh = false;

//Hooks
UINT pKeyHook[0xFE];
//Functions
Color* cWhite;
Color* cRed;
Color* cOrange;
Color* cBlue;
Color* cGreen;
ConMsg oConMsg;
ConColorMsg oConColorMsg;
void ConsoleLog(char* text, Color* color = nullptr);
void CheckHealth();
void CheckArmor();
void CheckAux();
void CheckTriggerbot();
void CheckCrosshair();
void CheckCrosshair();
void CheckWireframe();
void CheckSpeed();
void ForceAttack();
void ForceJump();
void bunnyhop();
void InfiniteAmmo();
PTR DecHealthAddr;
PTR DecHealthJumpAddr;
void DecHealth();
PTR SetDuckTimeAddr;
PTR SetDuckTimeJumpAddr;
void SetDuckTime();
void NoViewPunch();

void HL2::DrawMenu()
{
	ImGui::SetNextWindowSize({ 400.0f, 500.0f });
	ImGui::Begin("Half-Life: 2 Cheat by rdbo & Zordon1337", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Text("HL2Ware.net by rdbo & Zordon1337");
	ImGui::BeginTabBar("navbar");
	if (ImGui::BeginTabItem("Cheats"))
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Infinite Health", &inf_health);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Infinite Armor", &inf_armor);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Infinite Aux Power", &inf_aux);
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Checkbox("Infinite Ammo", &inf_ammo))
			InfiniteAmmo();
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Checkbox("Auto BunnyHop", &autobh))
			bunnyhop();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("One-Hit Kills", &hitkill);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Triggerbot", &triggerbot);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Break MaxSpeed", &brk_speed);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Instant Duck", &no_ducktime);
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Checkbox("No View Punch", &no_punch))
			NoViewPunch();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Wireframe", &wireframe);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Console Logs", &conlog);
		ImGui::EndTabItem();
	}	

	if (ImGui::BeginTabItem("Custom Crosshair"))
	{
		ImGui::Checkbox("Enable", &xhair);
		ImGui::SliderInt("Crosshair Width", &crosshair.size.x, 1, SCREEN_WIDTH - 1);
		ImGui::SliderInt("Crosshair Height", &crosshair.size.y, 1, SCREEN_HEIGHT - 1);
		ImGui::ColorPicker3("Crosshair Color", crosshair.color);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();
}

DWORD WINAPI HL2::Hack(LPVOID lpReserved)
{
	client = (PTR)GetModuleHandle("client.dll");
	tier0 = (PTR)GetModuleHandle("tier0.dll");
	engine = (PTR)GetModuleHandle("engine.dll");
	server = (PTR)GetModuleHandle("server.dll");
	shaderapi = (PTR)GetModuleHandle("shaderapidx9.dll");
	DecHealthAddr = (PTR)(server + HL2::Offsets::asmDecHealth);
	DecHealthJumpAddr = DecHealthAddr + 8;
	SetDuckTimeAddr = (PTR)(server + HL2::Offsets::asmSetDuckTime);
	SetDuckTimeJumpAddr = (PTR)(SetDuckTimeAddr + 7);
	memcpy(&oDecAmmo, reinterpret_cast<char*>(server + HL2::Offsets::fDecAmmo), 2);
	memcpy(&oDecAmmo2, reinterpret_cast<char*>(server + HL2::Offsets::fDecAmmo2), 6);
	memcpy(&oDecAmmo3, reinterpret_cast<char*>(server + HL2::Offsets::fDecAmmo3), 6);
	memcpy(&oDecAmmo4, reinterpret_cast<char*>(server + HL2::Offsets::fDecAmmo4), 2);
	memcpy(&oDecAmmo5, reinterpret_cast<char*>(server + HL2::Offsets::fDecAmmo5), 2);
	memcpy(&oVPunch, reinterpret_cast<char*>(server + HL2::Offsets::fVPunch), 4);
	memcpy(&oHPunch, reinterpret_cast<char*>(server + HL2::Offsets::fHPunch), 5);

	oConMsg = (ConMsg)(tier0 + HL2::Offsets::fConMsg);
	oConColorMsg = (ConColorMsg)(tier0 + HL2::Offsets::fConColorMsg);
	cWhite = new Color(255, 255, 255, 255);
	cRed = new Color(255, 25, 25, 255);
	cOrange = new Color(255, 150, 25, 255);
	cBlue = new Color(25, 25, 255, 255);
	cGreen = new Color(25, 255, 25, 255);
	zeroPtr = new int();
	*zeroPtr = 0;
	crosshair.size.x = 1;
	crosshair.size.y = 11;
	crosshair.color[0] = 1.f;
	crosshair.color[1] = 0.5f;
	crosshair.color[2] = 0.f;
	while (!hl2_init)
	{
		if (init_hook)
		{
			Hook();
			hl2_init = true;
			ConsoleLog((char*)"Injected HL2Ware.net! have fun!");
		}
	}
	while (init_hook)
	{
		shadersLoaded = *(bool*)(shaderapi + HL2::Offsets::bShadersLoaded);
		if (shadersLoaded)
		{
			localPlayerAddr = *(PTR*)(server + HL2::Offsets::dwLocalPlayer);
			if (localPlayerAddr != NULL)
			{
				SetSpeed != (float*)((*(DWORD*)(server + HL2::Offsets::dwSetSpeedBase)) + HL2::Offsets::flSetSpeed) ?
					SetSpeed = (float*)((*(DWORD*)(server + HL2::Offsets::dwSetSpeedBase)) + HL2::Offsets::flSetSpeed) : 0;
				rDrawOtherModels != (PTR*)(client + HL2::Offsets::r_drawothermodels) ?
					rDrawOtherModels = (PTR*)(client + HL2::Offsets::r_drawothermodels) : 0;
				showCrosshair != (bool*)(client + HL2::Offsets::bShowCrosshair) ?
					showCrosshair = (bool*)(client + HL2::Offsets::bShowCrosshair) : 0;

				localPlayer = (Player*)(localPlayerAddr);
				localPlayerHealthAddr = (PTR)(localPlayerAddr + HL2::Offsets::dwHealth);
				CheckHealth();
				CheckArmor();
				CheckAux();
				CheckTriggerbot();
				CheckCrosshair();
				CheckCrosshair();
				CheckWireframe();
				CheckSpeed();
				bunnyhop();
			}
		}
	}
	return TRUE;
}

void CheckHealth()
{
	if (inf_health && localPlayer->Health < HealthValue)
	{
		localPlayer->Health = HealthValue;
		char buff[50];
		snprintf(buff, sizeof(buff), "Health set to: %i", HealthValue);
		ConsoleLog(buff, cGreen);
	}
}


void bunnyhop()
{
	if (autobh)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			if (!localPlayer->isOnAir)
			{
				*(DWORD*)(client + HL2::Offsets::dwForceJump) = 6;
			}
		}
	}
}
void CheckArmor()
{
	if (inf_armor && localPlayer->Armor < ArmorValue)
	{
		localPlayer->Armor = ArmorValue;
		char buff[50];
		snprintf(buff, sizeof(buff), "Armor set to: %i", ArmorValue);
		ConsoleLog(buff, cGreen);
	}
}
void CheckAux()
{
	if (inf_aux && localPlayer->AuxPower < AuxValue)
	{
		localPlayer->AuxPower = AuxValue;
		char buff[50];
		snprintf(buff, sizeof(buff), "Aux Power set to: %f", AuxValue);
		ConsoleLog(buff, cGreen);
	}
}

void CheckTriggerbot()
{
	if (triggerbot && localPlayer->OnTarget == true)
	{
		char buff[50];
		snprintf(buff, sizeof(buff), "Aim On Target");
		ConsoleLog(buff, cBlue);
		ForceAttack();
	}
}
void CheckCrosshair()
{
	if (xhair && *showCrosshair == 1)
	{
		*showCrosshair = 0;
		char buff[50];
		snprintf(buff, sizeof(buff), "Crosshair is now hidden");
		ConsoleLog(buff, cBlue);
	}

	else if (!xhair && *showCrosshair == 0)
	{
		*showCrosshair = 1;
		char buff[50];
		snprintf(buff, sizeof(buff), "Crosshair is no longer hidden");
		ConsoleLog(buff, cBlue);
	}
}

void CheckWireframe()
{
	if (wireframe && *rDrawOtherModels != 2)
	{
		*rDrawOtherModels = 2;
		char buff[50];
		snprintf(buff, sizeof(buff), "Wireframe On");
		ConsoleLog(buff, cBlue);
	}

	if (!wireframe && *rDrawOtherModels == 2)
	{
		*rDrawOtherModels = 1;
		char buff[50];
		snprintf(buff, sizeof(buff), "Wireframe Off");
		ConsoleLog(buff, cBlue);
	}
}

void CheckSpeed()
{
	if (brk_speed && *SetSpeed != speedValue)
	{
		*SetSpeed = speedValue;
		char buff[50];
		snprintf(buff, sizeof(buff), "Speed set to: %i", speedValue);
		ConsoleLog(buff, cBlue);
	}
}

__declspec(naked) void DecHealth()
{
	__asm
	{
		cmp localPlayerAddr,0
		je originalcode

		infinite_health:
		cmp inf_health,1
		jne one_hit_kills
		
		cmp esi,localPlayerHealthAddr
		jne one_hit_kills
		mov edi,HealthValue
		jmp originalcode

		one_hit_kills:
		cmp hitkill,1
		jne originalcode
		cmp esi,localPlayerHealthAddr
		je originalcode
		mov edi,0
		jmp originalcode


		originalcode:
		mov[esi], edi
		pop edi
		mov eax, 00000001
		jmp DecHealthJumpAddr
	}
}
__declspec(naked) void SetDuckTime()
{
	__asm
	{
		cmp no_ducktime,1
		jne originalcode
		movss xmm0,[zeroPtr]
		jmp originalcode

		originalcode:
		movss[edi], xmm0
		mov edi, [esi+04]
		jmp SetDuckTimeJumpAddr
	}
}

void HL2::Hook()
{
	MH_CreateHook((LPVOID)DecHealthAddr, DecHealth, nullptr);
	MH_EnableHook((LPVOID)DecHealthAddr);
	MH_CreateHook((LPVOID)SetDuckTimeAddr, SetDuckTime, nullptr);
	MH_EnableHook((LPVOID)SetDuckTimeAddr);
}

void HL2::UnHook()
{
	MH_DisableHook((LPVOID)DecHealthAddr);
	MH_RemoveHook((LPVOID)DecHealthAddr);
	MH_DisableHook((LPVOID)SetDuckTimeAddr);
	MH_RemoveHook((LPVOID)SetDuckTimeAddr);
}

void ConsoleLog(char* text, Color* color)
{
	if (conlog)
	{
		oConColorMsg(cOrange, (char*)"[HL2: Log] ");
		if (color != nullptr)
		{
			oConColorMsg(color, text);
		}

		else
		{
			oConMsg(text);
		}
		oConMsg((char*)"\n");
	}
}

void ForceAttack()
{
	*(DWORD*)(client + HL2::Offsets::dwForceAttack) = 6;
}
void ForceJump()
{
	*(DWORD*)(client + HL2::Offsets::dwForceJump) = 6;
}

void NoViewPunch()
{
	if (no_punch)
	{
		char nop[5] = "\x90\x90\x90\x90";
		char nop5[6] = "\x90\x90\x90\x90\x90";
		PHook::WriteBytes((server + HL2::Offsets::fVPunch), nop, 4);
		PHook::WriteBytes((server + HL2::Offsets::fHPunch), nop5, 5);
	}

	else
	{
		PHook::WriteBytes((server + HL2::Offsets::fVPunch), oVPunch, 4);
		PHook::WriteBytes((server + HL2::Offsets::fHPunch), oHPunch, 5);
	}
}

void InfiniteAmmo()
{
	if (inf_ammo)
	{
		char nop[3] = "\x90\x90";
		char nop6[7] = "\x90\x90\x90\x90\x90\x90";
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo), nop, 2);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo2), nop6, 6);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo3), nop6, 6);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo4), nop, 2);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo5), nop, 2);
	}

	else
	{
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo), oDecAmmo, 2);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo2), oDecAmmo2, 6);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo3), oDecAmmo3, 6);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo4), oDecAmmo4, 2);
		PHook::WriteBytes((server + HL2::Offsets::fDecAmmo5), oDecAmmo5, 2);
	}
}

void HL2::CustomCrosshair()
{
	if (shadersLoaded && xhair)
	{
		D3DCOLOR color = D3DCOLOR_ARGB((int)(255), (int)(crosshair.color[0] * 255), (int)(crosshair.color[1] * 255), (int)(crosshair.color[2] * 255));
		crosshair.size.x % 2 == 1 ? 0 : crosshair.size.x += 1;
		crosshair.size.y % 2 == 1 ? 0 : crosshair.size.y += 1;
		int screenW = wCRect.right - wCRect.left;
		int screenH = wCRect.bottom - wCRect.top;
		int screenCenterX = (int)(screenW / 2);
		int screenCenterY = (int)(screenH / 2);
		int centerX = (int)(crosshair.size.x / 2);
		int centerY = (int)(crosshair.size.y / 2);
		Draw::DrawFilledRect(screenCenterX - centerX, screenCenterY - centerY, crosshair.size.x, crosshair.size.y, color);
		Draw::DrawFilledRect(screenCenterX - centerY, screenCenterY - centerX, crosshair.size.y, crosshair.size.x, color);
	}
}