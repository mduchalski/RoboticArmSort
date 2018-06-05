#include "misc.h"
#include "stdafx.h"
#include "RoboticArmSort.h"

PointF ArmPoint0()
{
	return PointF(window.X + window.Width / 2.0f, window.Y +
		0.8f * window.Height - 5.0f * SCALE);
}

PointF ArmPoint1()
{
	return PointF(window.X + window.Width / 2.0f + ARM_LEN_1 * (REAL)cos(angle.first),
		window.Y + 0.8f * window.Height - 5.0f * SCALE - ARM_LEN_1 * (REAL)sin(
			angle.first));
}

PointF ArmPoint2()
{
	return PointF(window.X + window.Width / 2.0f + ARM_LEN_1 * (REAL)cos(angle.first)
		+ ARM_LEN_2 * (REAL)cos(angle.second), window.Y + 0.8f * window.Height -
		5.0f * SCALE - ARM_LEN_1 * (REAL)sin(angle.first) - ARM_LEN_2 * (REAL)sin(
			angle.second));
}
