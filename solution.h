#include "utils.h"
#include "input.h"

#pragma once

struct Solution 
{
	uint32_t id;
	uint32_t parentId = 0;
	bool valid = false;
	bool finalized = false;
	uint32_t realScore = 0;
	
//static start
	static bool staticInited;
	static uint32_t idCounter;
	static void initStatic(const Input& input);
//static end
	Solution();
	
	void solve();
	bool step();
	void print() const;
	uint32_t getScore() const;
	uint32_t getRealScore() const;
	string getFileName() const;
};

bool Solution::staticInited = false;
uint32_t Solution::idCounter = 0;

void Solution::initStatic(const Input& input)
{
	staticInited = true;
}
Solution::Solution()
{
	assert(staticInited);
}

void Solution::solve()
{
	while (step());
}

bool Solution::step()
{

	return true;
}

void Solution::print() const
{
	const string fname = getFileName();
	setActiveOutputFile(fname);

	closeActiveOutputFile();
}

uint32_t Solution::getScore() const
{
	return realScore;
}
uint32_t Solution::getRealScore() const
{
	return realScore;
}

std::string Solution::getFileName() const
{
	return OUTPUTDIR + TASKNAME + "_"+ to_string(realScore) + "_" + getTimeStampForFileName() + OUTPUTEXT;
}
