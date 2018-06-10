#pragma once
#ifndef MISC_H
#define MISC_H
#include "stdafx.h"
#include "blocks.h"
#include "arm.h"

REAL ZeroLine();
REAL CenterLine();

RectF GetBoundingRect(const Arm&);
RectF GetClientRectAlt(HWND hWnd);

bool InConflict(const Arm&, const Block&);
bool InConflict(const Arm&, const std::vector<Block>&);

void AttemptGrab(Arm&, std::vector<Block>&);
void AttemptLayDown(Arm&, std::vector<Block>&);

bool SortByHeight(const std::pair<REAL, REAL>, const std::pair<REAL, REAL>);

#endif