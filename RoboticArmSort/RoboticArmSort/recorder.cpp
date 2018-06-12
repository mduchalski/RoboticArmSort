#include "recorder.h"
#include "RoboticArmSort.h"

Recorder::Recorder()
{
	isRecording = isPlaying = false;
}

void Recorder::StartRecording(const std::vector<Block>& _blocks, const Arm& _arm)
{
	isRecording = true;
	isPlaying = false;
	blocksSnapshot = _blocks;
	while (!actionsBuffer.empty())
		actionsBuffer.pop();
	actionsBuffer.push(
		AnimationActionCont{ DirectMove, (REAL)_arm.Alfa(), 
		(REAL)_arm.Beta(), false });
}

void Recorder::StopRecording()
{
	isRecording = false;
	EnableWindow(hwndButtons[2], TRUE);
}

void Recorder::StartPlayback(HWND hWnd, Animation& _animation,
	std::queue<AnimationActionCont>& _actionQueue, std::vector<Block>& _blocks)
{
	_blocks = blocksSnapshot;
	_actionQueue = actionsBuffer;
	_animation.Start(hWnd);
	isPlaying = true;
	EnableWindow(hwndButtons[3], TRUE);
}

void Recorder::StopPlayback(std::queue<AnimationActionCont>& _actionQueue)
{
	while (!_actionQueue.empty())
		_actionQueue.pop();
	isPlaying = false;
}

bool Recorder::IsPlaying() const
{
	return isPlaying;
}

void Recorder::OnKeypush(const WPARAM wParam, const Arm& _arm, bool armWasMounted)
{
	if (isRecording && (wParam == VK_DOWN || wParam == VK_UP || wParam == VK_LEFT
		|| wParam == VK_RIGHT))
		actionsBuffer.push(
			AnimationActionCont{ DirectMove, (REAL)_arm.Alfa(), 
			(REAL)_arm.Beta(), false });
	else if (isRecording && wParam == VK_SPACE && armWasMounted)
		actionsBuffer.push(
			AnimationActionCont{ VerticalLayDown, _arm.EndPoint().X, 0.0f, false });
	else if (isRecording && wParam == VK_SPACE)
		actionsBuffer.push(
			AnimationActionCont{ VerticalGrab, _arm.EndPoint().X, 0.0f, false });
}
