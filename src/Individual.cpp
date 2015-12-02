#include "Individual.h"
#include "Objective.h"

Individual::Individual()
{
	std::fill_n(_genotype, MAX_PIPES_COUNT, 0);						// Wypelnienie zerami
	
	for(int gene=MAX_PIPES_COUNT-1; gene>=0; --gene)				// Losowanie bitow
		if (rand()%2 == 0)
		{
			for(size_t i=0; i<sizeof(int)*8; ++i)
				if ( rand()%2 == 0 )
					_genotype[gene] ^= 1 << i;
		}
}

std::ostream& operator<<(std::ostream& os, const Individual& ind)
{
	int pipesCount = std::count_if( ind._genotype, ind._genotype + MAX_PIPES_COUNT,			/// Obliczenie liczby rurociagow
								    [] (int genotype) { return genotype != 0? 1 : 0; } );

	os << pipesCount << std::endl;
	float x1, y1, x2, y2;
	for(int gene=MAX_PIPES_COUNT-1; gene>=0; --gene)
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

		os << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
	}
	os << ind.watered << " " << ind.pipesLen << std::endl;
	return os;
}
