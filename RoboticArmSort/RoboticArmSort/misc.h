#pragma once
#ifndef MISC_H
#define MISC_H
#include "stdafx.h"
#include "blocks.h"
#include "arm.h"

REAL ZeroLine();
REAL CenterLine();

RectF GetBoundingRect(const Arm&);

bool InConflict(const Arm&, const Block&);
bool InConflict(const Arm&, const std::vector<Block>&);

std::pair<bool, double> Bisection(double f(const double, const double, 
	const PointF, const Arm&), const double, const PointF, const Arm&,
	double, double, const double);

#endif