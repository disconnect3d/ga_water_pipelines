#written in python3.3.3 by Dominik Czarnota

import os, shutil
from subprocess import Popen, PIPE

testCasesDirs = "./TestCases/"
programsDir = "./Execs/"
mapFile = "param.ini"
OUTPUT_TXT = "czarnota.txt"
try:
	os.makedirs(testCasesDirs)
except:
	pass

def LaunchAll():        #moze byc jakis blad po przerabianiu z czytania z stdin...
	programsPaths = list(map(lambda x: programsDir+x if ".exe" in x else None, os.listdir(programsDir)))
	programsPaths.remove(None)

	for program in programsPaths:
		testDirName = program[program.rindex('/')+1:program.rindex('.')]
		try:
			os.makedirs(testCasesDirs+testDirName)
		except:
			continue
		for i in range(10):
			p = Popen(program)
			shutil.copy2("./" + testDirName + ".txt", testCasesDirs + testDirName + "/out" + str(i) + ".txt")

def LaunchOneWithParams(launchName, outputName, mut, xover):
	program = "./Execs/" + launchName
	for pmut in mut:
		for pcross in xover:
			testDirName = outputName + " m_" + str(pmut) + " x_" + str(pcross)
			print ("Creating: " + testDirName, end="")
			try:
				os.makedirs(testCasesDirs+testDirName)
			except FileExistsError:
				pass
			for i in range(10):
				print (" .", end="")
				with open(testCasesDirs + testDirName + "/out" + str(i) + ".txt", "w") as fp:
					program = program + " " + str(pmut) + " " + str(pcross)
					p = Popen(program, stdin=PIPE, stdout=PIPE)
					stdout, stderr = p.communicate()
					shutil.copy2("./" + launchName[:launchName.rindex('.')] + ".txt", testCasesDirs + testDirName + "/out" + str(i) + ".txt")
			print (" done")

MUTATIONS = (0.3, )
CROSSOVERS = (0.5, )
LaunchOneWithParams("czarnota.exe", "moje", MUTATIONS, CROSSOVERS)
