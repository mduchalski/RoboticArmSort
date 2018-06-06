#include "stdafx.h"
#include "RoboticArmSort.h"
#include "painting.h"
#include "drawing.h"

// Paints non-changing user interface elements
void PaintGuiBase(HDC hdc)
{
	Graphics graphics(hdc);

	// TEXT HERE
	SolidBrush brush(BASE_COLOR);

	Font font(FontFamily::GenericSansSerif(), SCALE, 11, Gdiplus::UnitPoint);

	StringFormat format;format.SetLineAlignment(StringAlignment::StringAlignmentCenter);
	format.SetAlignment(StringAlignment::StringAlingmentCenter);

	graphics.DrawString(L"Tryb Pracy", -1, &font,
	RectF((REAL)2* SCALE, (REAL)2 * SCALE, (REAL)8 * SCALE, (REAL)2 * SCALE),
	&format, &brush);

	graphics.DrawString(L"Predkosc", -1, &font,
		RectF((REAL)2 * SCALE, (REAL)5 * SCALE, (REAL)8 * SCALE, (REAL)2 * SCALE),
		&format, &brush);

	graphics.DrawString(L"Nagrywanie", -1, &font,
		RectF((REAL)2 * SCALE, (REAL)8 * SCALE, (REAL)8 * SCALE, (REAL)2 * SCALE),
		&format, &brush);

	graphics.DrawString(L"Automatyczny", -1, &font,
		RectF((REAL)10 * SCALE, (REAL)2 * SCALE, (REAL)8 * SCALE, (REAL)1 * SCALE),
		&format, &brush);

	graphics.DrawString(L"Manualny", -1, &font,
		RectF((REAL)10 * SCALE, (REAL)3 * SCALE, (REAL)8 * SCALE, (REAL)1 * SCALE),
		&format, &brush);

	graphics.DrawString(L"Strzalki - sterowanie Spacja - uchwyt", -1, &font,
		RectF((REAL)1 * SCALE, (REAL)11 * SCALE, (REAL)18 * SCALE, (REAL)1 * SCALE),
		&format, &brush);


}



// Paints other (not UI) non-changing elements on the screen
void PaintBase(HDC hdc)
{
	Graphics graphics(hdc);
}