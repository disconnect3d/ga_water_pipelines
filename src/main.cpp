#include <iostream>
#include <ctime>
typedef unsigned int SIZE_T;	/// poniewaz size_t z <cstddef> niekoniecznie jest unsigned intem...
#include "Individual.cpp"
#include "Objective.cpp"
#include "Population.cpp"

using std::srand;
using std::time;
using std::ofstream;

const int popsize 	= 2000;
const int ngen 		= 50000;
float pmut 			= 0.4f;
float pcross 		= 0.4f;

const char* fileName = "param.ini";

int main(int argc, char* argv[])
{
	srand( time( static_cast<unsigned>(0) ) );

	if (argc > 1)
	{
		pmut = atof(argv[1]);
		pcross = atof(argv[2]);
	}
	Population* p = new Population(popsize, ngen, pmut, pcross, fileName);
	p->evolve();
	ofstream fp("czarnota.txt");
	fp << p->best;
	delete p;
	return 0;
}
