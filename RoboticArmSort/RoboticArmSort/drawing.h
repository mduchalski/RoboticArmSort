#pragma once
#ifndef DRAWING_H
#define DRAWING_H
#include "stdafx.h"

void DrawWideCLine(Graphics&, const Brush*, const PointF, const PointF, 
	const REAL);
void DrawMagnet(Graphics&, const Brush*, const PointF, const REAL, const REAL,
	const REAL);

#endif