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
		setActiveInputFile(fileName);
		int E, S;
		char tmpStr[15];
		scanf("%d", &E);
		assert(0 < E && E <= inp.C * inp.S);
		
		vector<vector<int> > vServersAvail(inp.S, vector<int>(inp.C, -1));
		vector<int> vServerTimes(inp.S);

		forn(e, E)
		{
			scanf("%s %d", tmpStr, &S);
			assert(inp.fileId.count(tmpStr) == 1);
			assert(inp.S > S && S >= 0);

			int currId = inp.fileId.at(tmpStr);
			//check dependency
			bool ok = true;

			const vector<int>& vDep = inp.vDep[currId];

			int& serverT = vServerTimes[S];

			vector<int>& vAvail = vServersAvail[S];

			for (auto dep : vDep)
			{
				if (vAvail[dep] == -1)
				{
					assert(false);
				}
				serverT = max(serverT, vAvail[dep]);
			}
			serverT += inp.vCompTime[currId];
			vAvail[currId] = serverT;
			//copy to the other servers
			forn(i, inp.S)
			{
				int& tmp = vServersAvail[i][currId];
				if (tmp == -1 || tmp > vAvail[currId] + inp.vReplTime[currId])
					tmp = vAvail[currId] + inp.vReplTime[currId];
			}
		}

		//calculate score

		vector<int> vBestScore(inp.T);

		forn(i, inp.T)
		{
			const int targetId = inp.vTargetIds[i];
			const int targetDeadline = inp.vTargetDeadline[i];
			const int targetGoalP = inp.vTargetGoalP[i];
			int minTime = 1e9;
			forn(j, inp.S)
			{
				minTime = min(minTime, vServersAvail[j][targetId]);
			}
			if (minTime != -1 && minTime <= targetDeadline)
			{
				score += targetGoalP + targetDeadline - minTime;
			}
		}
		closeActiveInputFile();
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
