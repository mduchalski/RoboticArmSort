#pragma once
#ifndef ARM_H
#define ARM_H
#include "blocks.h"
#include "stdafx.h"

class Arm
{
public:
	Arm(const REAL, const REAL, const REAL, const Color, const Color);

	// Basic actions
	void Increment(const double, const double);
	bool AttemptGrab(Block&);
	bool AttemptLayDown(Block&);
	bool IsMounted() const;
	
	// Determining dimensions
	PointF MountPoint() const;
	PointF MiddlePoint() const;
	PointF EndPoint() const;
	REAL ConnectionLine() const;
	REAL EndLine() const;
	REAL MountLine() const;
	REAL RightMountLine() const;
	REAL LowLine() const;
	REAL HighLine() const;
	REAL LeftLine() const;
	REAL RightLine() const;
	
	bool InRect(const RectF);

	double Alfa() const;
	double Beta() const;
	REAL LenA() const;
	REAL LenB() const;
	REAL Width() const;

	//friend bool InConflict(const Arm, const Block);
	friend void DrawArm(Graphics&, const Arm);

protected:
	Color colorA, colorB;
	REAL width, grabbedHeight;
	double alfa, beta;
	REAL lenA, lenB;
};

#endif