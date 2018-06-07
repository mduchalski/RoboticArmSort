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

bool InConflict(const std::vector<Block>& _blocks)
{
	if (ArmPoint2().Y + MAX_BLOCK_HEIGHT - MIN_BLOCK_HEIGHT >= window.Y + 0.8f * window.Height)
		return true;
	for (std::vector<Block>::const_iterator cit = _blocks.begin();
		cit != _blocks.end(); cit++)
		if ((ArmPoint2().X >= ArmPoint0().X + cit->relativePos &&
			ArmPoint2().X <= ArmPoint0().X + cit->relativePos + BLOCK_WIDTH) &&
			ArmPoint2().Y + MAX_BLOCK_HEIGHT - MIN_BLOCK_HEIGHT >= 
			window.Y + 0.8f * window.Height - cit->height)
				return true;
	return false;
}