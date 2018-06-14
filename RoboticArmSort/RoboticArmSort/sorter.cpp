#include "sorter.h"
#include "RoboticArmSort.h"
#include "misc.h"

// Interfacing with the queue on every tick
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
		{
			KillSort(_actionsQueue);
			isSorting = false;
		}
	}
}

// Initialize sorting
void Sorter::InitSort(HWND hWnd, std::queue<AnimationActionCont>& _actionsQueue,
	Animation& _animation, std::vector<Block>& _blocks, const Arm& _arm)
{
	blockHeights.clear();
	std::sort(_blocks.begin(), _blocks.end(), SortByPos);
	if (std::is_sorted(_blocks.begin(), _blocks.end(), SortByHeight))
		KillSort(_actionsQueue);

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

// Finish sorting, after all block heights have been determined
void Sorter::FinishSort(std::queue<AnimationActionCont>& _actionsQueue, 
	const Arm& _arm)
{
	for (std::size_t j = 0; j < blockHeights.size(); j++)
	{
		for (std::size_t k = 0; k < blockHeights.size() - 1; k++)
		{
			if (blockHeights[k].second > blockHeights[k + 1].second)
			{
				Swap(_actionsQueue, _arm, blockHeights[k].first + 0.5f * BLOCK_WIDTH,
					blockHeights[k + 1].first + 0.5 * BLOCK_WIDTH,
					CenterLine() + BLOCKS_OFFSET + 2.5f * BLOCK_WIDTH);

				swap(blockHeights[k + 1], blockHeights[k]);
				REAL temp = blockHeights[k + 1].first;
				blockHeights[k + 1].first = blockHeights[k].first;
				blockHeights[k].first = temp;
			}
		}
	}
}

// Generate actions sequence that swaps two blocks at positions a, b, using
// "parking" space park
void Sorter::Swap(std::queue<AnimationActionCont>& _actionsQueue, 
	const Arm& _arm, const REAL a, const REAL b, const REAL park)
{
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT,
		0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, a, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalGrab, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, park, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalLayDown, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, b, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalGrab, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, a, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalLayDown, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, park, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalGrab, 0.0f, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalMove, ZeroLine() -
		3.0f * MAX_BLOCK_HEIGHT + MIN_BLOCK_HEIGHT, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ HorizontalMove, b, 0.0f, false });
	_actionsQueue.push(AnimationActionCont{ VerticalLayDown, 0.0f, 0.0f, false });
}

// Abort sorting
void Sorter::KillSort(std::queue<AnimationActionCont>& _actionsQueue)
{
	isSorting = false;
	EnableWindow(hwndButtons[2], TRUE);
	//EnableWindow(hwndButtons[3], TRUE);
	EnableWindow(hwndButtons[4], TRUE);
	EnableWindow(hwndSlider, TRUE);
	while (!_actionsQueue.empty())
		_actionsQueue.pop();
}
