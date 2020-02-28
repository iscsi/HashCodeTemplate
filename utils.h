#include <bits/stdc++.h>
#include <windows.h>
#include <random>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include "config.h"

#pragma once

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define for1(i, n) for (int i = 1; i <= (int)(n); ++i)
#define ford(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define fore(i, a, b) for (int i = (int)(a); i <= (int)(b); ++i)

template<class T> bool umin(T &a, T b) { return a > b ? (a = b, true) : false; }
template<class T> bool umax(T &a, T b) { return a < b ? (a = b, true) : false; }

using namespace std;

string getTimeStampForFileName()
{
	std::time_t t = std::time(nullptr);
	std::ostringstream tmpss;
	tmpss << put_time(localtime(&t), "%F_%H_%M_%S");
	string res = tmpss.str();
	replace(res.begin(), res.end(), '-', '_');
	return res;
}

//Logger start

enum LOGCOLOUR
{
	GREY = 8,
	BLUE, //=FOREGROUND_BLUE,//1
	GREEN, // = FOREGROUND_GREEN,//2
	TURQUISE, // = FOREGROUND_BLUE | FOREGROUND_GREEN,
	RED, // = FOREGROUND_RED,//4
	PURPLE, //= FOREGROUND_RED | FOREGROUND_BLUE,//5
	YELLOW,// = FOREGROUND_RED | FOREGROUND_GREEN,//6,
	WHITE,//7
};

struct LOGGER
{
	std::ostringstream mOs;
	
	LOGGER() { mOs.precision(10); }
	virtual ~LOGGER()
	{
		mOs << "\n";
		std::ofstream logFile;
		logFile.open(LOGGERFNAME, std::ofstream::out | std::ofstream::app);
		logFile << mOs.str();
		logFile.close();
	}
	std::ostringstream& Get(LOGCOLOUR col = WHITE)
	{
		std::time_t t = std::time(nullptr);
		mOs << put_time(localtime(&t), "--%F %H:%M:%S - ");//__TIMESTAMP__
		return mOs;
	}
};

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define LOGC(colour_, message_)                  \
    LOGGER().Get(colour_)                       \
    << __FILENAME__ << ":" << __LINE__ <<" --- " << message_;\

#define LOG(message_) LOGC(LOGCOLOUR::WHITE, message_)

//Logger end

//random start

std::mt19937 rng(0x15c51);

template<typename T>
T getRandom(T from, T to)
{
	std::uniform_int_distribution<T> distribution(from, to);
	return distribution(rng);
}

template<>
double getRandom(double from, double to)
{
	std::uniform_real_distribution<double> distribution(from, to);
	return distribution(rng);
}

//random end

//stat start

template<typename T>
void printStat(const vector<T>& _data, string& _name)
{
	T sum = static_cast<T>(0);
	T mi = numeric_limits<T>::max();
	T mx = numeric_limits<T>::min();
	double avg = 0;
	for (int i = 0; i < _data.size(); ++i)
	{
		sum += _data[i];
		avg += _data[i];
		umax(mx, _data[i]);
		umin(mi, _data[i]);
	}
	if (std::abs(static_cast<double>(sum) - avg) > 1e-9)
	{
		LOGC(RED, "tulcsordulas : " << _name);
		assert(false);
		return;
	}
	if (_data.empty())
	{
		LOGC(RED, "ures adat : " << _name);
		return;
	}
	LOGC(GREEN, _name << ":dbszam: " << _data.size() << \
		";min: " << mi << 
		";max: " << mx << 
		";avg: " << avg / _data.size() << 
		";sum: " << sum;
	);
}

template<typename VT, typename FUNC>
void printStat(const vector<VT>& _data,const FUNC& _func, string& _name)
{
	vector<result_of_t<FUNC(VT)> > v;
	forn(i, _data.size())
	{
		const VT& act = _data[i];
		v.push_back(_func(act));
	}
	printStat(v, _name);
}

void statTest()
{
	vector<int> v(100);
	forn(i, v.size())
	{
		v[i] = getRandom(0, 100);
	}
	printStat(v, string("v test"));
	vector<pair<double, double> > w(200);
	forn(i, w.size())
	{
		w[i].first = getRandom(0.0, 100.0);
	}
	printStat(w, [](const pair<double, double>& a) {return a.first; }, string("w test"));
}

//stat end

//archiver start

struct Archiver 
{
	string mVersion;
	
	string getFileCurrentIdentity(const string& filename)
	{
		HANDLE hFile;
		FILETIME ftModified, ftModifiedLocal;
		SYSTEMTIME stLocal;
		hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			LOG(RED, "nincs meg a file : " << filename);
			return "";
		}

		if (!GetFileTime(hFile, NULL, &ftModified, NULL))
		{
			LOG(RED, "hiba az utolso modositas lekerdezesenel : " << filename);
			return FALSE;
		}
		FileTimeToLocalFileTime(&ftModified, &ftModifiedLocal);
		FileTimeToSystemTime(&ftModifiedLocal, &stLocal);
		ostringstream os;
		os << stLocal.wYear << "_" << 
		setfill('0') << setw(2) << stLocal.wMonth << "_" <<
		setfill('0') << setw(2) << stLocal.wDay << "_" << 
		setfill('0') << setw(2) << stLocal.wHour << "_" <<
		setfill('0') << setw(2) << stLocal.wMinute << "_" <<
		setfill('0') << setw(2) << stLocal.wSecond;
		return os.str();
	}

	void SetVersion()
	{
		mVersion.clear();
		for (auto actFileName : ARCHIVERFILES)
		{
			umax(mVersion, getFileCurrentIdentity(ARCHIVERDIR + actFileName));
		}
	}

	bool CheckVersionAlreadyExists()
	{
		struct stat info;
		if (stat("Archive", &info) != 0)
		{
			CreateDirectory("Archive", NULL);
		}
		string nd = ("Archive/" + mVersion);
		if (stat(nd.c_str(), &info) != 0)
		{
			CreateDirectory(nd.c_str(), NULL);
			LOGC(GREEN, "new version : " << nd << " created");
			for (auto actFileName : ARCHIVERFILES)
			{
				string from = ARCHIVERDIR + actFileName;
				string to = nd + "/" + actFileName;
				if (!CopyFile(from.c_str(), to.c_str(), TRUE))
				{
					LOGC(RED, "copy failed: " << to );
				}
				else
				{
					LOGC(GREEN, "new version : " << actFileName << " saved");
				}
			}
		}
		else
		{
			LOGC(GREEN, "version : " << nd << " already exists");
		}
		return true;
	}

	Archiver()
	{
		SetVersion();
		CheckVersionAlreadyExists();
	}
};

//archiver end

//Param start

template<typename T>
void ReadParam(int argc, char** argv, uint32_t pos, T& val)
{
	if (pos > 0 && pos < argc)
	{
		stringstream ss(argv[pos]);
		ss >> val;
		LOGC(GREEN, "param readed pos: " << pos << " ; val: " << val);
	}
	else
	{
		LOGC(RED, "param read error: " << pos);
	}
}

//param end

//time measurement start

/*  //time measurement
	using std::chrono::system_clock;

	auto start = system_clock::now();
	auto end = system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cerr << diff.count() << '\n';

*/

//time measurement end

//set/unset active files

string activeInputFile;
string activeOutputFile;
string activeErrorFile;

void setActiveInputFile(const string& fname)
{
	assert(activeInputFile.empty());
	activeInputFile = fname;
	freopen(activeInputFile.c_str(), "rb", stdin);
}

void closeActiveInputFile()
{
	assert(!activeInputFile.empty());
	activeInputFile.clear();
	fclose(stdin);
}

void setActiveOutputFile(const string& fname)
{
	assert(activeOutputFile.empty());
	activeOutputFile = fname;
	freopen(activeOutputFile.c_str(), "wb", stdout);
}

void closeActiveOutputFile()
{
	assert(!activeOutputFile.empty());
	activeOutputFile.clear();
	fclose(stdout);
}

void setActiveErrorFile(const string& fname)
{
	assert(activeErrorFile.empty());
	activeErrorFile = fname;
	freopen(activeErrorFile.c_str(), "wb", stderr);
}

void closeActiveErrorFile()
{
	assert(!activeErrorFile.empty());
	activeErrorFile.clear();
	fclose(stderr);
}

//set/unset active filesend

// https://stackoverflow.com/questions/15719084/how-to-use-lambda-function-as-hash-function-in-unordered-map

	//auto my_hash = [](const pair<int, int>& a) {return a.first^a.second; };
	//unordered_map<pair<int, int>, int, decltype(my_hash)> mMap(10, my_hash);
	//mMap[{ 10, 20 }] = 7;
	//mMap[{ 30, 20 }] = 14;

