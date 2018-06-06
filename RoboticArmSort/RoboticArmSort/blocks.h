#pragma once
#ifndef BLOCKS_H
#define BLOCKS_H
#include "stdafx.h"

REAL GetLeftBound();
REAL GetRightBound();

class Block
{
public:
	Block(REAL, REAL);

	REAL relativePos, height;
	bool mounted;
};


std::vector<Block> RandBlocks();

#endif