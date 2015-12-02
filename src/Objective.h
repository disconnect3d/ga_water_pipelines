#pragma once

#include "Individual.h"
#include <cmath>		// std::pow(), std::sqrt(), std::abs()
#include <algorithm>	// std::count_if(), std::fill_n()

const float BITS_VALUE = 65535.f;
const float MAX_PIPES_LENGTH = 14.14214f;	// 10 * sqrt(2)

/// Struktura reprezentujaca miasto
struct Town
{
	typedef float F;
	Town(F x, F y, F radius)
	: x(x), y(y), radius(radius)
	{}
	F x, y, radius;
};

struct Objective
{
	static int townsNumber;		/// Ilosc miast
	static Town* towns;			/// Tablica miast
	static bool *townsWatered;  /// Tablica wykorzystywana w obliczaniu fitnessa

	/// Zczytuje dane o miastach
	static void ReadTowns(const char* fileName);

	/// Liczy funkcje dostosowania osobnika
	static void CountFitness(Individual& ind);
};