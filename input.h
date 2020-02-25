#include "utils.h"

#pragma once

struct Input
{
	const string fileName = INPUTDIR + TASKNAME + INPUTEXT;

	Input(){}

	bool readData()
	{
		assert(false && "implement input read");
		return true;
	}
};