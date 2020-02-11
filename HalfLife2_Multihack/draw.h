#pragma once
#include "includes.h"
#include "phook.h"

namespace Draw
{
	void DrawString(LPCSTR TextToDraw, int x, int y, D3DCOLOR color, LPD3DXFONT m_font);
	void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
	void DrawBorderBox(int x, int y, int w, int h, int thickness, D3DCOLOR color);
};

std::string IntToString(int number);
std::string FloatToString(float number);