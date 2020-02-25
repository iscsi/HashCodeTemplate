import numpy as np
import matplotlib.pyplot as plt
from pylab import imshow, show, get_cmap, annotate
import string
from matplotlib.ticker import MaxNLocator # https://stackoverflow.com/questions/12050393/how-to-force-the-y-axis-to-only-use-integers-in-matplotlib

def simple_function_plot():
	x = [1,2,3]
	y = [5,7,4]

	x2 = [1,2,3]
	y2 = [10,14,12]
	
	plt.plot(x, y, label='First Line')
	plt.plot(x2, y2, label='Second Line')
	
	plt.xlabel('Plot Number')
	plt.ylabel('Important var')
	plt.title('Interesting Graph\nCheck it out')
	plt.legend()
	plt.show()
	
def simple_bar_plot():
	plt.bar([1,3,5,7,9],[5,2,7,8,2], label="Example one")

	plt.bar([2,4,6,8,10],[8,6,2,5,6], label="Example two", color='g')
	plt.legend()
	plt.xlabel('bar number')
	plt.ylabel('bar height')

	plt.title('Epic Graph\nAnother Line! Whoa')

	plt.show()
	
def simple_histo_plot():
	population_ages = [22,55,62,45,21,22,34,42,42,4,99,102,110,120,121,122,130,111,115,112,80,75,65,54,44,43,42,48]

	bins = [0,10,20,30,40,50,60,70,80,90,100,110,120,130]

	plt.hist(population_ages, bins, histtype='bar', rwidth=0.8)

	plt.xlabel('x')
	plt.ylabel('y')
	plt.title('Interesting Graph\nCheck it out')
	plt.legend()
	plt.show()

def simple_scatter_plot():	
	x = [1,2,3,4,5,6,7,8]
	y = [5,2,4,2,1,4,5,2]
	si = [20, 25, 330, 40, 50, 60, 980, 650]
	cmap=plt.get_cmap("Set1")
	#print(cmap)
	co = x
	plt.scatter(x,y, label='skitscat', c = co, cmap=cmap, s=si, marker="o")
	#plt.scatter(x,y, label='skitscat', cmap=cmap, s=si, marker="o", facecolors = 'none', edgecolors = ('r','b','g'))

	plt.xlabel('x')
	plt.ylabel('y')
	plt.title('Interesting Graph\nCheck it out')
	plt.legend()
	plt.show()
	
def simple_color_map():
	data = np.random.rand(10, 10) * 20

	# create discrete colormap
	cmap = get_cmap("Set1")
	#bounds = [0,1,2,3,4,5,6,7,8,9,10,20]
	#norm = colors.BoundaryNorm(bounds, cmap.N)

	fig, ax = plt.subplots()
	ax.imshow(data, cmap=cmap) #norm=norm)
	ax.annotate("ka", (4,7))
	ax.annotate("12", (7,1))

	# draw gridlines
	#ax.grid(which='major', axis='both', linestyle='-', color='k', linewidth=2)
	#ax.set_xticks(np.arange(-.5, 10, 1));
	#ax.set_yticks(np.arange(-.5, 10, 1));

	plt.show()
	
def read_from_file():
	inFileName = "dc.in"
	outFileName = "dc.out"
#https://medium.com/datadriveninvestor/python-reading-and-writing-data-from-files-d3b70441416e

	inFile = open(inFileName, "r")
	rowNum, slotSize, unavailableSlot, poolSize, serverNum = map(int,inFile.readline().split())
	print(inFile.readline())

	unavailableSlotPos = np.genfromtxt(inFileName, dtype=int, skip_header = 1, skip_footer = serverNum)

	serverDesc = np.genfromtxt(inFileName, dtype=int, skip_header = 1 + unavailableSlot)

	print(serverDesc)

	plt.hist(serverDesc[:,	1])
	#plt.hist(serverDesc[0,:])
	
	plt.show()

def draw_2018q_histo(histData, xTRange, yTRange, titleStr):
	bins = np.arange(0,xTRange+1, step=max(1,(xTRange)/10))
	plt.hist(histData, bins, histtype='bar')
	
	plt.xticks(np.arange(0,xTRange+1, step=max(1,(xTRange)/5)))
	plt.yticks(np.arange(0,yTRange+1, step=max(1,(yTRange)/5)))
	#plt.title(titleStr)
	#plt.show()
	

def draw_2018q():
	fileNumber = 5
	cmap=plt.get_cmap("Set1")
	plt.subplots_adjust(hspace = 1)
	subplotHeight = 3
	subplotWidth = 2
	for actfId in range(fileNumber):
		inFileName = "in/{}.in".format(string.ascii_lowercase[actfId])
		inFile = open(inFileName, "r")
		rowNum, colNum, F, N, B, T = map(int, inFile.readline().split())
		
		rides = np.genfromtxt(inFileName, dtype=int, skip_header = 1)

		for i in range(2):
			
			#rideLen = rides[:,0]

			#for j in range(len(rides[:,0])):
			#	rideLen[j] = abs(rides[j][5]-rides[j][4])
			#utak hossza	rideLen[j] = abs(rides[j][0]-rides[j][2]) + abs(rides[j][1]-rides[j][3])
			
			#print(rides[:,0])
			#plt.subplot(fileNumber,2,actfId * 2 + 1+ i)
			plt.subplot(subplotHeight,subplotWidth,i+1)
			plt.scatter(rides[:,2*i+1],rides[:,2*i+2], s= 5, label='start poziciok', marker=">")
			plt.xlim(0,colNum)
			plt.ylim(0,rowNum)
			plt.xticks(np.arange(0,colNum+1, step=max(1,colNum/5)))
			plt.yticks(np.arange(0,rowNum+1, step=max(1,rowNum/5)))	
			
		
		rideLen = rides[:,0]

		for j in range(len(rides[:,0])):
			#rideLen[j] = abs(rides[j][5]-rides[j][4])
			rideLen[j] = abs(rides[j][0]-rides[j][2]) + abs(rides[j][1]-rides[j][3])
		plt.subplot(subplotHeight,subplotWidth,3)
		draw_2018q_histo(rideLen, colNum+rowNum, N, 'ride hossza')
		plt.subplot(subplotHeight,subplotWidth,4)
		draw_2018q_histo(rides[:,4], T, N, 'start time')
		plt.subplot(subplotHeight,subplotWidth,5)
		draw_2018q_histo(rides[:,5], T, N, 'finish time')
		plt.subplot(subplotHeight,subplotWidth,6)
		draw_2018q_histo(rides[:,5]-rides[:,4], T, N, 'finish time')
		plt.show()
		
		#plt.hist(population_ages, bins, histtype='bar', rwidth=0.8)
		
def draw_2016q():
	fileNumber = 3
	cmap=plt.get_cmap("Set1")
	for actfId in xrange(1,fileNumber+1):
		inFileName = "out{}.txt".format(actfId)
		print(inFileName)
		inFile = open(inFileName, "r")
		rowNum, colNum = map(int, inFile.readline().split())
		wH, cN = map(int, inFile.readline().split())
		wHData = np.genfromtxt(inFileName, dtype=int, skip_header = 2, skip_footer = cN)
		cData = np.genfromtxt(inFileName, dtype=int, skip_header = 2 + wH)
		if actfId == 2:
			plt.scatter(wHData[0],wHData[1], c = 'red', s= 5, marker="x")
		else:
			plt.scatter(wHData[:,0],wHData[:,1], c = 'red', s= 5, marker="x")
		plt.scatter(cData[:,0],cData[:,1], c='blue', s= 5, marker="o")
		plt.show()

draw_2016q()
		

#simple_scatter_plot()
#simple_color_map()
#raw_input("Press enter to continue")
