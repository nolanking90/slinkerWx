#pragma once
class Servo
{
public:
	Servo(long sn);
	void step(long size);
	void gotoAbsPos(long pos);
	void gotoHome();
	long mySN;


};

