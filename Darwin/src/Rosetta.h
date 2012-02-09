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
public:
    Rosetta(GenomeType eGenomeType) // default constructor
    {
    	m_eGenomeType = eGenomeType;
    }

    void ModelToGenome();
    void GenomeToModel();

    void randomGenome();
    void initGenome();

    void initTokamak();


    GenomeType GetGenomeType() { return m_eGenomeType; }
    //Genome GetGenome() { return m_cGenome; }


};



#endif /* ROSETTA_H_ */
