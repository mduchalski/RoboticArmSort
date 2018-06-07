#pragma once
#ifndef MISC_H
#define MISC_H
#include "stdafx.h"
#include "blocks.h"

PointF ArmPoint0();
PointF ArmPoint1();
PointF ArmPoint2();
bool InConflict(const std::vector<Block>&);

#endif