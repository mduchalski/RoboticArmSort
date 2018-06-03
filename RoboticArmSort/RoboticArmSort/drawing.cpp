#include "stdafx.h"
#include "drawing.h"

// Draws wide, circle-terminated line. Used for drawing the robot arm. Start,
// stop points create a line going through the middle of the figure, terminating
// at center points of the two circles
void DrawWideCLine(Graphics& graphics, const Brush* brush, const PointF start, 
	const PointF stop, const REAL width)
{
	graphics.DrawLine(&Pen(brush, width), start, stop);
	graphics.FillEllipse(brush, RectF(start.X - width / 2.0f,
		start.Y - width / 2.0f, width, width));
	graphics.FillEllipse(brush, RectF(stop.X - width / 2.0f,
		stop.Y - width / 2.0f, width, width));
}