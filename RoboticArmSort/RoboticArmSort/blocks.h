#pragma once
#ifndef BLOCKS_H
#define BLOCKS_H
#include "stdafx.h"

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

public:
	REAL height, relativePos;
	bool grabbedByArm;
};

void RandBlocks(std::vector<Block>&);

#endif