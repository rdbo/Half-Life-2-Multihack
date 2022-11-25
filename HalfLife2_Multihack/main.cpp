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
	roboto = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\PressStart2P-Regular.ttf", 12, &font_cfg);
	style.WindowMinSize = ImVec2(160, 20);
	style.FramePadding = ImVec2(4, 2);
	style.ItemSpacing = ImVec2(6, 2);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.Alpha = 0.95f;
	style.IndentSpacing = 6.0f;
	style.ItemInnerSpacing = ImVec2(2, 4);
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 14.0f;
	style.GrabRounding = 16.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.WindowRounding = 0.3f;
	style.WindowBorderSize = 0;
	style.TabBorderSize = 0;
	style.TabRounding = 0;
	style.ItemInnerSpacing.x = 1.0f;
	style.Colors[ImGuiCol_Text] = COLOR_WHITE;
	style.Colors[ImGuiCol_TextDisabled] = COLOR_WHITE;
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
	style.Colors[ImGuiCol_Button] = ImVec4(56, 81, 112, 255);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(56, 81, 112, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(56, 81, 112, 255);
	style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.37f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.43f, 0.69f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.43f, 0.69f, 0.58f, 1.00f);
	
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
	Beep(523, 250);
	Beep(700, 250);
	Beep(900, 250);
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