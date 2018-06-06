#pragma once
#include "resource.h"
#include "stdafx.h"
#include "blocks.h"

extern const Color BASE_COLOR, ARM_COLOR_1, ARM_COLOR_2;
extern const REAL SCALE, ARM_LEN_1, ARM_LEN_2, ARM_OFFSET, ARM_MAGNET_OFFSET,
	MIN_BLOCK_HEIGHT, MAX_BLOCK_HEIGHT, BLOCK_WIDTH, GRAB_ERROR;
extern const double ANIMATE_INCR;

extern HINSTANCE hInst;
extern HWND hwndButton, hwndSlider;
extern RectF window;
extern std::pair<double, double> angle, target, move;
extern std::vector<Block> blocks;