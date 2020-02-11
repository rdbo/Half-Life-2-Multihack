#include "draw.h"

void Draw::DrawBorderBox(int x, int y, int w, int h, int thickness, D3DCOLOR color)
{
	DrawFilledRect(x, y, w, thickness, color); //Top horiz line
	DrawFilledRect(x, y, thickness, h, color); // Left vertical line
	DrawFilledRect((x + w), y, thickness, h, color); //right vertical line
	DrawFilledRect(x, y + h, w + thickness, thickness, color); //bottom horiz line
}

void Draw::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT BarRect = { x, y, x + w, y + h }; //create the rect
	p_device->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0); //clear = re-draw target area
}

void Draw::DrawString(LPCSTR TextToDraw, int x, int y, D3DCOLOR color, LPD3DXFONT m_font)
{
	RECT rct = { x - 120, y, x + 120, y + 15 }; //create the rect
	m_font->DrawText(NULL, TextToDraw, -1, &rct, DT_NOCLIP, color);
}