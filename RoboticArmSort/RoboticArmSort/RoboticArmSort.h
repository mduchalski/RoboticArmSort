#pragma once
#include "resource.h"
#include "stdafx.h"
#include "blocks.h"
#include "arm.h"

extern const REAL SCALE, ARM_LEN_1, ARM_LEN_2, ARM_OFFSET, ARM_MAGNET_OFFSET,
	MIN_BLOCK_HEIGHT, MAX_BLOCK_HEIGHT, BLOCK_WIDTH, BLOCKS_OFFSET, GRAB_ERROR;
extern const UINT TIMER_DELAY;
extern const double ANIMATE_INCR;

extern double velocity;
extern HINSTANCE hInst;
extern HWND hwndSlider;
extern std::vector<HWND> hwndButtons;
extern RectF window, armBounds;
extern std::pair<double, double> angle, target, move;
extern std::vector<Block> blocks;
extern Arm arm;