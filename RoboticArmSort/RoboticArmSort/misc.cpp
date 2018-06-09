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

std::pair<bool, double> Bisection(double f(const double, const double, 
	const PointF, const Arm&), const double known, const PointF target, 
	const Arm& _arm, double a, double b, const double eps)
{
	while (f(a, known, target, _arm) * f(b, known, target, _arm) > 0 && 
		std::abs(a - b) > eps)
		b -= eps;
	if (f(a, known, target, _arm) * f(b, known, target, _arm) > 0)
		return { false, 0.0 };

	double c = a;
	while ((b - a) >= eps)
	{
		c = (a + b) / 2;

		if (f(c, known, target, _arm) == 0.0)
			break;
		else if (f(c, known, target, _arm) * f(a, known, target, _arm) < 0)
			b = c;
		else
			a = c;
	}

	return { true, c };
}
