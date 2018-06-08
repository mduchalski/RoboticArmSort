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

	// Manual control actions
	void OnKeydown(HWND, const WPARAM);
	void OnKeyup(HWND, const WPARAM);

	// Automatic constrol actions
	void GoToPos(const PointF);

private:
	double alfaMove, betaMove, maxSpeed;
	PointF target;
	UINT timerId;
	bool isRunning;

	void UpdateSpeed();
	bool AutomaticMode();
	bool DecodeKeys(const WPARAM);
};

#endif