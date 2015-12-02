#pragma once
#include <iostream>
#include <cstdlib>		// rand(), srand()
#include <new>			// placement new
#include <fstream>		// std::ifstream
#include <cmath>		// std::sqrt, std::pow
#include <algorithm> 	// std::sort
#include "Individual.h"

const float ELITIST_PERCENTAGE = 0.1f;	/// Procent osobnikow, ktore zostana przeniesione do nastepnej generacji w przypadku "dobrej populacji" (ilosc generacji >= 3000) lub ktore zostana stworzone na nowo (w przypadku "slabej populacji")

struct Population
{
	/// Inicjalizuje populacje, zczytuje informacje o miastach, tworzy tablice townsWatered, ktora bedzie uzywana do fitnessa
	Population(int popsize, int ngen, float pmut, float pcross, const char* fileName);
	
	/// Niszczy populacje, sprzata po sobie w pamieci
	~Population();

	/// Przeprowadza wszystkie generacje
	void evolve();

	/// Przeprowadza kolejna generacje (wykorzystywane w evolve)
	void step();

	/// Najlepszy znaleziony osobnik
	Individual best;

protected:
	/// Selekcja
	void selection(bool weakPopulation=true);
	
	/// operatory genetyczne
	/// Krzyzowanie - zamienia losowe rurociagi dwoch osobnikow
	void crossover(Individual& ind1, Individual& ind2);
	/// Mutacja - tworzy lub niszczy losowy rurociag
	void mutation(Individual& ind);

	/// metoda sprawdzajaca najlepszego osobnika, potrzebna do sprawdzenia najlepszego osobnika na koncu algorytmu...
	void CheckForBest();

	/// Szukanie binarne osobnika do selekcji rankingowej
	Individual& BinarySearch(Individual** ptrs, float* individuals_fitting, float rnd) const;

	Individual* _pop;
	
	/// parametry algorytmu
	int		_popsize;
	int		_ngen;
	int 	_pmut;
	int		_pcross;
	int     _pextr;
	
	/// zmienne potrzebne do selekcji
	bool 	_firstSelection;
	int		_selectsize;
	float	_a;
	float	_k;
	float*	_selectionArray;

	/// Usuniecie konstruktorow zeby przypadkiem nie strzelic sobie gdzies w stope.
	Population(const Population& other) = delete;
	Population& operator=(const Population& other) = delete;
};
