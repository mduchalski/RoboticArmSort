#include "animation.h"
#include "stdafx.h"
#include "RoboticArmSort.h"
#include "misc.h"

Animation::Animation(const UINT _timerId, const double _maxSpeed)
{
	timerId = _timerId;
	maxSpeed = _maxSpeed;

	alfaMove = betaMove = 0.0;
	isRunning = false;
	target = { -1.0f, -1.0f };
}

void Animation::OnTick(HWND hWnd, Arm& _arm, const std::vector<Block>& _blocks)
{
	if (AutomaticMode())
		UpdateSpeed();
	
	arm.Increment(alfaMove, betaMove);
	if (InConflict(arm, blocks))
	{
		arm.Increment(-alfaMove, -betaMove);
		KillTimer(hWnd, TMR);
	}
}

void Animation::SetMaxSpeed(const double _maxSpeed)
{
	maxSpeed = _maxSpeed;
}

void Animation::OnKeydown(HWND hWnd, const WPARAM wParam)
{
	if (DecodeKeys(wParam))
		UpdateSpeed();
	else return;

	if (!isRunning && !AutomaticMode())
	{
		SetTimer(hWnd, timerId, TIMER_FREQ, NULL);
		isRunning = true;
	}
}

void Animation::OnKeyup(HWND hWnd, const WPARAM wParam)
{
	alfaMove = (wParam == VK_LEFT || wParam == VK_RIGHT) ? 0.0 : alfaMove;
	betaMove = (wParam == VK_UP || wParam == VK_DOWN) ? 0.0 : betaMove;

	if (alfaMove == 0.0 && betaMove == 0.0)
	{
		isRunning = false;
		KillTimer(hWnd, timerId);
	}	
}

void Animation::GoToPos(const PointF _target)
{
	return;
}

void Animation::UpdateSpeed()
{
	if (AutomaticMode())
	{

	}
	else
	{
		if (std::abs(alfaMove) > 10e-6)
			alfaMove = (alfaMove > 0) ? maxSpeed / (double)(1000 / TIMER_FREQ) :
				-maxSpeed / (double)(1000 / TIMER_FREQ);
		if (std::abs(betaMove) > 10e-6)
			betaMove = (betaMove > 0) ? maxSpeed / (double)(1000 / TIMER_FREQ) :
			-maxSpeed / (double)(1000 / TIMER_FREQ);
	}
}

bool Animation::AutomaticMode()
{
	return target.X > 0.0 || target.Y > 0.0;
}

bool Animation::DecodeKeys(const WPARAM wParam)
{
	switch (wParam)
	{
	case VK_LEFT:
		alfaMove = 1.0;
		break;
	case VK_RIGHT:
		alfaMove = -1.0;
		break;
	case VK_UP:
		betaMove = 1.0;
		break;
	case VK_DOWN:
		betaMove = -1.0;
		break;
	default:
		return false;
		break;
	}
	return true;
}
