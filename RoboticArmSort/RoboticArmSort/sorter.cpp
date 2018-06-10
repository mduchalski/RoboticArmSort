#include "sorter.h"
#include "RoboticArmSort.h"
#include "misc.h"

void Sorter::OnTick(std::queue<AnimationActionCont>& _actionsQueue,
	const Arm& _arm)
{
	if (!_actionsQueue.empty() && _actionsQueue.front().finished)
	{
		if (_actionsQueue.front().action == VerticalCheck)
		{
			blockHeights[i].second = _actionsQueue.front().retVal;
			i++;
		}

		_actionsQueue.pop();
	}
	if (_actionsQueue.empty())
	{
		if (isSorting)
			FinishSort(_actionsQueue, _arm);
		else
			isSorting = false;
	}
}

void Sorter::InitSort(HWND hWnd, std::queue<AnimationActionCont>& _actionsQueue,
	Animation& _animation, const std::vector<Block>& _blocks, const Arm& _arm)
{
	for (std::vector<Block>::const_iterator cit = _blocks.begin();
		cit != _blocks.end(); cit++)
	{
		blockHeights.push_back(std::pair<REAL, REAL>(cit->LeftLine(), 0.0f));
		_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
			MAX_BLOCK_HEIGHT - (_arm.EndLine() - _arm.EndPoint().Y), 
			0.0f, false });
		_actionsQueue.push(AnimationActionCont{ HorizontalMove, cit->LeftLine()
			+ BLOCK_WIDTH / 2.0f, 0.0f, false });
		_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
			MAX_BLOCK_HEIGHT - (_arm.EndLine() - _arm.EndPoint().Y),
			0.0f, false });
		_actionsQueue.push(AnimationActionCont{ VerticalCheck, 0.0f, 0.0f, false });
	}
	_animation.Start(hWnd);
	isSorting = true;
	i = 0;
}

void Sorter::FinishSort(std::queue<AnimationActionCont>& _actionsQueue, 
	const Arm& _arm)
{
	std::vector<std::pair<REAL, REAL> > temp = blockHeights;

	for (std::size_t i = 1; i < blockHeights.size(); i++)
		for (std::size_t j = 0; j < blockHeights.size() - i; j++)
			if (blockHeights[j + 1].second > blockHeights[j].second)
			{
				Swap(_actionsQueue, _arm, blockHeights[j].first + 0.5f * BLOCK_WIDTH,
					blockHeights[j + 1].first + 0.5 * BLOCK_WIDTH, 
					CenterLine() + BLOCKS_OFFSET + 2.5f * BLOCK_WIDTH);
				swap(blockHeights[j + 1], blockHeights[j]);
			}
}

void Sorter::Swap(std::queue<AnimationActionCont>& _actionsQueue, 
	const Arm& _arm, const REAL a, const REAL b, const REAL park)
{
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
		0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, a, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalGrab, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
		0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, park, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalLayDown, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
		0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, b, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalGrab, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
		0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, a, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalLayDown, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
		0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, park, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalGrab, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
		0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, b, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalLayDown, 0.0f, 0.0f, false });
}
