#include "misc.h"
#include "stdafx.h"
#include "RoboticArmSort.h"

REAL ZeroLine()
{
	return window.Y + 0.8f * window.Height;
}

REAL CenterLine()
{
	return window.X + window.Width / 2.0f;
}

bool InConflict(const Arm& _arm, const Block& _block)
{
	if (((_arm.MountLine() <= _block.RightLine() && _arm.MountLine() >= _block.LeftLine()) ||
		(_arm.RightMountLine() >= _block.LeftLine() && _arm.RightMountLine() <= _block.RightLine())) &&
		_arm.EndLine() >= _block.HighLine()) return true;
	return false;
}

bool InConflict(const Arm& _arm, const std::vector<Block>& _blocks)
{
	for (std::vector<Block>::const_iterator cit = _blocks.begin();
		cit != blocks.end(); cit++)
		if (!cit->IsMounted() && InConflict(_arm, *cit)) return true;
	return false;
}