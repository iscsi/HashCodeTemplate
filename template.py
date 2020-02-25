import subprocess
import os
import sys
import multiprocessing
import tempfile
import shutil
import time
import random
from colorama import init, Fore, Back, Style

def PrintGreen(s):
	print("{}{}{}".format(Fore.GREEN,s, Style.RESET_ALL))

def PrintRed(s):
	print("{}{}{}".format(Fore.RED,s, Style.RESET_ALL))
	
def ConvexCombination(val1, val2, ratio):
	return val1*ratio + val2*(1.0-ratio)
	
def MyTriangleProb(val1, val2, mid):
	if random.getrandbits(1) :
		return random.triangular(val1, mid, mid)
	return random.triangular(mid, val2, mid)
	
class Optimizer:
	def __init__(self, executableName, fname, defaultParams, paramRanges):
		self.numberOFThread = max(multiprocessing.cpu_count() - 2,1 )
		self.fname = fname
		self.inputFileName = fname + '.in'
		self.outputFileName = fname + '.out'
		self.bestScore = 0
		self.bestParams = defaultParams
		self.paramRanges = paramRanges
		self.FNULL = open(os.devnull, 'w')
		self.executable = executableName

	def OptimizeParam(self, optIndex, numberOfTry):
		processes = []
		actParam = self.bestParams[:]
		actParamMin = self.paramRanges[optIndex][0]
		actParamMax = self.paramRanges[optIndex][1]
		for i in range(numberOfTry) :
			mi = open(self.inputFileName)
			mo = tempfile.NamedTemporaryFile()
			actParam[optIndex] = ConvexCombination(actParamMin, actParamMax, float(numberOfTry - i) / numberOfTry)
			args = "{} {}".format(self.executable, ' '.join(map(str, actParam)))
			newp = subprocess.Popen(args, stdin = mi, stdout = mo, stderr = subprocess.PIPE , shell=True)
			processes.append((newp, actParam[optIndex], args, mo))
			if len(processes) == self.numberOFThread or i == numberOfTry:
				for process , currP, currA, outfile in processes:
					process.wait()
					score = int(process.stderr.read())
					print("{} score: {} ".format(currA,score))
					if self.bestScore < score :
						self.bestParams[optIndex] = currP
						self.bestScore = score
						newBestFile = open("{}_{}{}.out".format(self.fname, score, currA.replace(" ", "_").replace(self.executable,"")), "w")
						outfile.seek(0)
						shutil.copyfileobj(outfile, newBestFile)
						newBestFile.close()
						PrintGreen("new best : bestP {}".format(' '.join(map(str, self.bestParams))))
				processes[:] = []
				
	def SA(self, calcTime):
		processes = []
		startTime = time.time()
		actParamMin = self.bestParams[:]
		actParamMax = self.bestParams[:]
		while time.time() < startTime + calcTime:
			currRatio = (startTime + calcTime - time.time())/calcTime
			for i in range(len(self.bestParams)):
				actParamMin[i] = ConvexCombination(self.paramRanges[i][0],  self.bestParams[i], min(currRatio, 0.85))
				actParamMax[i] = ConvexCombination(self.paramRanges[i][1],  self.bestParams[i], min(currRatio, 0.85))
			#choose a variable
			for i in range(self.numberOFThread):
				actParam = self.bestParams[:]
				for j in range(len(self.bestParams)):
					actParam[j] = MyTriangleProb(actParamMin[j], actParamMax[j], self.bestParams[j])
				mi = open(self.inputFileName)
				mo = tempfile.NamedTemporaryFile()
				args = "{} {}".format(self.executable, ' '.join(map(str, actParam)))
				newp = subprocess.Popen(args, stdin = mi, stdout = mo, stderr = subprocess.PIPE , shell=True)
				processes.append((newp, actParam, args, mo))
				if len(processes) == self.numberOFThread:
					for process , currP, currA, outfile in processes:
						process.wait()
						score = int(process.stderr.read())
						print("{} score: {} ".format(currA,score))
						if self.bestScore < score :
							self.bestParams = currP
							self.bestScore = score
							newBestFile = open("{}_{}{}.out".format(self.fname, score, currA.replace(" ", "_").replace(self.executable,"")), "w")
							outfile.seek(0)
							shutil.copyfileobj(outfile, newBestFile)
							newBestFile.close()
							PrintGreen("new best : bestP {}".format(' '.join(map(str, self.bestParams))))
					processes[:] = []
			 
	def PrintBest(self):
		PrintRed("bestParams {} bestScore: {} ".format(self.bestParams,self.bestScore))

init() # colorama init
oo = Optimizer("a.exe", "ghc03", [0.6, 1.0, 0.5], [(0.4,1.0),(0.5, 2.0),(0.2,2.0)])
#oo.OptimizeParam(2,50)
for i in range(20):
	oo.SA(90)
	oo.PrintBest()
