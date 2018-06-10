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
	action = { Nothing, 0.0f, 0.0f, true };
}

void Animation::OnTick(HWND hWnd, Arm& _arm, std::vector<Block>& _blocks,
	std::queue<AnimationActionCont>& actions)
{
	// Handle interfacing with the queue
	if (actions.empty() && action.finished && action.action)
		Stop(hWnd);

	if (!actions.empty() && actions.front() == action && action.finished)
	{
		actions.front().finished = true;
		actions.front().retVal = action.retVal;
		action = { Nothing, 0.0f, 0.0f, true };
	}
	if (!actions.empty() && action.finished)
	{
		action = actions.front();
		Move(hWnd, _arm);
	}
	

	// Stop if in desired A-B range
	if (action.action)
	{
		UpdateSpeed(_arm);
		DecodeInRange(_arm);
		if (std::abs(alfaMove) < 10e-3 && std::abs(betaMove) < 10e-3)
			action.finished = true;
	}

	// Check for conflicts
	arm.Increment(alfaMove, betaMove);
	if (InConflict(arm, blocks) || !_arm.InRect(armBounds))
	{
		arm.Increment(-alfaMove, -betaMove);
		if (!action.action)
			Stop(hWnd);
		if (action.action == VerticalCheck)
		{
			action.finished = true;
			action.retVal = ZeroLine() - _arm.LowLine();
		}
		else if (action.action == VerticalGrab)
		{
			AttemptGrab(_arm, _blocks);
			action.finished = true;
		}
		else if (action.action == VerticalLayDown)
		{
			AttemptLayDown(_arm, _blocks);
			action.finished = true;
		}
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

	if (!isRunning)
		Start(hWnd);
}

void Animation::OnKeyup(HWND hWnd, const WPARAM wParam)
{
	alfaMove = (wParam == VK_LEFT || wParam == VK_RIGHT) ? 0.0 : alfaMove;
	betaMove = (wParam == VK_UP || wParam == VK_DOWN) ? 0.0 : betaMove;

	if (alfaMove == 0.0 && betaMove == 0.0)
		Stop(hWnd);
}

void Animation::Move(HWND hWnd, const Arm& _arm)
{
	PointF target;
	if (action.action == HorizontalMove)
		target = PointF(action.parameter, _arm.EndPoint().Y);
	else if (action.action == VerticalMove)
		target = PointF(_arm.EndPoint().X, action.parameter);
	else if (action.action != Nothing)
		target = target + PointF(0.0f, ZeroLine() - _arm.EndLine() + 25.0f);
	ZeroLine();
	_arm.MountPoint();

	alfaTarget = GetAlfaTarget(target, _arm);
	betaTarget = GetBetaTarget(target, _arm);

	alfaMove = (alfaTarget > _arm.Alfa()) ? 1.0 : -1.0;
	betaMove = (betaTarget > _arm.Beta()) ? 1.0 : -1.0;

	if (!action.action)
		Start(hWnd);
}

void Animation::UpdateSpeed(Arm& _arm)
{
	// For all types of animations, calculate move values for the given speed
	if (std::abs(alfaMove) > 10e-6)
		alfaMove = (alfaMove > 0) ? maxSpeed / (double)(1000 / TIMER_DELAY) :
		-maxSpeed / (double)(1000 / TIMER_DELAY);
	if (std::abs(betaMove) > 10e-6)
		betaMove = (betaMove > 0) ? maxSpeed / (double)(1000 / TIMER_DELAY) :
		-maxSpeed / (double)(1000 / TIMER_DELAY);

	// For automatic mode operation, such speed ratio is found that arm moves
	// either parrarel to X- or Y-axis
	if (action.action == HorizontalMove)
		betaMove = -(_arm.LenA() * cos(_arm.Alfa())) * alfaMove /
		(_arm.LenB() * cos(_arm.Beta()));
	// Y-axis pararell shifting is only implemented below certain level,
	// because it doesn't work reliably on higher ones
	else if (action.action > HorizontalMove && 
		_arm.EndLine() >= ZeroLine() - MAX_BLOCK_HEIGHT)
		betaMove = (_arm.LenA() * sin(_arm.Alfa())) * -alfaMove /
		(_arm.LenB() * sin(_arm.Beta()));
}

void Animation::Start(HWND hWnd)
{
	SetTimer(hWnd, timerId, TIMER_DELAY, NULL);
	isRunning = true;
}

void Animation::Stop(HWND hWnd)
{
	action = { Nothing, 0.0f, 0.0f, true };
	isRunning = false;
	KillTimer(hWnd, TMR);
}

void Animation::DecodeInRange(const Arm& _arm)
{
	if (std::abs(alfaTarget - _arm.Alfa()) <= std::abs(alfaMove))
		alfaMove = 0.0;
	if (std::abs(betaTarget - _arm.Beta()) <= std::abs(betaMove))
		betaMove = 0.0;
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

bool AnimationActionCont::operator ==(AnimationActionCont other)
{
	return action == other.action && parameter == other.parameter;
}
