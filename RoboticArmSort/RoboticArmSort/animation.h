#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H
#include "stdafx.h"
#include "Resource.h"
#include "arm.h"
#include "blocks.h"

class Animation
{
public:
	Animation(const UINT, const double);

	// Basic actions and status checks
	void OnTick(HWND, Arm&, const std::vector<Block>&);
	void SetMaxSpeed(const double);
	void UpdateSpeed(Arm&);

	// Manual control actions
	void OnKeydown(HWND, const WPARAM, Arm&);
	void OnKeyup(HWND, const WPARAM);

	// Automatic control actions
	void Move(HWND, const Arm&, bool, REAL);

private:
	double alfaMove, betaMove, maxSpeed, alfaTarget, betaTarget;
	//PointF target;
	UINT timerId;
	bool isRunning, autoDirection;

	bool AutomaticMode();
	bool DecodeKeys(const WPARAM);
	double GetAlfaTarget(const PointF, const Arm&);
	double GetBetaTarget(const PointF, const Arm&);
};

#endif