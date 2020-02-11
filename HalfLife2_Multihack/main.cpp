#include "includes.h"
#include "phook.h"
#include "hack.h"
#include "colors.h"
#include "fonts/roboto_regular.h"

#define IMGUI_WINDOW_W 400
#define IMGUI_WINDOW_H 400
#define FONT_SIZE 18

HWND window;
RECT wCRect;
void* pDevice[119];
PTR EndSceneAddress;
EndScene oEndScene;
WNDPROC oWndProc;
bool showMenu = false;
ImFont* roboto;
LPDIRECT3DDEVICE9 p_device;

void InitImGui(LPDIRECT3DDEVICE9 device)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMinSize = ImVec2(IMGUI_WINDOW_W, IMGUI_WINDOW_H);
	ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;
	roboto = io.Fonts->AddFontFromMemoryTTF(roboto_regular, sizeof(roboto_regular), FONT_SIZE, &font_cfg);
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = COLOR_WHITE;
	colors[ImGuiCol_WindowBg] = COLOR_BLACK;
	colors[ImGuiCol_TitleBg] = COLOR_DARK_ORANGE;
	colors[ImGuiCol_TitleBgActive] = COLOR_ORANGE;
	colors[ImGuiCol_Tab] = COLOR_DARK_ORANGE;
	colors[ImGuiCol_TabHovered] = COLOR_LIGHT_ORANGE;
	colors[ImGuiCol_TabActive] = COLOR_ORANGE;
	colors[ImGuiCol_CheckMark] = COLOR_ORANGE;
	colors[ImGuiCol_FrameBg] = COLOR_DARK_GRAY;
	colors[ImGuiCol_FrameBgHovered] = COLOR_DARK_GRAY_2;
	colors[ImGuiCol_FrameBgActive] = COLOR_GRAY;
	colors[ImGuiCol_SliderGrab] = COLOR_ORANGE;
	colors[ImGuiCol_SliderGrabActive] = COLOR_LIGHT_ORANGE;
	colors[ImGuiCol_Button] = COLOR_DARK_ORANGE;
	colors[ImGuiCol_ButtonHovered] = COLOR_ORANGE;
	colors[ImGuiCol_ButtonActive] = COLOR_LIGHT_ORANGE;
	colors[ImGuiCol_Border] = COLOR_ORANGE;
	colors[ImGuiCol_ResizeGrip] = COLOR_DARK_ORANGE;
	colors[ImGuiCol_ResizeGripHovered] = COLOR_ORANGE;
	colors[ImGuiCol_ResizeGripActive] = COLOR_LIGHT_ORANGE;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

bool initialized = false;
long __stdcall PHook::hkEndScene(LPDIRECT3DDEVICE9 device)
{
	GetClientRect(window, &wCRect);

	if (!initialized)
	{
		InitImGui(device);
		initialized = true;
	}
	p_device = device;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (showMenu)
		HL2::DrawMenu();

	HL2::CustomCrosshair();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(device);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall PHook::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
		pKeyHook[wParam] = uMsg;
	if (showMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == KEY_MENU)
		{
			showMenu = !showMenu;
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PHook::Init, hMod, 0, nullptr);
		CreateThread(nullptr, 0, HL2::Hack, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PHook::Shutdown, hMod, 0, nullptr);
		break;
	}
	return TRUE;
}