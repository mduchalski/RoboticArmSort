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

RectF GetBoundingRect(const Arm& _arm)
{
	return RectF(_arm.MountPoint().X + BLOCKS_OFFSET - _arm.LenB() * cos(M_PI -
		atan(BLOCKS_OFFSET / (MAX_BLOCK_HEIGHT - ARM_OFFSET)) - acos(
		(pow(_arm.LenB(), 2.0) - pow(_arm.LenA(), 2.0) + pow(BLOCKS_OFFSET, 2.0) +
		pow(MAX_BLOCK_HEIGHT - ARM_OFFSET, 2.0)) / (2.0 * _arm.LenB() * sqrt(
		pow(BLOCKS_OFFSET, 2.0) + pow(MAX_BLOCK_HEIGHT - ARM_OFFSET, 2.0))))),
		_arm.MountPoint().Y - _arm.LenA() - 10.0f,
		sqrt(pow(_arm.LenB(), 2.0) - pow(ZeroLine() - MIN_BLOCK_HEIGHT -
		_arm.MountPoint().Y + _arm.LenA() * sin(atan((MAX_BLOCK_HEIGHT -
		ARM_OFFSET) / BLOCKS_OFFSET)), 2.0)) + _arm.LenA() * cos(atan(
		(MAX_BLOCK_HEIGHT - ARM_OFFSET) / BLOCKS_OFFSET)) + 
		_arm.LenB() * cos(M_PI - atan(BLOCKS_OFFSET / (MAX_BLOCK_HEIGHT - ARM_OFFSET))
		- acos( (pow(_arm.LenB(), 2.0) - pow(_arm.LenA(), 2.0) + pow(BLOCKS_OFFSET, 2.0) +
		pow(MAX_BLOCK_HEIGHT - ARM_OFFSET, 2.0)) / (2.0 * _arm.LenB() * sqrt(
		pow(BLOCKS_OFFSET, 2.0) + pow(MAX_BLOCK_HEIGHT - ARM_OFFSET, 2.0))))),
		_arm.LenA() + ZeroLine() - _arm.MountPoint().Y + 10.0f);
}

RectF GetClientRectAlt(HWND hWnd)
{
	RECT temp;
	GetClientRect(hWnd, &temp);
	return RectF(0.0f, 0.0f, (REAL)temp.right, (REAL)temp.bottom);
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