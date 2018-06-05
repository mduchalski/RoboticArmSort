#include "stdafx.h"
#include "RoboticArmSort.h"
#include "painting.h"
#include "drawing.h"
#include "misc.h"

// Paints non-changing user interface elements
void PaintGuiBase(HDC hdc)
{
	Graphics graphics(hdc);

	// TEXT HERE
}

// Paints other (not UI) non-changing elements on the screen
void PaintBase(HDC hdc)
{
	Graphics graphics(hdc);

	// Draw a "magnet" shaped arm mount
	DrawMagnet(graphics, &SolidBrush(ARM_COLOR_1),
		PointF(window.X + window.Width / 2.0f, 
			window.Y + 0.8f * window.Height - 5.0f * SCALE),
		0.75f * SCALE, ARM_OFFSET, 3.0f * SCALE);
	// Draw a single horizontal line in the middle
	graphics.DrawLine(&Pen(BASE_COLOR),
		PointF(window.X, window.Y + 0.8f * window.Height),
		PointF(window.X + window.Width, window.Y + 0.8f * window.Height));
}

// Paint the robotic arm with a terminating magnet
void PaintArm(HDC hdc, const double _alfa, const double _beta)
{
	Graphics graphics(hdc);

	DrawWideCLine(graphics, &SolidBrush(ARM_COLOR_1), ArmPoint1(_alfa),
		ArmPoint2(_alfa, _beta), 1.5f * SCALE);
	DrawWideCLine(graphics, &SolidBrush(ARM_COLOR_2), ArmPoint0(), 
		ArmPoint1(_alfa), 1.5f * SCALE);
	DrawMagnet(graphics, &SolidBrush(ARM_COLOR_2), ArmPoint2(_alfa, _beta),
		0.75f * SCALE, ARM_MAGNET_OFFSET, 1.5f * SCALE);
}

