#include "stdafx.h"
#include "RoboticArmSort.h"
#include "painting.h"
#include "drawing.h"
#include "misc.h"
#include "arm.h"

// Paints non-changing user interface elements
void PaintGui(Graphics& graphics)
{
	SolidBrush brush(Color::Black);

	Font font(FontFamily::GenericSansSerif(), SCALE, 0, Gdiplus::UnitPixel);

	StringFormat format;
	format.SetLineAlignment(StringAlignment::StringAlignmentCenter);
	format.SetAlignment(StringAlignment::StringAlignmentCenter);

	graphics.DrawString(L"Tryb pracy", -1, &font,
	RectF((REAL)2* SCALE, (REAL)2 * SCALE, (REAL)8 * SCALE, (REAL)2 * SCALE),
	&format, &brush);

	graphics.DrawString(L"Prêdkoœæ", -1, &font,
		RectF((REAL)2 * SCALE, (REAL)5 * SCALE, (REAL)8 * SCALE, (REAL)2 * SCALE),
		&format, &brush);

	graphics.DrawString(L"Nagrywanie", -1, &font,
		RectF((REAL)2 * SCALE, (REAL)8 * SCALE, (REAL)8 * SCALE, (REAL)2 * SCALE),
		&format, &brush);

	Font smallFont(FontFamily::GenericSansSerif(), 0.75f * SCALE, 0, Gdiplus::UnitPixel);
	graphics.DrawString(L"strza³ki: sterowanie, spacja: uchwyt", -1, &smallFont,
		RectF((REAL)1 * SCALE, (REAL)11 * SCALE, (REAL)18 * SCALE, (REAL)1 * SCALE),
		&format, &brush);

	graphics.DrawRectangle(&Pen(Color::Black), SCALE, SCALE, 18 * SCALE, 11 * SCALE);
}

// Paints all window components
void OnPaint(HDC hdc)
{
	Bitmap bmp(window.X + window.Width, window.Y + window.Height);
	Graphics buffer(&bmp);

	buffer.FillRectangle(&SolidBrush(Color::White), window);
	DrawArm(buffer, arm);
	for (std::vector<Block>::const_iterator cit = blocks.begin();
		cit != blocks.end(); cit++)
		cit->Draw(buffer);
	PaintGui(buffer);
	// Draw a single horizontal line in the middle
	buffer.DrawLine(&Pen(Color::Black),
		PointF(window.X, window.Y + 0.8f * window.Height),
		PointF(window.X + window.Width, window.Y + 0.8f * window.Height));
	buffer.DrawRectangle(&Pen(Color::Red), armBounds);

	Graphics graphics(hdc);
	graphics.DrawImage(&bmp, 0, 0);
}