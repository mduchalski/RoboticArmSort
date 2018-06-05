#include "misc.h"
#include "stdafx.h"
#include "RoboticArmSort.h"

PointF ArmPoint0()
{
	return PointF(window.X + window.Width / 2.0f, window.Y +
		0.8f * window.Height - 5.0f * SCALE);
}

PointF ArmPoint1(const double _alfa)
{
	return PointF(window.X + window.Width / 2.0f + ARM_LEN_1 * (REAL)cos(_alfa),
		window.Y + 0.8f * window.Height - 5.0f * SCALE - ARM_LEN_1 * (REAL)sin(
			_alfa));
}

PointF ArmPoint2(const double _alfa, const double _beta)
{
	return PointF(window.X + window.Width / 2.0f + ARM_LEN_1 * (REAL)cos(_alfa)
		+ ARM_LEN_2 * (REAL)cos(_beta), window.Y + 0.8f * window.Height -
		5.0f * SCALE - ARM_LEN_1 * (REAL)sin(_alfa) - ARM_LEN_2 * (REAL)sin(
			_beta));
}