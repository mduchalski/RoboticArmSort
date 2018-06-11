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
	Font font(FontFamily::GenericSansSerif(),
		0.6f * SCALE, 0, Gdiplus::UnitPixel);
	StringFormat format;
	format.SetLineAlignment(StringAlignment::StringAlignmentCenter);
	format.SetAlignment(StringAlignment::StringAlignmentCenter);

	// UI labels
	graphics.DrawString(L"Praca", -1, &font,
		RectF(SCALE, SCALE, 6.0f * SCALE, SCALE), &format, &brush);
	graphics.DrawString(L"Predkosc", -1, &font,
		RectF(SCALE, 3.0f * SCALE, 6.0f * SCALE, SCALE), &format, &brush);
	graphics.DrawString(L"Nagrywanie", -1, &font,
		RectF(SCALE, 6.0f * SCALE, 6.0f * SCALE, SCALE), &format, &brush);

	// Control description
	graphics.DrawString(L"strzalki: sterowanie, spacja: uchwyt", -1, &font,
		RectF(SCALE, 8.0f * SCALE, 6.0f * SCALE, 2.0f * SCALE),
		&format, &brush);

	// Control box bounding rectangle
	graphics.DrawRectangle(&Pen(Color::Black), RectF(0.5f * SCALE, 0.5f * SCALE,
		7.0f * SCALE, 10.0f * SCALE));
}

// Paints all window components
void OnPaint(HDC hdc)
{
	Bitmap bmp(window.X + window.Width, window.Y + window.Height);
	Graphics buffer(&bmp);

	buffer.FillRectangle(&SolidBrush(Color::White), armBounds);
	DrawArm(buffer, arm);
	for (std::vector<Block>::const_iterator cit = blocks.begin();
		cit != blocks.end(); cit++)
		cit->Draw(buffer);
	buffer.FillRectangle(&SolidBrush(Color::White), RectF(0.5f * SCALE, 
		0.5f * SCALE, 7.0f * SCALE, 10.0f * SCALE));
	PaintGui(buffer);
	// Draw a single horizontal line in the middle
	buffer.DrawLine(&Pen(Color::Black),
		PointF(window.X, window.Y + 0.8f * window.Height),
		PointF(window.X + window.Width, window.Y + 0.8f * window.Height));
	//buffer.DrawRectangle(&Pen(Color::Red), armBounds);

	Graphics graphics(hdc);
	graphics.DrawImage(&bmp, 0, 0);
}