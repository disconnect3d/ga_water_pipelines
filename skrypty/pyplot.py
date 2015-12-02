"""
Plots the solution found. Requires param.ini file and czarnota.txt to be in the same directory.
"""
import matplotlib.pyplot as plt

fig = plt.gcf()
towns = "param.ini"
output = "czarnota.txt"
circles = []
lines = []

with open(towns) as fp:
    data = fp.readlines()
    data = (i.split() for i in data[1:])
    for item in data:
        x, y, r = map(float, item)
        circles.append( plt.Circle((x,y), r, facecolor='none', edgecolor=(1,0,0), clip_on=False) )
    for circle in circles:
        fig.gca().add_artist(circle)

with open(output) as fp:
    data = fp.readlines()
    print(data[-1])
    data = (i.split() for i in data[1:-1])
    for nr, item in enumerate(data):
        x1, y1, x2, y2 = map(float, item)
        lines.append( plt.Line2D( (x1, x2), (y1, y2), color='b', linewidth=2, clip_on=False ) )
    for line in lines:
        fig.gca().add_artist(line)

plt.show()
