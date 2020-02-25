#include "utils.h"

#pragma once

struct Judge
{
	const string fileName;
	bool valid = false;

	Judge(const string& fname) : fileName(fname) 
	{
		
	}

	bool isValid() const
	{
		assert(false && "implement judge is valid");
		return valid;
	}

	uint32_t getScore() const 
	{
		assert(false && "implement judge get score");
		return 0;
	}
};
