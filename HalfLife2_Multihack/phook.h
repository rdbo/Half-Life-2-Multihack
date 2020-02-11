#pragma once
#include "includes.h"

extern bool init_hook;
extern HWND window;
extern RECT wCRect;
extern void* pDevice[119];
extern PTR EndSceneAddress;
extern EndScene oEndScene;
extern WNDPROC oWndProc;
extern bool initialized;
extern UINT pKeyHook[0xFE];
extern LPDIRECT3DDEVICE9 p_device;

namespace PHook
{
	void Init();
	void Shutdown();
	void WriteBytes(PTR addressToWrite, char* valueToWrite, int byteNum);
	long __stdcall hkEndScene(LPDIRECT3DDEVICE9 device);
	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}