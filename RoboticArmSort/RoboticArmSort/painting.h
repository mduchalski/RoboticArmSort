#pragma once
#ifndef PAINTING_H
#define PAINTING_H
#include "blocks.h"

void PaintGui(HDC);
void OnPaint(HDC);
void PaintArm(Graphics&);
void PaintBlocks(Graphics&, const std::vector<Block>&);

#endif