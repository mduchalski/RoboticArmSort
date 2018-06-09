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
	alfaTarget = betaTarget = M_PI + 1.0;
}

void Animation::OnTick(HWND hWnd, Arm& _arm, const std::vector<Block>& _blocks)
{
	if (AutomaticMode())
	{
		UpdateSpeed(_arm);

		if (std::abs(alfaTarget - _arm.Alfa()) <= std::abs(alfaMove))
			alfaMove = 0.0;
		if (std::abs(betaTarget - _arm.Beta()) <= std::abs(betaMove))
			betaMove = 0.0;
		
		if (!AutomaticMode())
		{
			KillTimer(hWnd, timerId);
			isRunning = false;
			alfaTarget = betaTarget = M_PI + 1.0;
		}
	}

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

void Animation::OnKeydown(HWND hWnd, const WPARAM wParam, Arm& _arm)
{
	if (DecodeKeys(wParam))
		UpdateSpeed(_arm);
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

void Animation::GoToPos(HWND hWnd, Arm& _arm, const PointF _target)
{
	return;
}

void Animation::Move(HWND hWnd, const Arm& _arm, bool direction, REAL dist)
{
	autoDirection = direction;
	PointF target = _arm.EndPoint() + 
		(autoDirection ? PointF(dist, 0.0f) : PointF(0.0f, dist));
	alfaTarget = GetAlfaTarget(target, _arm);
	betaTarget = GetBetaTarget(target, _arm);

	alfaMove = (alfaTarget > _arm.Alfa()) ? 1.0 : -1.0;
	betaMove = (betaTarget > _arm.Beta()) ? 1.0 : -1.0;

	if (AutomaticMode())
	{
		SetTimer(hWnd, timerId, TIMER_FREQ, NULL);
		isRunning = true;
	}
}

void Animation::UpdateSpeed(Arm& _arm)
{
	if (std::abs(alfaMove) > 10e-6)
		alfaMove = (alfaMove > 0) ? maxSpeed / (double)(1000 / TIMER_FREQ) :
		-maxSpeed / (double)(1000 / TIMER_FREQ);
	if (std::abs(betaMove) > 10e-6)
		betaMove = (betaMove > 0) ? maxSpeed / (double)(1000 / TIMER_FREQ) :
		-maxSpeed / (double)(1000 / TIMER_FREQ);

	// For automatic mode operation, such speed ratio is found that arm moves
	// either parrarel to X- or Y-axis
	if (AutomaticMode() && autoDirection)
		betaMove = -(_arm.LenA() * cos(_arm.Alfa())) * alfaMove / 
			(_arm.LenB() * cos(_arm.Beta()));
	else if (AutomaticMode())
		betaMove = (_arm.LenA() * sin(_arm.Alfa())) * std::abs(alfaMove) /
			(_arm.LenB() * sin(_arm.Beta()));
}

bool Animation::AutomaticMode()
{
	return alfaTarget <= M_PI && betaTarget <= M_PI;
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

double Animation::GetAlfaTarget(const PointF _target, const Arm& _arm)
{
	return atan((_arm.MountPoint().Y - _target.Y) /
		(_target.X - _arm.MountPoint().X)) + acos((pow(_arm.LenA(), 2.0) + pow(
		_arm.MountPoint().Y - _target.Y, 2.0) + pow(_target.X -
		_arm.MountPoint().X, 2.0) - pow(_arm.LenB(), 2.0)) / (2 * _arm.LenA() * 
		sqrt(pow(_arm.MountPoint().Y - _target.Y, 2.0) + pow(_target.X -
		_arm.MountPoint().X, 2.0))));
}

double Animation::GetBetaTarget(const PointF _target, const Arm& _arm)
{
	return alfaTarget - M_PI + acos((pow(_arm.LenA(), 2.0) +
		pow(_arm.LenB(), 2.0) - pow(_arm.MountPoint().Y - _target.Y, 2.0) -
		pow(_target.X - _arm.MountPoint().X, 2.0)) / (2 * _arm.LenA() *
		_arm.LenB()));
}

double AlfaFromXY(const double _alfa, const double _, 
	const PointF target, const Arm& _arm)
{
	return pow(target.X - _arm.MountPoint().X - _arm.LenA() * cos(_alfa), 2.0) +
		pow(target.Y - _arm.MountPoint().Y + _arm.LenA() * sin(_alfa), 2.0)
		- _arm.LenB() * _arm.LenB();
}
double BetaFromXY(const double _beta, const double _alfa,
	const PointF target, const Arm& _arm)
{
	return _arm.MountPoint().X + _arm.LenA() * cos(_alfa) + _arm.LenB() * 
		cos(_beta) - target.X;
}
