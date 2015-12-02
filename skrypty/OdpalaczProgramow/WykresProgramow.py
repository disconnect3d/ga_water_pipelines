#written in python3.3.3 by Dominik Czarnota

from matplotlib import pyplot as plt
import os, random

testCasesDirs = "./TestCases/"
mapFile = "param.ini"
itemsList = []

def getMarker():
	markers = ['+', 'o', 's', 'x', 'D', '.', '^', 'v', '<', '>', '1','2','3','4', '*']
	i=0
	while 1:
		yield markers[i]
		i += 1
		if i+1 > len(markers):
			i = 0

def getColor():
	colors = ['r', 'g', 'b', 'k', 'y', 'm', 'c']
	i = 0
	j = 0
	while 1:
		yield colors[j]
		i += 1
		if i > 5:
			j += 1
			i = 0
		if j+1 > len(colors):
			j = 0

color = getColor()
marker = getMarker()

with open(mapFile) as fp:
	townsNumber = float(fp.readline().split()[0])

for dir in os.listdir(testCasesDirs):
	centerX = 0
	centerY = 0
	testFileNum = 0
	for file in os.listdir(testCasesDirs + dir):
		testFileNum += 1
		with open(testCasesDirs + dir + "/" + file) as fp:
			d = fp.readlines()
			#print("f=" + str(dir)+ str(file), d)
			data = tuple(map(float, d[-1].split()))
			print ("dupa=", data)
			centerX += data[1]
			centerY += data[0]
	centerY /= testFileNum
	centerX /= testFileNum
	centerY /= townsNumber
	centerY = 1. - centerY

	itemsList.append( (centerX, centerY, next(color), next(marker), dir) )
	#plt.plot(centerX, centerY, next(color), label=dir)
	print(centerX, centerY)
cmpFunc = lambda x, y: x[0]+0.1*x[1]>y[0]+0.1*y[1]

def cmp_to_key(mycmp):
    'Convert a cmp= function into a key= function'
    class K(object):
        def __init__(self, obj, *args):
            self.obj = obj
        def __lt__(self, other):
            return mycmp(self.obj, other.obj) < 0
        def __gt__(self, other):
            return mycmp(self.obj, other.obj) > 0
        def __eq__(self, other):
            return mycmp(self.obj, other.obj) == 0
        def __le__(self, other):
            return mycmp(self.obj, other.obj) <= 0  
        def __ge__(self, other):
            return mycmp(self.obj, other.obj) >= 0
        def __ne__(self, other):
            return mycmp(self.obj, other.obj) != 0
    return K


itemsList.sort(key=cmp_to_key(cmpFunc))
ax = plt.subplot(111)
for nr, i in enumerate(itemsList):
	#if nr < 20:
	ax.plot(i[0], i[1], color=i[2], marker=i[3], label=i[4])
	#else:
	#	break


box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.1, box.width, box.height * 0.9])

# Put a legend below current axis
ax.legend(loc='upper center', prop={'size':8}, bbox_to_anchor=(0.5, -0.05), fancybox=True, shadow=True, ncol=4)
plt.show()
