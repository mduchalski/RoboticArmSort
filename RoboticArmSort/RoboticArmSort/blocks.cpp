#include "blocks.h"
#include "stdafx.h"
#include "RoboticArmSort.h"
#include "misc.h"

// These functions calculate range of block X-posistions in whitch there is a
// guarantee that it will possible for the arm to reach each block
REAL GetLeftBound()
{
	return(MAX_BLOCK_HEIGHT - ARM_OFFSET) / (REAL)tan(asin(
		(double)((ARM_LEN_2 + MAX_BLOCK_HEIGHT - ARM_OFFSET) / ARM_LEN_1)));
}

REAL GetRightBound()
{
	return 2.0f * (REAL)sqrt(ARM_LEN_1 * ARM_LEN_1 - (ARM_LEN_2 +
		MAX_BLOCK_HEIGHT - ARM_OFFSET) * (ARM_LEN_2 + MAX_BLOCK_HEIGHT -
		ARM_OFFSET)) - GetLeftBound();
}

// Constructor for Block
Block::Block(REAL _relativePos, REAL _height)
{
	relativePos = _relativePos;
	height = _height;
	grabbedByArm = false;
}

void Block::Draw(Graphics& graphics) const
{
	if (!grabbedByArm)
		graphics.FillRectangle(&SolidBrush(Color::DarkBlue), LeftLine(), 
			HighLine(), BLOCK_WIDTH, height);
}

void Block::ToArm()
{
	grabbedByArm = true;
}
void Block::FromArm(REAL pos)
{
	relativePos = pos - CenterLine();
	grabbedByArm = false;
}

bool Block::IsMounted() const
{
	return grabbedByArm;
}
REAL Block::Height() const
{
	return height;
}

REAL Block::LeftLine() const
{
	return CenterLine() + relativePos;
}
REAL Block::RightLine() const
{
	return LeftLine() + BLOCK_WIDTH;
}
REAL Block::HighLine() const
{
	return ZeroLine() - height;
}

// Distrubutes blocks with random height evenly in the permitted area
std::vector<Block> RandBlocks()
{
	std::vector<Block> result;
	srand((unsigned int)time(NULL));

	for (REAL i = CenterLine() + BLOCKS_OFFSET + 1.25f * BLOCK_WIDTH;
		i < armBounds.X + armBounds.Width - BLOCK_WIDTH; 
		i += 1.25f * BLOCK_WIDTH)
		result.push_back(Block(i, MIN_BLOCK_HEIGHT + (REAL)
			(rand() % (int)(MAX_BLOCK_HEIGHT - MIN_BLOCK_HEIGHT - 20))));

	return result;
}
