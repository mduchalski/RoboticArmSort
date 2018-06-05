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

// Draws a "magnet" figure - straight surface of a given base length facing down
// with smooth transition to a circle of a given mounting point (center) and
// radius
void DrawMagnet(Graphics& graphics, const Brush* brush, const PointF mount,
	const REAL radius, const REAL height, const REAL base)
{
	graphics.FillEllipse(brush, RectF(mount.X - radius, mount.Y - radius,
		2 * radius, 2 * radius));
	REAL angle = (REAL)M_PI / 2.0f - atan(base / (2.0f * height)) - atan(
		sqrt(height * height + base * base / 4.0f - radius * radius) / radius);
	graphics.FillPolygon(brush, std::vector<PointF>{ 
		PointF(mount.X - base / 2.0f, mount.Y + height),
		PointF(mount.X + base / 2.0f, mount.Y + height),
		PointF(mount.X + radius * cos(angle), mount.Y + radius * sin(angle)),
		PointF(mount.X - radius * cos(angle), mount.Y + radius * sin(angle))
	}.data(), 4);
}
