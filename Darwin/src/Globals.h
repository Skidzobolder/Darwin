/*
 * Globals.h
 *
 *  Created on: 9-feb.-2012
 *      Author: woutergl
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()


#include "rapidxml.hpp"
#include "rapidxml_print.hpp"



enum GenomeType
{
	GENOME_TOKAMAK = 1, //steady state tokamak
	GENOME_TOKAMAKTIME = 2 //time dependent tokamak
};


enum NormalizationType
{
	NORM_PROPORTIONAL = 1,		// normalized fitness is based on proportional fitness
	NORM_RANK = 2,				// normalized fitness is based on rank
	NORM_EQUAL = 3				// normalized fitness is equal for all genomes

};

enum RecombinationType
{
	RECOMBINATION_RWS = 1,			//Roulette Wheel selection
	RECOMBINATION_SUS = 2,			//Stochastic Universal Sampling
	RECOMBINATION_TOURNAMENT = 3	//Tournament selection
};

enum EncodingType
{
	ENCODING_BINARY = 1,			//genes are encoded as binary numbers
	ENCODING_VALUE = 2,				//genes are encoded as numeric values
	ENCODING_TREE = 3,				//genes are encoded as a tree structure
	ENCODING_INTEGER = 4,			//genes are encoded as integer numbers
	ENCODING_ALPHANUM = 5			//genes are encoded as alphanumeric characters
};

struct Parent
{
    int nIndex;								//index in current population
    double dFitness;						//fitness
    double dNormalizedFitness;				//percentual chance that this parent is selected for reproduction
    double dAccumulatedNormalizedFitness;	//accumulated percentual chance that this parent is selected for reproduction

};

#endif /* GLOBALS_H_ */
