#include "stdafx.h"
#include "RoboticArmSort.h"
#include "painting.h"
#include "drawing.h"

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
}