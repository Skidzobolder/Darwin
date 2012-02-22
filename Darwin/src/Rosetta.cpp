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

	cGenome.addChromosome("Auxiliary Heating Power", ENCODING_BINARY);
	cGenome.addGene("1", "Auxiliary Heating Power");
	cGenome.addGeneAttribute("Auxiliary Heating Power", "1", "bits", "64");

	cGenome.addChromosome("Magnetic Field", ENCODING_INTEGER);
	cGenome.addGene("1", "Magnetic Field");
	cGenome.addGeneAttribute("Magnetic Field", "1", "min", "0");
	cGenome.addGeneAttribute("Magnetic Field", "1", "max", "100");

	cGenome.addChromosome("Plasma Density", ENCODING_ALPHANUM);
	cGenome.addGene("1", "Plasma Density");
	cGenome.addGene("2", "Plasma Density");
	cGenome.addGene("3", "Plasma Density");


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

void Rosetta::randomGenome(){
	cGenome.FillWithRandomData();
}
