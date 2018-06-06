#include "blocks.h"
#include "stdafx.h"
#include "RoboticArmSort.h"
#include "misc.h"

// These functions calculate range of block X-posistions in whitch there is a
// guarantee that it will possible for the arm to reach each block
REAL GetLeftBound()
{
	return ArmPoint0().X + (MAX_BLOCK_HEIGHT - ARM_OFFSET) / (REAL)tan(asin(
		(double)((ARM_LEN_2 + MAX_BLOCK_HEIGHT - ARM_OFFSET) / ARM_LEN_1)));
}

REAL GetRightBound()
{
	return 2.0f * ((REAL)sqrt(ARM_LEN_1 * ARM_LEN_1 - (ARM_LEN_2 +
		MAX_BLOCK_HEIGHT - ARM_OFFSET) *  (ARM_LEN_2 + MAX_BLOCK_HEIGHT -
		ARM_OFFSET)) + ArmPoint0().X) - GetLeftBound();
}

// Constructor for Block structure
Block::Block(REAL _relativePos, REAL _height)
{
	relativePos = _relativePos;
	height = _height;
	mounted = false;
}

// Distrubutes blocks with random height evenly in the permitted area
std::vector<Block> RandBlocks()
{
	std::vector<Block> result;
	srand((unsigned int)time(NULL));

	for (REAL i = GetLeftBound() + 1.25f * BLOCK_WIDTH; i < GetRightBound(); 
		i += 1.25f * BLOCK_WIDTH)
		result.push_back(Block(i - ArmPoint0().X,
			MIN_BLOCK_HEIGHT + (REAL)(rand() % (int)(
				MAX_BLOCK_HEIGHT - MIN_BLOCK_HEIGHT))));

	return result;
}