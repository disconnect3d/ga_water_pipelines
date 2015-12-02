"""
Generates maps based on param.ini file
"""
import random as rnd


with open("param.ini", "w") as fp:
    townsNum = rnd.randint(10,20)
    fp.writelines(str(townsNum) + "\n")
    for i in range(townsNum):
        x = rnd.randint(0, 100)
        y = rnd.randint(0, 100)
        r = rnd.randint(1, 5)
        while ( (x+2*r)>100 or (x-2*r)<0 ):
            x = rnd.randint(0, 100)
        while( (y+2*r)>100 or (y-2*r)<0 ):
            y = rnd.randint(0, 100)
        x, y, r = map(lambda x: x/100., (x, y, r))
        x, y, r = map(str, (x,y,r))
        fp.writelines(x + " " + y + " " + r + "\n")
    
