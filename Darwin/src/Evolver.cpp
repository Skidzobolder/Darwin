/*
 * Evolver.cpp
 *
 *  Created on: 3-feb.-2012
 *      Author: Skidzobolder
 */

#include "Evolver.h"




void Evolver::readConfig(std::string strFileName)
{
	std::ifstream t(strFileName.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();

	Evolver::traverse_xml(buffer.str());

	Evolver::printSettings();
}

void Evolver::traverse_xml(const std::string& input_xml)
{

    // make a safe-to-modify copy of input_xml
    // (you should never modify the contents of an std::string directly)
    std::vector<char> xml_copy(input_xml.begin(), input_xml.end());
    xml_copy.push_back('\0');

    // only use xml_copy from here on!
    rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);

    rapidxml::xml_node<>* rootNode = doc.first_node("Config");
    m_strTitle = rootNode->first_node("Title")->value();
    m_strDescription = rootNode->first_node("Description")->value();
    m_strPath = rootNode->first_node("DirectoryPath")->value();
    m_eGenomeType = static_cast<GenomeType> (atoi(rootNode->first_node("GenomeType")->value()));
    m_nChromosomes = atoi(rootNode->first_node("GenomeType")->first_attribute("chromosomes")->value());

    rapidxml::xml_node<>* settingsNode = rootNode->first_node("Settings");
    m_nPopulationSize = atoi(settingsNode->first_node("PopulationSize")->value());
    m_nMaxGenerations = atoi(settingsNode->first_node("MaxGenerations")->value());
    m_dTruncation = atof(settingsNode->first_node("Truncation")->value());
    m_eNormalization = static_cast<NormalizationType> (atoi(settingsNode->first_node("Truncation")->first_attribute("normalization")->value()));
    m_bElitism = (settingsNode->first_node("Elitism")->value() == std::string("true"))? true: false;
    m_nMaxParents = atoi(settingsNode->first_node("MaxParents")->value());
    m_eRecombination = static_cast<RecombinationType> (atoi(settingsNode->first_node("Recombination")->value()));

}

void Evolver::printSettings(){
	std::cout << "Config file loaded:" << std::endl;
	std::cout << "Title: " << m_strTitle << std::endl;
	std::cout << "Description: " << m_strDescription << std::endl;
	std::cout << "Directory path: " << m_strPath << std::endl;
	std::cout << "PopulationSize: " << m_nPopulationSize << std::endl;
	std::cout << "Max Generations: " << m_nMaxGenerations << std::endl;


}



void Evolver::start(){

	//create pointer to array of Rosetta objects for population and next generation
	Rosetta* pacPopulation = new Rosetta[m_nPopulationSize];
	Rosetta* pacNextGeneration = new Rosetta[m_nPopulationSize];


	//initialize Rosetta objects
	for(int i = 0; i<m_nPopulationSize; i++ ){
		pacPopulation[i].SetGenomeType(m_eGenomeType);
		pacPopulation[i].initGenome();
		//put random data in all genomes
		pacPopulation[i].randomGenome();
	}

	std::cout << pacPopulation[0].cGenome.GetXML() << std::endl;

	//create Aperture object

//do until maxGenerations reached

	//calculate fitness
	for(int i = 0; i<m_nPopulationSize; i++ ){
		pacPopulation[i].CalcFitness();
		//std::cout << "fitness of " << i << ": " << pacPopulation[i].GetFitness() << std::endl;
	}

	std::vector<Parent> vsSelection = MakeSelection(pacPopulation);

/*	for(unsigned int i = 0; i < vsSelection.size(); i++ ){
		std::cout << "Parent " << i << " index: " << vsSelection[i].nIndex;
		std::cout << " fitness: " << vsSelection[i].dFitness;
		std::cout << " normalized fitness: " << vsSelection[i].dNormalizedFitness;
		std::cout << " accumulated fitness: " << vsSelection[i].dAccumulatedNormalizedFitness << std::endl;
	}
*/


	//if elitims is true, set the first genome of the next generation to the genome with highest fitness
	if(m_bElitism){
		pacNextGeneration[0].SetGenome(pacPopulation[vsSelection[0].nIndex].cGenome.GetXML());
		pacNextGeneration[0].SetGenomeType(m_eGenomeType);
	}


	//do recombinations


	for(int i = m_bElitism; i<m_nPopulationSize; i++ ){		//if elitism is true, the for loop begins at 1 instead of 0
		m_cRecombination.Clear();		//make sure new recombination is empty

		switch (m_eRecombination)
		{
			case RECOMBINATION_RWS:
				RWS(vsSelection, pacPopulation);
				break;
			case RECOMBINATION_SUS:
				SUS(vsSelection, pacPopulation);
				break;
			case RECOMBINATION_TOURNAMENT:
				Tournament(vsSelection, pacPopulation);
				break;

			default:
				std::cout << "Unknown recombination type" << std::endl;
				break;
		}

		pacNextGeneration[i].SetGenome(m_cRecombination.RecombinedGenomeXML(m_nChromosomes));
		pacNextGeneration[i].SetGenomeType(m_eGenomeType);
	}

	//do mutations

	//copy next generation to population
	for(int i = 0; i<m_nPopulationSize; i++ ){
		pacPopulation[i].SetGenome(pacNextGeneration[i].cGenome.GetXML());
		pacPopulation[i].SetGenomeType(m_eGenomeType);
	}



//loop



	delete[] pacPopulation;	//clean up memory
	delete[] pacNextGeneration;
}

std::vector<Parent> Evolver::MakeSelection(Rosetta* population){
	//calculate which genomes are selected als possible parents
	int nSelectedGenomes;  		//number of genomes that get to have offspring

	assert(m_dTruncation > 0 && m_dTruncation <= 100);	//Truncation must be greater than 0 and smaller or equal to 100.
	nSelectedGenomes = m_nPopulationSize * m_dTruncation/100;

	std::cout << "number of selected genomes: " << nSelectedGenomes << std::endl;
	std::cout << "elitism: " << m_bElitism << std::endl;

	std::vector<Parent> vsParent(nSelectedGenomes);

	int nParentsAdded = 0;

	double dFitnessBelow = -1.0;
	while(nParentsAdded < nSelectedGenomes){
		double dHighestFitness = 0.0;
		int nFitnessOccurences = 0;

		for(int i = 0; i<m_nPopulationSize; i++ ){

			if(population[i].GetFitness() == dHighestFitness && nParentsAdded+nFitnessOccurences < nSelectedGenomes){
				nFitnessOccurences++;

				vsParent[nParentsAdded+nFitnessOccurences-1].nIndex = i;
				vsParent[nParentsAdded+nFitnessOccurences-1].dFitness = population[i].GetFitness();
			}

			if(population[i].GetFitness() > dHighestFitness && (population[i].GetFitness() < dFitnessBelow || dFitnessBelow == -1.0)){
				dHighestFitness = population[i].GetFitness();
				nFitnessOccurences = 1;

				vsParent[nParentsAdded].nIndex = i;
				vsParent[nParentsAdded].dFitness = population[i].GetFitness();
			}
		}
		dFitnessBelow = dHighestFitness;
		nParentsAdded = nParentsAdded + nFitnessOccurences;

	}



	//calculate chance to make offspring for each parent
	double dTotalFitness = 0;
	for(int i = 0; i<nSelectedGenomes; i++ ){
		dTotalFitness += vsParent[i].dFitness;
	}

	//some initializations to be used in the normalization
	assert(dTotalFitness != 0);		//check to avoid divide by zero error
	double dTmpAccumulatedFitness = 0.0;
	int nTotalUnits = 0;

	if(m_eNormalization == NORM_RANK){			//totalUnits is only needed for rank based selection , totalUnits = permutation of nSelectedGenomes
		for(int j = 1; j<=nSelectedGenomes; j++ ){
    		nTotalUnits += j;
		}
	}

	//normalize the fitness for each genome based on normalization method
	for(int i = 0; i<nSelectedGenomes; i++ ){
		switch (m_eNormalization)
	    {
    		case NORM_PROPORTIONAL: //chance is proportional based on fitness
    			vsParent[i].dNormalizedFitness = vsParent[i].dFitness/dTotalFitness;
    			vsParent[i].dAccumulatedNormalizedFitness = dTmpAccumulatedFitness;
    			break;
    		case NORM_RANK:		//for example: if you have a selection of 10 genomes, the first genome is 10 times more likely to be picked than the last, the second is 9 times, third is 8 times, ...
    			vsParent[i].dNormalizedFitness = ((nSelectedGenomes-i)/(double)nSelectedGenomes)*(((double)nSelectedGenomes)/(double)nTotalUnits);
    			vsParent[i].dAccumulatedNormalizedFitness = dTmpAccumulatedFitness;
    			break;
    		case NORM_EQUAL:	//chance to be picked is equal for all genomes
    			vsParent[i].dNormalizedFitness = 1.0/nSelectedGenomes;
    			vsParent[i].dAccumulatedNormalizedFitness = dTmpAccumulatedFitness;
    			break;

	        default:
	            std::cout << "Unknown normalization type" << std::endl;
	            break;
	    }
		dTmpAccumulatedFitness += vsParent[i].dNormalizedFitness;
	}


	return vsParent;
}

void Evolver::RWS(std::vector<Parent> selection, Rosetta* population){

	int nLow = 0;
	int nHigh = 10000;	//5 decimals
	double dRandom = 0;

	for(int i = 0; i<m_nMaxParents; i++){
		dRandom = ((rand() % (nHigh - nLow + 1)) + nLow)/(double)nHigh;		//random number between 0 and 1, 5 decimals

		for(unsigned int j = 0; j < selection.size(); j++){
			if(selection[j].dAccumulatedNormalizedFitness + selection[j].dNormalizedFitness >= dRandom){
				//add parent to recombination
				m_cRecombination.AddParent(population[selection[j].nIndex].cGenome.GetXML());
				break;
			}

		}

	}

}
void Evolver::SUS(std::vector<Parent> selection, Rosetta* population){
	int nLow = 0;
	int nHigh = 10000;	//5 decimals
	double dRandom = 0;

	dRandom = ((rand() % (nHigh - nLow + 1)) + nLow)/(double)nHigh;		//random number between 0 and 1, 5 decimals

	for(int i = 0; i<m_nMaxParents; i++){

		for(unsigned int j = 0; j < selection.size(); j++){
			if(selection[j].dAccumulatedNormalizedFitness + selection[j].dNormalizedFitness >= dRandom){
				//add parent to recombination
				m_cRecombination.AddParent(population[selection[j].nIndex].cGenome.GetXML());
				break;
			}
		}

		dRandom += 1.0/(double)m_nMaxParents;
		(dRandom > 1)?dRandom -= 1:dRandom;		//if dRandom get bigger than 1, subtract 1 to make it between 0 and 1 again
	}
}
void Evolver::Tournament(std::vector<Parent> selection, Rosetta* population){

}
