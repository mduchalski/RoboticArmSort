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

	// Draw a single horizontal line in the middle
	graphics.DrawLine(&Pen(BASE_COLOR),
		PointF(window.X, window.Y + 0.8f * window.Height),
		PointF(window.X + window.Width, window.Y + 0.8f * window.Height));
}

// Paints all window components that change during an animation
void PaintAnimate(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT toInvalidate{ (INT)(ArmPoint0().X - ARM_LEN_1 - ARM_LEN_2 - 
			0.75f * SCALE) - 1,
		(INT)(ArmPoint0().Y - ARM_LEN_1 - ARM_LEN_2 - 0.75f * SCALE) - 1, 
		(INT)(ArmPoint0().X + ARM_LEN_1 + ARM_LEN_2 + 0.75f * SCALE) + 1, 
		(INT)(window.Y + 0.8f * window.Height) - 1 };

	InvalidateRect(hWnd, &toInvalidate, TRUE);

	Graphics graphics(hdc);
	PaintArm(graphics);

	EndPaint(hWnd, &ps);
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
		0.75f * SCALE, ARM_MAGNET_OFFSET, 1.5f * SCALE);
}

