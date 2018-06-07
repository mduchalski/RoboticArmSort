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

	void Draw(Graphics&) const;
	void ToArm();
	void FromArm(REAL);

	bool IsMounted() const;
	REAL Height() const;

	REAL LeftLine() const;
	REAL RightLine() const;
	REAL HighLine() const;

private:
	REAL height, relativePos;
	bool grabbedByArm;
};

std::vector<Block> RandBlocks();


#endif