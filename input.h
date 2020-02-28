#include "utils.h"

#pragma once

struct Input
{
	const string fileName = INPUTDIR + TASKNAME + INPUTEXT;

	Input()
	{
		setActiveInputFile(fileName);
		read();
		closeActiveInputFile();
	}
	void read()
	{
	}
};