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
	using namespace rapidxml;
    // (input_xml contains the above XML)

    // make a safe-to-modify copy of input_xml
    // (you should never modify the contents of an std::string directly)
    std::vector<char> xml_copy(input_xml.begin(), input_xml.end());
    xml_copy.push_back('\0');

    // only use xml_copy from here on!
    xml_document<> doc;
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

    xml_node<>* rootNode = doc.first_node("Config");
    m_strTitle = rootNode->first_node("Title")->value();
    m_strDescription = rootNode->first_node("Description")->value();
    m_strPath = rootNode->first_node("DirectoryPath")->value();
    m_eGenomeType = static_cast<GenomeType> (atoi(rootNode->first_node("GenomeType")->value()));

    xml_node<>* settingsNode = rootNode->first_node("Settings");
    m_nPopulationSize = atoi(settingsNode->first_node("PopulationSize")->value());
    m_nMaxGenerations = atoi(settingsNode->first_node("MaxGenerations")->value());
    m_dHasOffspring = atof(settingsNode->first_node("HasOffspring")->value());
    m_bElitism = (settingsNode->first_node("Elitism")->value() == std::string("true"))? true: false;

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

	//create pointer to array of Rosetta objects
	Rosetta* pacRosetta = new Rosetta[m_nPopulationSize];

	//initialize Rosetta objects
	for(int i = 0; i<m_nPopulationSize; i++ ){
		pacRosetta[i].SetGenomeType(m_eGenomeType);
		pacRosetta[i].initGenome();
	}

	//put random data in all genomes

	//create Aperture object

//do until maxGenerations reached

	//calculate fitness
	srand(time(0)); // set initial seed value to system clock
	for(int i = 0; i<m_nPopulationSize; i++ ){
		pacRosetta[i].CalcFitness();
		std::cout << "fitness of " << i << ": " << pacRosetta[i].GetFitness() << std::endl;
	}

	//calculate chance to make offspring for each genome
	int nSelectedGenomes;  		//number of genomes that get to have offspring

	assert(m_dHasOffspring > 0 && m_dHasOffspring <= 100);	//Has offspring must be greater than 0 and smaller or equal to 100.
	nSelectedGenomes = m_nPopulationSize * m_dHasOffspring/100;

	std::cout << "number of selected genomes: " << nSelectedGenomes << std::endl;
	std::cout << "elitism: " << m_bElitism << std::endl;

	Parent* asParent = new Parent[nSelectedGenomes];

	int nParentsAdded = 0;

	double dFitnessBelow = -1.0;
	while(nParentsAdded < nSelectedGenomes){
		double dHighestFitness = 0.0;
		int nFitnessOccurences = 0;

		for(int i = 0; i<m_nPopulationSize; i++ ){

			if(pacRosetta[i].GetFitness() == dHighestFitness && nParentsAdded+nFitnessOccurences < nSelectedGenomes){
				nFitnessOccurences++;
				//std::cout << "adding parent with same fitness at index " << nParentsAdded+nFitnessOccurences-1 << " occurence: " << nFitnessOccurences << std::endl;

				asParent[nParentsAdded+nFitnessOccurences-1].nIndex = i;
				asParent[nParentsAdded+nFitnessOccurences-1].dFitness = pacRosetta[i].GetFitness();
			}

			if(pacRosetta[i].GetFitness() > dHighestFitness && (pacRosetta[i].GetFitness() < dFitnessBelow || dFitnessBelow == -1.0)){
				//std::cout << "adding parent at index " << nParentsAdded << std::endl;

				dHighestFitness = pacRosetta[i].GetFitness();
				nFitnessOccurences = 1;
				asParent[nParentsAdded].nIndex = i;
				asParent[nParentsAdded].dFitness = pacRosetta[i].GetFitness();

			}
		}
		dFitnessBelow = dHighestFitness;


		nParentsAdded = nParentsAdded + nFitnessOccurences;
		std::cout << "Parentsadded: "<<nParentsAdded << " fitnessBelow: " << dFitnessBelow << " occurences: " << nFitnessOccurences << std::endl;
	}


	for(int i = 0; i<nSelectedGenomes; i++ ){
		std::cout << "Parent " << i << " index: " << asParent[i].nIndex << " fitness: " << asParent[i].dFitness << std::endl;
	}

	//save state

	//make next generation

	//do mutations
//loop


	delete[] pacRosetta;
	delete[] asParent;
}

