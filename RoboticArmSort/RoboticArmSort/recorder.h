#pragma once
#ifndef RECORDER_H
#define RECORDER_H
#include "stdafx.h"
#include "blocks.h"
#include "animation.h"

class Recorder
{
public:
	void StartRecording(const std::vector<Block>&);
	void StartPlayback(std::queue<AnimationActionCont>&);
	void OnKeypush(const Arm&);

private:
	std::vector<Block> blocksSnapshot;
	std::queue<AnimationActionCont> actionsBuffer;
	bool isRecording;
};

#endif