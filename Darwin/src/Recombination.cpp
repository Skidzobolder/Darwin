/*
 * Recombination.cpp
 *
 *  Created on: 16-feb.-2012
 *      Author: woutergl
 */

#include "Recombination.h"

void Recombination::Clear(){
	vstrParents.clear();
	//m_cNewGenome.SetXML("");
}





void Recombination::AddParent(const std::string& input_xml){
	vstrParents.push_back(input_xml);
}

std::string	Recombination::RecombinedGenomeXML(int chromosomes){
	using namespace rapidxml;

	Genome newGenome;
    newGenome.newGenome("next generation");

    std::string newGenomeXML = newGenome.GetXML();
    std::vector<char> xml_copyNewGenome(newGenomeXML.begin(), newGenomeXML.end());
    xml_copyNewGenome.push_back('\0');

    xml_document<> docNewGenome;
    docNewGenome.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copyNewGenome[0]);

    xml_node<>* newRootNode = docNewGenome.first_node("Genome");
    xml_node<>* clonedNode;


	for(int i = 0; i < chromosomes; i++){
		int nLow = 0;
		int nHigh = vstrParents.size()-1;
		int parent =  (rand() % (nHigh - nLow + 1)) + nLow;

	    // make a safe-to-modify copy of input_xml
	    // (you should never modify the contents of an std::string directly)
	    std::vector<char> xml_copy(vstrParents[parent].begin(), vstrParents[parent].end());
	    xml_copy.push_back('\0');

	    // only use xml_copy from here on!
	    xml_document<> doc;
	    doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	    xml_node<>* rootNode = doc.first_node("Genome");
	    xml_node<>* chromosomeNode = rootNode->first_node("Chromosome");

	    for(int j = 0; j < i; j++){
	    	chromosomeNode = chromosomeNode->next_sibling("Chromosome");		//go to nth chromosome of this parent
	    }

	    clonedNode = docNewGenome.clone_node( chromosomeNode );
	    newRootNode->append_node(clonedNode);

	}

	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), docNewGenome);

	return xml_as_string;
}

void Recombination::PrintRecombination(){
	std::cout << "recombination contains " << vstrParents.size() << " parents" << std::endl;

	for(unsigned int i = 0; i < vstrParents.size(); i++){
		std::cout << vstrParents[i] << std::endl;
	}


}
