/*
 * Rosetta.cpp
 *
 *  Created on: 3-feb.-2012
 *      Author: woutergl
 */

#include "Rosetta.h"


void Rosetta::initGenome(){

	switch (m_eGenomeType)
	    {
	        case GENOME_TOKAMAK:
	           // std::cout << "Initializing steady state tokamak genome" << std::endl;
	            initTokamak();
	            break;
	        case GENOME_TOKAMAKTIME:
	           // std::cout << "Initializing time dependent tokamak genome" << std::endl;
	            initTokamak();
	            break;

	        default:
	            std::cout << "Unknown genome type" << std::endl;
	            break;
	    }


}

void Rosetta::initTokamak(){

	cGenome.newGenome("Tokamak steady state");

	cGenome.addChromosome("Auxiliary Heating Power");
	cGenome.addGene("1", "Auxiliary Heating Power");

	cGenome.addChromosome("Magnetic Field");

	std::string test("Magnetic Field");
	cGenome.addGene("1", test.c_str());

	cGenome.addChromosome("Plasma Density");
	cGenome.addGene("1", "Plasma Density");
	cGenome.addGene("2", "Plasma Density");
	cGenome.addGene("3", "Plasma Density");

/*	cGenome.addChromosome("Auxiliary Heating Power2");
	cGenome.addGene("1", "Auxiliary Heating Power2");

	cGenome.addChromosome("Magnetic Field2");
	cGenome.addGene("1", "Magnetic Field2");

	cGenome.addChromosome("Plasma Density2");
	cGenome.addGene("1", "Plasma Density2");*/
}

void Rosetta::CalcFitness(){

	int nLow = 0;
	int nHigh = 100;

	m_dFitness = (rand() % (nHigh - nLow + 1)) + nLow;

}

void Rosetta::SetGenome(const std::string& input_xml){
	//cGenome.SetGenomeXML(input_xml);
	cGenome.SetXML(input_xml);
}

