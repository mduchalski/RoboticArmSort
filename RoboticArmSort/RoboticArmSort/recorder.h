#pragma once
#ifndef RECORDER_H
#define RECORDER_H
#include "stdafx.h"
#include "blocks.h"
#include "animation.h"

class Recorder
{
public:
	Recorder();

	void StartRecording(const std::vector<Block>&, const Arm&);
	void StopRecording();
	void StartPlayback(HWND, Animation&, 
		std::queue<AnimationActionCont>&, std::vector<Block>&);
	void StopPlayback(std::queue<AnimationActionCont>&);
	void OnKeypush(const WPARAM, const Arm&, bool);

	bool IsPlaying() const;

private:
	std::vector<Block> blocksSnapshot;
	std::queue<AnimationActionCont> actionsBuffer;
	bool isRecording, isPlaying;
};

#endif