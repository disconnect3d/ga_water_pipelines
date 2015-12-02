#include "Population.h"

Population::Population(int popsize, int ngen, float pmut, float pcross, const char* fileName)
: _popsize(popsize), _ngen(ngen), _pmut(pmut*100), _pcross(pcross*100),
_firstSelection(true),
_selectsize(static_cast<int>((1.f-ELITIST_PERCENTAGE) * _popsize)),
_a(ELITIST_PERCENTAGE/_selectsize),
_k((1.f-ELITIST_PERCENTAGE)/_selectsize)
{
	_pop = new Individual[_popsize];
	Objective::ReadTowns(fileName);
	_selectionArray = new float[_popsize];
}

Population::~Population()
{
	delete [] _pop;
	delete [] _selectionArray;
}

void Population::evolve()
{
	clock_t start = std::clock();
	float passed;
	int i=0;
	for(; i<_ngen; ++i)						// Odpalenie ngen generacji + sprawdzanie czasu stopu			
	{
		step();
	 	passed =  (float) ( std::clock() - start ) / CLOCKS_PER_SEC;
	 	if (passed >= 175) break;
	 	if (!(i % 100)) std::cout << "ngen=" << i <<", t=" << (passed) << "s" << std::endl;
	}
	passed = (float) ( std::clock() - start ) / CLOCKS_PER_SEC;
	std::cout << "Wygenerowano " << i << " pokolen, w czasie " << passed << " s" << std::endl;
	CheckForBest();

}

void Population::CheckForBest()
{
	Individual** ptrs = new Individual*[_popsize];

	for(int i=_popsize-1; i>=0; --i)					// Obliczanie fitnessu osobnikow
	{
		ptrs[i] = &_pop[i];
		Objective::CountFitness(_pop[i]);
	}

	std::sort(ptrs, ptrs+_popsize,						// Sortowanie wskaznikow wg funkcji dostosowania
			  [] (const Individual* i1, const Individual* i2) { return i1->fitness > i2->fitness; } );		

	if (ptrs[0]->fitness > best.fitness)				// Przypisanie najlepszego
		best = *ptrs[0];

	delete [] ptrs;
}

void Population::step()
{
	if (_ngen < 3000)									// Jesli liczba generacji < 150 to odpalamy selekcje z "weakPopulation", wtedy zamiast przepisywac osobniki, tworzymy nowe.
		selection(true);
	else
		selection(false);

	for(int i=0; i<_popsize-1; i+=2)					// Przeprowadzamy mutacje, krzyzowanie, ekstrakcje
	{
		if (rand()%100 <= _pmut)
			mutation(_pop[i]);
		if (rand()%100 <= _pmut)
			mutation(_pop[i+1]);
		if (rand()%100 <= _pcross)
			crossover(_pop[i], _pop[i+1]);
	}
}

/* SELEKCJA RANKINGOWA - Pr(A) = a + k * ( 1 - r(A) / r_max ), gdzie:
	a = 0.1f / rozmiar populacji (ktora jest podejmowana selekcji)
	k = 0.9f / rozmiar populacji (ktora jest podejmowana selekcji)
	A  		- osobnik
	r(A) 	- jego ranga (sortujemy tak, ze najlepszy osobnik ma range 0, a najgorszy n-1)
	r_max 	- najwyzsza ranga
*/
void Population::selection(bool weakPopulation)
{
	Individual** ptrs = new Individual*[_popsize];
	for(int i=_popsize-1; i>=0; --i)					// Obliczanie funkcji dostosowania osobnikow, w celu posortowania
	{
		ptrs[i] = &_pop[i];
		Objective::CountFitness(_pop[i]);
	}
	
	std::sort( ptrs, ptrs+_popsize,						// Sortowanie wskaznikow wg funkcji dostosowania
			   [] (const Individual* i1, const Individual* i2) { return i1->fitness > i2->fitness; } );

	if (ptrs[0]->fitness > best.fitness)				// Po sortowaniu na pierwszym miejscu tablicy ptrs jest najlepszy osobnik
		best = *ptrs[0];
	
	_selectionArray[0] = _a + _k;
	for(int i=1; i<_popsize-1; ++i)						// Tworzenie tablicy rang
		_selectionArray[i] = _a + _k * (1.f - FLOAT(i) / FLOAT((_selectsize-1.f)) ) + _selectionArray[i-1];
	_selectionArray[_popsize-1] = 1.f;
	
	Individual* selectedPopulation = reinterpret_cast<Individual*>( new char[sizeof(Individual) * _popsize] );

	if (weakPopulation)									// Gdy populacja jest slaba, to troche osobnikow zastepujemy nowymi
		for(int i = 0; i < _popsize - _selectsize; ++i)
			new (selectedPopulation+i) Individual();
	else												// W innym wypadku bierzemy troche najlepszych osobnikow
		for(int i = 0; i < _popsize - _selectsize; ++i)
			new (selectedPopulation+i) Individual(*ptrs[i]);

	for(int i=_popsize-_selectsize; i<_popsize; ++i)	// Wybieranie reszty osobnikow do nowej populacji na podstawie selekcji rankingowej
	{
		float selected = static_cast<float>(rand()%10000) / 10000.f;
		new (selectedPopulation+i) Individual(BinarySearch(ptrs, _selectionArray, selected));
	}
	

	if (!_firstSelection)								// W inicjalizacji inicjalizujemy domyslnym konstruktorem, a w kazdej kolejnej przypisujemy populacje do `selectedPopulation`, stad usuwanie w ten sposob
	{
		delete [] reinterpret_cast<char*>(_pop);
		_firstSelection = false;
	}
	else												// Usuwanie w pierwszym wykorzystaniu selekcji
		delete [] _pop;

	_pop = selectedPopulation;
	delete [] ptrs;
}


Individual& Population::BinarySearch(Individual** ptrs, float* individuals_fitting, float rnd) const
{
    if (rnd <= individuals_fitting[0])					// Jesli prawdopodobienstwo jest mniejsze/rowne niz pierwszy fitness, to zwracamy pierwszego osobnika
		return *ptrs[0];
    int imin = 0;
    int imax = _popsize-1;
    int index = (imax+imin)/2;
    while (imax >= imin)								// Petla w ktorej szukamy osobnika do zwrocenia
    {
		
        if ( (individuals_fitting[index] < rnd) && (individuals_fitting[index+1] < rnd) )
            imin = index+1;
        else if ( (individuals_fitting[index] < rnd) && (individuals_fitting[index+1] >= rnd) )
        {
            index += 1;
            break;
        }
        else if ( (individuals_fitting[index] > rnd) && (individuals_fitting[index-1] > rnd) )
            imax = index-1;
        else if ( (individuals_fitting[index] >= rnd) && (individuals_fitting[index-1] < rnd) )
            break;
       	else if (individuals_fitting[index-1] == rnd)
       	{
       		--index;
       		break;
       	}
        else
        {}
		index = (imax+imin)/2;
	}

	return *ptrs[index];
}


void Population::crossover(Individual& ind1, Individual& ind2)
{
	for(int gene=0; gene<MAX_PIPES_COUNT/2; ++gene)		// Zamiana pierwszej polowy liczb z tablicy rurociagow
	{
		SIZE_T tmp = ind1._genotype[gene];
		ind1._genotype[gene] = ind2._genotype[gene];
		ind2._genotype[gene] = tmp;
	}
}


void Population::mutation(Individual& ind)
{
	switch (rand()%2)									// losowanie czy usunac, czy dodac rurociag
		{
		case 0:	
		{												// Usuwanie rurociagu
			int i=0;
			while(true)									// Szukanie miejsca gdzie usunac rurociag
			{
				int rnd = rand()%MAX_PIPES_COUNT;
				if(ind._genotype[rnd] != 0)				// Usuwanie wylosowanego rurociagu
				{
					ind._genotype[rnd] = 0;
					return;
				}
				++i;
				if (i>8)								// Jesli przez 8 prob nie znaleziono rurociagu do usuniecia, to rezygnujemy
					return;
			}
			break;
		}
		case 1:											// Dodawanie rurociagu
			for(int gene=0; gene<MAX_PIPES_COUNT; ++gene)		// Szukanie indeksu gdzie nie ma rurociagu
			{
				if (ind._genotype[gene] == 0)
				{
					for(int i=0; i<32; ++i)						// Tworzenie rurociagu
					{
						if (rand()%2 == 0)
							ind._genotype[gene] |= 1 << i;
					}
					return;
				}
			}
			break;
	}
}