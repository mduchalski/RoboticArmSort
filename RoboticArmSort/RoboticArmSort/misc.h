#pragma once
#ifndef MISC_H
#define MISC_H
#include "stdafx.h"
#include "blocks.h"
#include "arm.h"

REAL ZeroLine();
REAL CenterLine();

bool InConflict(const Arm&, const Block&);
bool InConflict(const Arm&, const std::vector<Block>&);

#endif