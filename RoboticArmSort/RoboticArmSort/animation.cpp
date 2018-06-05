#include "animation.h"
#include "stdafx.h"
#include "RoboticArmSort.h"

// Alters move pair accordingly based on user key input
bool DecodeKeys(std::pair<double, double>& move, const WPARAM keyId)
{
	switch (keyId)
	{
	case VK_LEFT:
		move.first = ANIMATE_INCR;
		break;
	case VK_RIGHT:
		move.first = -ANIMATE_INCR;
		break;
	case VK_UP:
		move.second = ANIMATE_INCR;
		break;
	case VK_DOWN:
		move.second = -ANIMATE_INCR;
		break;
	default:
		return false;
		break;
	}
	return true;
}
