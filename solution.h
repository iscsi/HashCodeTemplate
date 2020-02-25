#include "utils.h"
#include "input.h"

#pragma once

struct Solution 
{
	uint32_t id;
	uint32_t parentId;
	bool valid = false;
	bool finalized = false;

	void init();
	void solve();
	void step();
	void print() const;
	uint32_t getScore() const;
	uint32_t getRealScore() const;
	string getFileName() const;
};

void Solution::init()
{

}

void Solution::solve()
{

}

void Solution::step()
{

}

void Solution::print() const
{

}

uint32_t Solution::getScore() const
{
	assert(false);
	return 0;
}

uint32_t Solution::getRealScore() const
{
	assert(false);
	return 0;
}

std::string Solution::getFileName() const
{
	assert(false);
	return "";
}
