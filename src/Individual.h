#pragma once
#include <iostream>
#include <cstdlib>
#include <new>
#include <fstream>
#include <algorithm>	// std::fill_n()

const int MAX_PIPES_COUNT = 10;	// Maksymalna liczba rurociagow

struct Individual
{
	/// Friend, zeby mieli dostep do _genotype[]
	friend std::ostream& operator<<(std::ostream& os, const Individual& i);
	friend struct Population;
	friend struct Objective;

	Individual();
	
	float fitness;		/// Funkcja dostosowania
	int watered;		/// Ilosc nawadnianych miast
	float pipesLen;		/// Dlugosc rurociagow
protected:
	SIZE_T 	_genotype[MAX_PIPES_COUNT];
};
