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

    xml_node<>* settingsNode = rootNode->first_node("Settings");
    m_nPopulationSize = atoi(settingsNode->first_node("PopulationSize")->value());
    m_nMaxGenerations = atoi(settingsNode->first_node("MaxGenerations")->value());



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
/*	Genome cGenome;

	std::string strType = "Tokamak5";

	cGenome.newGenome(strType);
	//std::cout << cGenome.GetGenome() << std::endl;

	const char* pchId;

	pchId = "1";
	cGenome.addChromosome(pchId);


	pchId = "2";
	cGenome.addChromosome(pchId);


	pchId = "3654";
	cGenome.addChromosome("5");

	std::cout << std::endl << cGenome.GetGenome() << std::endl;

	pchId = "1";
	cGenome.addGene(pchId, "2");
	std::cout << cGenome.GetGenome() << std::endl;

*/

	//create Rosetta object
	Rosetta cRosetta(GENOMETYPE_TOKAMAK);
	std::cout << "Rosetta type is: " << cRosetta.GetGenomeType() << std::endl;

	cRosetta.initGenome();



	//make random children

	//create Aperture object

//do until maxGenerations reached

	//calculate fitness

	//save state

	//make next generation

	//do mutations
//loop



}

