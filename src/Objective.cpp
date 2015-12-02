#include <stdexcept>
#include "Objective.h"

int   Objective::townsNumber	= 0;
Town* Objective::towns			= 0;
bool* Objective::townsWatered	= 0;

inline void Objective::ReadTowns(const char* fileName)
{
	std::ifstream fp;
	fp.exceptions(std::ios::failbit | std::ios::badbit);		// Jesli pliku nie ma to wyswietlamy blad
	fp.open(fileName); 
	
	fp >> townsNumber;
	towns = reinterpret_cast<Town*>( new char[townsNumber * sizeof(Town)] );
	for(int i=townsNumber-1; i>=0; --i)							// Zczytywanie miast
	{
		float x, y, radius;
		fp >> x;
		fp >> y;
		fp >> radius;
		new (towns + i) Town(x, y, radius);
	}
	townsWatered = new bool[townsNumber];
}

void Objective::CountFitness(Individual& ind)
{
	float x1, y1, x2, y2;
	float pipesLength = 0.f;
	float pipesCount = 0.f;

	std::fill_n(townsWatered, townsNumber, false);				// Inicjalizacja `townsWatered` wartoscia `false`

	for(int gene=MAX_PIPES_COUNT-1; gene>=0; --gene)			// Iteracja po rurociagach
	{
		if (ind._genotype[gene] == 0)
			continue;
		
		float firstVal = ind._genotype[gene] >> 16;
		float secondVal = ( ind._genotype[gene] << 16 ) >> 16;
		
		firstVal /= BITS_VALUE;									// Dzielenie w celu znormalizowania wyniku (zeby byl w zakresie [0, 1])
		secondVal /= BITS_VALUE;
		
		SIZE_T pointsDirection = ind._genotype[gene] % 6;
		
		switch (pointsDirection)								// Z ktorej sciany na ktora sciane tworzymy rurociag
		{
			case 0:					// LEFT <-> TOP
				x1 = 0.f;
				y1 = firstVal;
				x2 = secondVal;
				y2 = 0.f;
				break;
			
			case 1:					// LEFT <-> RIGHT
				x1 = 0.f;
				y1 = firstVal;
				x2 = 1.f;
				y2 = secondVal;
				break;

			case 2:					// LEFT <-> BOTTOM
				x1 = 0.f;
				y1 = firstVal;
				x2 = secondVal;
				y2 = 1.f;
				break;

			case 3:					// TOP <-> RIGHT 
				x1 = firstVal;
				y1 = 0.f;
				x2 = 1.f;
				y2 = secondVal;
				break;

			case 4:					// TOP <-> BOTTOM 
				x1 = firstVal;
				y1 = 0.f;
				x2 = secondVal;
				y2 = 1.f;
				break;

			case 5:					// RIGHT <-> BOTTOM
				x1 = 1.f;
				y1 = firstVal;
				x2 = secondVal;
				y2 = 1.f;
				break;

		}
		++pipesCount;

		/// pare makr w celu lepszej czytelnosci kodu...
		#define POW(x) std::pow((x), 2)
		#define SQRT(x) std::sqrt((x))
		#define ABS(x) std::abs((x))
		#define FLOAT(x) static_cast<float>((x))
		#define x0 towns[i].x
		#define y0 towns[i].y
		#define r towns[i].radius

		pipesLength += SQRT( POW(x2-x1) + POW(y2-y1) );
		for(int i=townsNumber-1; i>=0; --i)				// Iteracja po miastach i sprawdzanie czy rura ktora sprawdzamy, zasila dane miasto
		{	
			if ( !townsWatered[i] )						// Jesli jakas inna rura zasila juz to miasto - to nie sprawdzamy go ponownie
			{
				float D = ABS( x0 * (y2-y1) + (x1-x2) * y0 + x2*y1 - x1*y2 ) / SQRT( POW(y1-y2) + POW(x1-x2) );
				if (D <= r)
					townsWatered[i] = true;
			}
		}
	}

	// Obliczanie ilosci nawodnionych map, funkcji dostosowania oraz przypisanie dlugosci rurociagow
	ind.watered = std::count_if( townsWatered, townsWatered+townsNumber, [] (bool isTownWatered) { return isTownWatered == true; } );
	ind.fitness = ( (MAX_PIPES_LENGTH - pipesLength) / MAX_PIPES_LENGTH ) + 0.8f * ( FLOAT(ind.watered) / FLOAT(townsNumber) );
	ind.pipesLen = pipesLength;
}
