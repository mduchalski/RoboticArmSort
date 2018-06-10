#pragma once
#ifndef SORTER_H
#define SORTER_H
#include "stdafx.h"
#include "arm.h"
#include "blocks.h"
#include "animation.h"

class Sorter
{
public:
	void OnTick(std::queue<AnimationActionCont>&, const Arm&);
	void InitSort(HWND, std::queue<AnimationActionCont>&, Animation&, 
		const std::vector<Block>&, const Arm&);
	void Swap(std::queue<AnimationActionCont>&, const Arm&,
		const REAL, const REAL, const REAL);

private:
	std::vector<std::pair<REAL, REAL> > blockHeights;
	std::size_t i;
	bool isSorting;

	void FinishSort(std::queue<AnimationActionCont>&, const Arm&);
};

#endif