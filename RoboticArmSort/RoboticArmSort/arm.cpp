#include "arm.h"
#include "stdafx.h"
#include "drawing.h"
#include "RoboticArmSort.h"
#include "misc.h"

Arm::Arm(const REAL _lenA, const REAL _lenB, const REAL _width,
	const Color _colorA, const Color _colorB)
{
	lenA = _lenA;
	lenB = _lenB;
	width = _width;
	colorA = _colorA;
	colorB = _colorB;

	alfa = M_PI / 4.0;
	beta = 0.0;
	grabbedHeight = 0.0f;
}

// Increment arm control angles by given values. Does not check for bounds!
void Arm::Increment(const double alfaIncr, const double betaIncr)
{
	alfa += alfaIncr;
	beta += betaIncr;
}

// Attempts grabbing the block, returns result
bool Arm::AttemptGrab(Block& block)
{
	if (std::abs(block.LeftLine() - MountLine()) < GRAB_ERROR
		&& abs(block.HighLine() - EndLine()) < GRAB_ERROR)
	{
		grabbedHeight = block.Height();
		block.ToArm();
		return true;
	}
	return false;
}

// Attempts laying down the block, returns result.
bool Arm::AttemptLayDown(Block& block)
{
	if (IsMounted() && block.IsMounted() &&
		std::abs(LowLine() - ZeroLine()) < GRAB_ERROR)
	{
		grabbedHeight = 0.0f;
		block.FromArm(MountLine());
		return true;
	}
	return false;
}

bool Arm::IsMounted()
{
	return grabbedHeight > 1.0f;
}

// Functions below determine three defining points of an arm
PointF Arm::MountPoint() const
{
	return PointF(PointF(CenterLine(), ZeroLine() - ARM_OFFSET));
}
PointF Arm::MiddlePoint() const
{
	return MountPoint() + PointF(lenA * (REAL)cos(alfa), -lenA * (REAL)sin(alfa));
}
PointF Arm::EndPoint() const
{
	return MiddlePoint() + PointF(lenB * (REAL)cos(beta), -lenB * (REAL)sin(beta));
}

// Functions below determine boundary lines of the arm
REAL Arm::ConnectionLine() const
{
	return EndPoint().Y + (MAX_BLOCK_HEIGHT - MIN_BLOCK_HEIGHT);
}
REAL Arm::EndLine() const
{
	return ConnectionLine() + grabbedHeight;
}
REAL Arm::MountLine() const
{
	return EndPoint().X - BLOCK_WIDTH / 2.0f;
}
REAL Arm::RightMountLine() const
{
	return MountLine() + BLOCK_WIDTH;
}
REAL Arm::LowLine() const
{
	return max(EndLine(), MiddlePoint().Y);
}
REAL Arm::HighLine() const
{
	return min(MiddlePoint().Y, EndPoint().Y);
}
REAL Arm::LeftLine() const
{
	return min(min(MiddlePoint().X, EndPoint().X), MountPoint().X);
}
REAL Arm::RightLine() const
{
	return max(max(MiddlePoint().X, EndPoint().X), MountPoint().X);
}

REAL Arm::Width() const
{
	return width;
}

bool Arm::InRect(const RectF rect)
{
	return LeftLine() > rect.X && RightLine() < rect.X + rect.Width &&
		HighLine() > rect.Y && EndLine() < rect.Y + rect.Height;
}

double Arm::Alfa() const
{
	return alfa;
}
double Arm::Beta() const
{
	return beta;
}
REAL Arm::LenA() const
{
	return lenA;
}
REAL Arm::LenB() const
{
	return lenB;
}


void DrawArm(Graphics& graphics, const Arm _arm)
{
	// Draw a "magnet" shaped arm mount
	DrawMagnet(graphics, &SolidBrush(_arm.colorA), _arm.MountPoint(),
		_arm.width / 2.0f, ARM_OFFSET, 3.0f * _arm.width);
	// Draw two arm parts
	DrawWideCLine(graphics, &SolidBrush(_arm.colorA), _arm.MiddlePoint(), 
		_arm.EndPoint(), _arm.width);
	DrawWideCLine(graphics, &SolidBrush(_arm.colorB), _arm.MountPoint(),
		_arm.MiddlePoint(), _arm.width);
	// Draw terminating magnet
	DrawMagnet(graphics, &SolidBrush(_arm.colorB), _arm.EndPoint(),
		min(BLOCK_WIDTH, _arm.width / 2.0f), 
		MAX_BLOCK_HEIGHT - MIN_BLOCK_HEIGHT, BLOCK_WIDTH);
	// Draw block if one is connected
	graphics.FillRectangle(&SolidBrush(Color::DarkBlue), _arm.MountLine(),
		_arm.ConnectionLine(), BLOCK_WIDTH, _arm.grabbedHeight);
}
