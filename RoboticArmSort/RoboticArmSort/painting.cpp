#include "stdafx.h"
#include "RoboticArmSort.h"
#include "painting.h"
#include "drawing.h"
#include "misc.h"

// Paints non-changing user interface elements
void PaintGui(Graphics& graphics)
{
	SolidBrush brush(BASE_COLOR);

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

	graphics.DrawRectangle(&Pen(BASE_COLOR), SCALE, SCALE, 18 * SCALE, 11 * SCALE);
}

// Paints all window components that change during an animation
void OnPaint(HDC hdc)
{
	RECT toInvalidate{ (INT)(ArmPoint0().X - ARM_LEN_1 - ARM_LEN_2 -
		0.75f * SCALE) - 1,
		(INT)(ArmPoint0().Y - ARM_LEN_1 - ARM_LEN_2 - 0.75f * SCALE) - 1,
		(INT)(ArmPoint0().X + ARM_LEN_1 + ARM_LEN_2 + 0.75f * SCALE) + 1,
		(INT)(window.Y + 0.8f * window.Height) - 1 };

	Bitmap bmp(toInvalidate.right, toInvalidate.bottom);
	Graphics buffer(&bmp);

	buffer.FillRectangle(&SolidBrush(Color::White), Rect(toInvalidate.left, toInvalidate.top, toInvalidate.right - toInvalidate.left, toInvalidate.bottom - toInvalidate.top));
	PaintArm(buffer);
	PaintBlocks(buffer, blocks);
	PaintGui(buffer);
	// Draw a single horizontal line in the middle
	buffer.DrawLine(&Pen(BASE_COLOR),
		PointF(window.X, window.Y + 0.8f * window.Height),
		PointF(window.X + window.Width, window.Y + 0.8f * window.Height));

	Graphics graphics(hdc);
	graphics.DrawImage(&bmp, 0, 0);
}

// Paint the robotic arm with a terminating magnet
void PaintArm(Graphics& graphics)
{
	// Draw a "magnet" shaped arm mount
	DrawMagnet(graphics, &SolidBrush(ARM_COLOR_1), ArmPoint0(),
		0.75f * SCALE, ARM_OFFSET, 3.0f * SCALE);
	// Draw two arm parts
	DrawWideCLine(graphics, &SolidBrush(ARM_COLOR_1), ArmPoint1(), ArmPoint2(),
		1.5f * SCALE);
	DrawWideCLine(graphics, &SolidBrush(ARM_COLOR_2), ArmPoint0(), ArmPoint1(),
		1.5f * SCALE);
	// Draw terminating magnet
	DrawMagnet(graphics, &SolidBrush(ARM_COLOR_2), ArmPoint2(),
		1.375f * BLOCK_WIDTH / 2.0f, MAX_BLOCK_HEIGHT - MIN_BLOCK_HEIGHT,
		BLOCK_WIDTH);
}

// Paints all blocks in the given vector
void PaintBlocks(Graphics& graphics, const std::vector<Block>& _blocks)
{
	for (std::vector<Block>::const_iterator cit = blocks.begin();
		cit != blocks.end(); cit++)
		if (cit->mounted)
			graphics.FillRectangle(&SolidBrush(Color::DarkBlue), RectF(
				ArmPoint2().X - 0.5f * BLOCK_WIDTH,
				ArmPoint2().Y - MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
				BLOCK_WIDTH, cit->height));
		else
			graphics.FillRectangle(&SolidBrush(Color::DarkBlue), RectF(
				ArmPoint0().X + cit->relativePos,
				window.Y + 0.8f * window.Height - cit->height,
				BLOCK_WIDTH, cit->height));
}