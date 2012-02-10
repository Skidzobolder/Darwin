/*
 * Rosetta.h
 *
 *  Created on: 3-feb.-2012
 *      Author: woutergl
 */

#ifndef ROSETTA_H_
#define ROSETTA_H_

#include <iostream>

#include "Globals.h"
#include "Genome.h"


class Rosetta
{
private:
	GenomeType m_eGenomeType;
	Genome m_cGenome;
	double m_dFitness;
public:
    Rosetta() // default constructor
    {

    }

    void ModelToGenome();
    void GenomeToModel();

    void randomGenome();
    void initGenome();

    void initTokamak();

    void SetGenomeType(GenomeType eGenomeType) {m_eGenomeType = eGenomeType; }

    GenomeType GetGenomeType() { return m_eGenomeType; }
    double GetFitness() { return m_dFitness; }




};



#endif /* ROSETTA_H_ */
