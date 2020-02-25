#include "utils.h"
#include "input.h"
#include "solution.h"
#include "judge.h"

int main(int argc, char** argv) 
{
	
	Input input;
	bool inputReadSucceded = input.readData();

	assert(inputReadSucceded == true);

	Solution sol;

	sol.init(); // vagy sol.copy(sol);
	sol.solve();

	const string solFileName = sol.getFileName();
	
	uint32_t solScore = sol.getRealScore();
	Judge judge(sol.getFileName());

	uint32_t score = judge.getScore();
	assert(score == solScore);

	//Archiver myArchive;

	//statTest();

	
	return 0;
}
