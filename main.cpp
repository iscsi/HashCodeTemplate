#include "utils.h"
#include "input.h"
#include "solution.h"
#include "judge.h"

int main(int argc, char** argv) 
{
	
	Input input;

	Solution::initStatic(input);
	Solution sol;

	sol.solve();
	sol.print();

	const string solFileName = sol.getFileName();
	
	uint32_t solScore = sol.getRealScore();
	Judge judge(sol.getFileName(), input);

	uint32_t score = judge.getScore();
	assert(score == solScore);

	LOG("task: " << TASKNAME << " ,score:" << score);
	//Archiver myArchive;

	//statTest();

	
	return 0;
}
