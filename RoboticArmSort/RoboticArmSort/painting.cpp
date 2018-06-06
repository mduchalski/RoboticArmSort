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
	RectF((REAL)2, (REAL)2, (REAL)8, (REAL)2),
	&format, &brush);

	graphics.DrawString(L"Predkosc", -1, &font,
		RectF((REAL)2, (REAL)5, (REAL)8, (REAL)2),
		&format, &brush);

	graphics.DrawString(L"Nagrywanie", -1, &font,
		RectF((REAL)2, (REAL)8, (REAL)8, (REAL)2),
		&format, &brush);

	graphics.DrawString(L"Automatyczny", -1, &font,
		RectF((REAL)10, (REAL)2, (REAL)8, (REAL)1),
		&format, &brush);

	graphics.DrawString(L"Manualny", -1, &font,
		RectF((REAL)10, (REAL)3, (REAL)8, (REAL)1),
		&format, &brush);

	graphics.DrawString(L"Strzalki - sterowanie Spacja - uchwyt", -1, &font,
		RectF((REAL)1, (REAL)11, (REAL)18, (REAL)1),
		&format, &brush);
}



// Paints other (not UI) non-changing elements on the screen
void PaintBase(HDC hdc)
{
	Graphics graphics(hdc);
}