#include "utils.h"
#include "input.h"

#pragma once

struct Judge
{
	const string fileName;
	uint32_t score = 0;
	
	bool valid = false;

	Judge(const string& fname, const Input& inp) : fileName(fname) 
	{
		assert(false && "implement judge");
	}
	bool isValid() const
	{
		return valid;
	}

	uint32_t getScore() const 
	{
		return score;
	}
};
