#include "stdafx.h"
#include "RoboticArmSort.h"
#include "painting.h"
#include "drawing.h"
#include "misc.h"
#include "blocks.h"

// Paints non-changing user interface elements
void PaintGuiBase(HDC hdc)
{
	Graphics graphics(hdc);

	// TEXT HERE
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
	DrawWideCLine(graphics, &SolidBrush(ARM_COLOR_1), ArmPoint1(),  ArmPoint2(),
		1.5f * SCALE);
	DrawWideCLine(graphics, &SolidBrush(ARM_COLOR_2), ArmPoint0(),  ArmPoint1(),
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

