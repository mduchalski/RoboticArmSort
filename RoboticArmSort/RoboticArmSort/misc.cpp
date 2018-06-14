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
	return RectF(_arm.MountPoint().X - 250.0f, _arm.MountPoint().Y - _arm.LenA() - 20.0f,
		750.0f, _arm.LenA() + ZeroLine() - _arm.MountPoint().Y + 20.0f);
}

RECT ConvertRect(const RectF rect)
{
	return { (INT)rect.X - 20, (INT)rect.Y - 20,
		(INT)(rect.X + rect.Width) + 20, (INT)(rect.Y + rect.Height) + 20 };
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

void AttemptGrab(Arm& _arm, std::vector<Block>& _blocks)
{
	for (std::vector<Block>::iterator it = _blocks.begin(); it != _blocks.end(); it++)
		_arm.AttemptGrab(*it);
}

void AttemptLayDown(Arm& _arm, std::vector<Block>& _blocks)
{
	for (std::vector<Block>::iterator it = _blocks.begin(); it != _blocks.end(); it++)
		_arm.AttemptLayDown(*it);
}

bool SortByPos(const Block a, const Block b)
{
	return a.LeftLine() < b.LeftLine();
}

bool SortByHeight(const std::pair<REAL, REAL> a, const std::pair<REAL, REAL> b)
{
	return a.second < b.second;
}
