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

	struct CompiledFile
	{
		int id;
		int compileTime;
		int replTime;
		vector<int> vDependency;
	};

	struct Target
	{
		int id;
		int deadline;
		int goalPoint;
		vector<int> vDependency;
	};

	struct Server
	{
		int serverTime;
		vector<int> vCompFileAvailable;

		Server(size_t compFileCount);
	};

	vector<Server> vServers;
	vector<bool> vTargetSolved;

	vector<tuple<int, int, int> > vCompilation; //time, id, server
	
//static start
	static bool staticInited;
	static uint32_t idCounter;
	static int sServerCounter;
	static void initStatic(const Input& input);

	static map<string, int> sFileId;
	static map<int, string> sIdFileName;
	static vector<CompiledFile> sCompiledFile;
	static vector<Target> sTarget;


//static end
	Solution();
	
	void solve();

	pair<int, int> solveTargetOnOneServer(int targetId, int serverId) const;
	int applyTargetOnOneServer(int targetId, int serverId);
	bool step();
	string print() const;
	uint32_t getScore() const;
	uint32_t getRealScore() const;
	string getFileName() const;
};

bool Solution::staticInited = false;
uint32_t Solution::idCounter = 0;

std::map<std::string, int> Solution::sFileId;
std::map<int, std::string> Solution::sIdFileName;
std::vector<Solution::CompiledFile> Solution::sCompiledFile;
std::vector<Solution::Target> Solution::sTarget;
int Solution::sServerCounter;

Solution::Server::Server(size_t compFileCount)
{
	serverTime = 0;
	vCompFileAvailable.resize(compFileCount, -1);
}


void Solution::initStatic(const Input& input)
{
	staticInited = true;
	sServerCounter = input.S;
	sFileId = input.fileId;

	for (auto  tmp : sFileId)
	{
		sIdFileName[tmp.second] = tmp.first;
	}

	sCompiledFile.resize(input.vCompTime.size());
	forn(i, input.vCompTime.size())
	{
		sCompiledFile[i].id = i;
		sCompiledFile[i].compileTime = input.vCompTime[i];
		sCompiledFile[i].replTime = input.vReplTime[i];
		sCompiledFile[i].vDependency = input.vDep[i];
	}

	sTarget.resize(input.vTargetIds.size());

	forn(i, input.vTargetIds.size())
	{
		sTarget[i].id = input.vTargetIds[i];
		sTarget[i].deadline = input.vTargetDeadline[i];
		sTarget[i].goalPoint = input.vTargetGoalP[i];
	}

	//build target dependency

	forn(i, input.vTargetIds.size())
	{
		int currId = sTarget[i].id;
		vector<bool> vUsed(input.vCompTime.size());
		std::function<void(int)> dfs;
		dfs = [&](int v)
		{
			vUsed[v] = true;

			forn(k, sCompiledFile[v].vDependency.size())
			{
				int to = sCompiledFile[v].vDependency[k];

				if (!vUsed[to])
					dfs(to);
			}
			sTarget[i].vDependency.push_back(v);
		};
		dfs(currId);
	}
}

Solution::Solution()
{
	assert(staticInited);
	vServers.resize(sServerCounter, Server(sCompiledFile.size()));
	vTargetSolved.resize(sTarget.size());
}

void Solution::solve()
{
	while (step());
	sort(vCompilation.begin(), vCompilation.end(), [](const auto& a, const auto& b) 
			{ return get<0>(a) < get<0>(b); }
	);
}

pair<int, int> Solution::solveTargetOnOneServer(int targetId, int serverId) const
{
	const auto& actServer = vServers[serverId];
	const auto& actTarget = sTarget[targetId];
	int fId = actTarget.id;
	int minTime = actServer.serverTime;
	int usedTime = 0;
	for (auto task : actTarget.vDependency)
	{
		if (actServer.vCompFileAvailable[task] == -1 || minTime + sCompiledFile[task].compileTime < actServer.vCompFileAvailable[task])
		{
			minTime += sCompiledFile[task].compileTime;
			usedTime += sCompiledFile[task].compileTime;
		}
		else
		{
			usedTime += max(minTime - actServer.vCompFileAvailable[task], 0);
			minTime = max(actServer.vCompFileAvailable[task], minTime);

		}
	}
	return make_pair(minTime, usedTime);
}

int Solution::applyTargetOnOneServer(int targetId, int serverId)
{
	vTargetSolved[targetId] = true;
	auto& actServer = vServers[serverId];
	const auto& actTarget = sTarget[targetId];
	
	int fId = actTarget.id;
	int& minTime = actServer.serverTime;
	for (auto task : actTarget.vDependency)
	{
		if (actServer.vCompFileAvailable[task] == -1 || minTime + sCompiledFile[task].compileTime < actServer.vCompFileAvailable[task])
		{
			vCompilation.emplace_back(make_tuple(minTime, task, serverId));
			minTime += sCompiledFile[task].compileTime;
			//update other servers

			int tmp = minTime + sCompiledFile[task].replTime;
			for (auto& tmpS : vServers)
			{
				if (tmpS.vCompFileAvailable[task] == -1 || tmpS.vCompFileAvailable[task] > tmp)
				{
					tmpS.vCompFileAvailable[task] = tmp;
				}
			}
		}
	}
	return minTime;
}

bool Solution::step()
{
	int bestTarget = -1, bestServer = -1, bestRealScore = 0;
	double bestScore = 0;
	forn(tg, sTarget.size())
	{
		if (vTargetSolved[tg])
			continue;

		const auto actTarget = sTarget[tg];
		forn(srv, vServers.size())
		{
			auto tmp = solveTargetOnOneServer(tg, srv);

			int minTime = tmp.first;
			int usedTime = tmp.second;
		
			if (minTime > actTarget.deadline)
				continue;

			double score = actTarget.goalPoint + actTarget.deadline - minTime;
			score /= (sqrt(usedTime) * sqrt(minTime - vServers[srv].serverTime));

			if (score > bestScore)
			{
				bestTarget = tg;
				bestServer = srv;
				bestScore = score;
				bestRealScore = actTarget.goalPoint + actTarget.deadline - minTime;
			}
		}
	}

	if (bestScore == 0)
		return false;
	realScore += bestRealScore;
	applyTargetOnOneServer(bestTarget, bestServer);

	return true;
}

string Solution::print() const
{
	const string fname = getFileName();
	setActiveOutputFile(fname);
	printf("%d\n", vCompilation.size());
	for (auto compItem : vCompilation)
	{
		printf("%s %d\n", sIdFileName[get<1>(compItem)].c_str(), get<2>(compItem));
	}
	closeActiveOutputFile();
	return fname;
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
