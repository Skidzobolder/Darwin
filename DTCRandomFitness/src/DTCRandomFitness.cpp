//============================================================================
// Name        : DTCRandomFitness.cpp
// Author      : Wouter Glorieux
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
	int nLow = 0;
	int nHigh = RAND_MAX;
	int nRandom = 0;
	nRandom =  (rand() % (nHigh - nLow + 1)) + nLow;

	return nRandom;
}
