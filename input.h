#include "utils.h"

#pragma once

struct Input
{
	const string fileName = INPUTDIR + TASKNAME + INPUTEXT;

	int C;
	int T;
	int S;

	vector<int> vCompTime;
	vector<int> vReplTime;
	vector<vector<int>> vDep;

	vector<int> vTargetIds;
	vector<int> vTargetDeadline;
	vector<int> vTargetGoalP;

	int nextId;

	map<string, int> fileId;


	Input()
	{
		nextId = 0;
		setActiveInputFile(fileName);
		read();
		closeActiveInputFile();
	}
	void read()
	{
		char tmpStr[15];
		scanf("%d %d %d", &C, &T, &S);
		vCompTime.resize(C);
		vReplTime.resize(C);
		vDep.resize(C);
		forn (st, C)
		{
			scanf("%s %d %d", tmpStr, &vCompTime[st], &vReplTime[st]);
			fileId[tmpStr] = nextId++;
			int n;
			scanf("%d", &n);
			forn(i, n)
			{
				scanf("%s", &tmpStr);
				vDep[st].push_back(fileId[tmpStr]);
			}	
		}
		int count = 0;
		vTargetDeadline.resize(T);
		vTargetGoalP.resize(T);

		forn(target, T)
		{
			scanf("%s %d %d", tmpStr, &vTargetDeadline[target], &vTargetGoalP[target]);
			vTargetIds.push_back(fileId[tmpStr]);
			if (vDep[vTargetIds.back()].size() == 0)
				++count;
		}
	}
};