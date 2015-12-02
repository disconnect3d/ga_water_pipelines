"""
Put executable files in `programsDir` directory
Requires param.ini (map) file to work
"""

import os
from subprocess import Popen, PIPE

testCasesDirs = "./TestCases/"
programsDir = "./Execs/"
mapFile = "param.ini"
try:
	os.makedirs(testCasesDirs)
except:
	pass

def LaunchAll():
	programsPaths = list(map(lambda x: programsDir+x if ".exe" in x else None, os.listdir(programsDir)))
	programsPaths.remove(None)

	for program in programsPaths:
		testDirName = program[program.rindex('/')+1:program.rindex('.')]
		try:
			os.makedirs(testCasesDirs+testDirName)
		except:
			continue
		for i in range(10):
			with open(testCasesDirs + testDirName + "/out" + str(i) + ".txt", "w") as fp:
				p = Popen(program, stdout=PIPE, stderr=PIPE)
				try:
					stdout, stderr = p.communicate()
				except:
					try:
						print(p.communicate())
					except:
						pass
					try:
						stdout = p.communicate()
					except:
						pass
				fp.writelines(stdout.decode("utf-8"))

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
					p = Popen(program, stdout=PIPE, stderr=PIPE)
					stdout, stderr = p.communicate()
					fp.writelines(stdout.decode("utf-8"))
			print (" done")

MUTATIONS = (0.03, )
CROSSOVERS = (0.5, )
LaunchOneWithParams("czarnota.exe", "moje", MUTATIONS, CROSSOVERS)
