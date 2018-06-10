#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H
#include "stdafx.h"
#include "Resource.h"
#include "arm.h"
#include "blocks.h"

enum AnimationAction { Nothing = 0, HorizontalMove, VerticalMove, VerticalGrab, 
	VerticalLayDown, VerticalCheck };

struct AnimationActionCont
{
	AnimationAction action;
	REAL parameter, retVal;
	bool finished;

	// Does not compare return value and finished status
	bool operator ==(AnimationActionCont);
};

class Animation
{
public:
	Animation(const UINT, const double);

	// Basic actions and status checks
	void OnTick(HWND, Arm&, std::vector<Block>&, 
		std::queue<AnimationActionCont>& actions);
	void SetMaxSpeed(const double);
	void Start(HWND); 
	void UpdateSpeed(Arm&);

	// Manual control actions
	void OnKeydown(HWND, const WPARAM, Arm&);
	void OnKeyup(HWND, const WPARAM);

private:
	double alfaMove, betaMove, maxSpeed, alfaTarget, betaTarget;
	AnimationActionCont action;
	UINT timerId;
	bool isRunning;
	
	void Stop(HWND);
	void Move(HWND, const Arm&);

	void DecodeInRange(const Arm&);
	bool DecodeKeys(const WPARAM);
	double GetAlfaTarget(const PointF, const Arm&);
	double GetBetaTarget(const PointF, const Arm&);
};

#endif