#include "Servo.h"
#include "APTAPI.h"

Servo::Servo(long sn)
{
	mySN = sn;
	long err = InitHWDevice(mySN);
}

void Servo::step(long size)
{
	long err = MOT_MoveRelativeEx(mySN, ((float)size) /10, true);
}

void Servo::gotoAbsPos(long pos)
{
	long err = MOT_MoveAbsoluteEx(mySN, ((float)pos) / 1000, true);
}

void Servo::gotoHome()
{
	long err = MOT_MoveHome(mySN, true);
}
