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
	cGenome.addGeneAttribute("Auxiliary Heating Power", "1", "bits", "16");
	cGenome.addGene("2", "Auxiliary Heating Power");
	cGenome.addGeneAttribute("Auxiliary Heating Power", "2", "bits", "16");
	cGenome.addGene("3", "Auxiliary Heating Power");
	cGenome.addGeneAttribute("Auxiliary Heating Power", "3", "bits", "16");

/*	cGenome.addChromosome("Magnetic Field", ENCODING_INTEGER);
	cGenome.addGene("1", "Magnetic Field");
	cGenome.addGeneAttribute("Magnetic Field", "1", "min", "-1000");
	cGenome.addGeneAttribute("Magnetic Field", "1", "max", "0");

	cGenome.addChromosome("Plasma Density", ENCODING_DOUBLE);
	cGenome.addGene("1", "Plasma Density");
	cGenome.addGeneAttribute("Plasma Density", "1", "min", "-1000");
	cGenome.addGeneAttribute("Plasma Density", "1", "max", "0");
	cGenome.addGeneAttribute("Plasma Density", "1", "decimals", "1");

	cGenome.addChromosome("Auxiliary Heating Power2", ENCODING_ALPHANUM);
	cGenome.addGene("1", "Auxiliary Heating Power2");
	cGenome.addGeneAttribute("Auxiliary Heating Power2", "1", "maxChars", "5");

	cGenome.addChromosome("Magnetic Field2", ENCODING_CUSTOM);
	cGenome.addChromosomeAttribute("Magnetic Field2", "chars", "ACGT");
	cGenome.addGene("1", "Magnetic Field2");
	cGenome.addGeneAttribute("Magnetic Field2", "1", "maxChars", "10");


	cGenome.addChromosome("Plasma Density2", ENCODING_TREE);
	cGenome.addGene("1", "Plasma Density2");

*/

}

void Rosetta::CalcFitness(){

	int nLow = 0;
	int nHigh = RAND_MAX;

	m_dFitness = (rand() % (nHigh - nLow + 1)) + nLow;


}

void Rosetta::SetGenome(const std::string& input_xml){
	//cGenome.SetGenomeXML(input_xml);
	cGenome.SetXML(input_xml);
}

void Rosetta::randomGenome(){
	cGenome.FillWithRandomData();
}

void Rosetta::DoMutations(MutationChances mutationChances){
	cGenome.DoMutations(mutationChances);

}
