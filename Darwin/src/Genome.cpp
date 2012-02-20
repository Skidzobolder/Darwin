/*
 * Genome.cpp
 *
 *  Created on: 6-feb.-2012
 *      Author: woutergl
 */

#include "Genome.h"

void Genome::newGenome(const char* pchId ){

	rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&m_strXML[0]);

	// xml declaration
    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	// root node
	rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, "Genome");

	root->append_attribute(doc.allocate_attribute("type", pchId));
	doc.append_node(root);

	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	rapidxml::print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	//set m_strXML to string value
	SetXML(xml_as_string);


}

void Genome::addChromosome(const char* pchId ){

	rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&m_strXML[0]);

    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");

	// chromosome node
    rapidxml::xml_node<>* chromosome = doc.allocate_node(rapidxml::node_element, "Chromosome");
	chromosome->append_attribute(doc.allocate_attribute("id", pchId));
	rootNode->append_node(chromosome);

	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	rapidxml::print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	//set m_strXML to string value
	SetXML(xml_as_string);

}

void Genome::addGene(const char* pchId, std::string strChromosomeId ){

    rapidxml::xml_document<> doc;
    std::vector<char> xml_copy(m_strXML.begin(), m_strXML.end());
    xml_copy.push_back('\0');

    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);

    rapidxml::xml_node<>* chromosome = doc.first_node("Genome");

	for (rapidxml::xml_node<> *child = chromosome->first_node("Chromosome"); child; child = child->next_sibling("Chromosome"))
	{


	    if(child->first_attribute("id")->value() == strChromosomeId){
	    // gene node
		rapidxml::xml_node<>* gene = doc.allocate_node(rapidxml::node_element, "Gene");
		gene->append_attribute(doc.allocate_attribute("id", pchId ));
		child->append_node(gene);

		break;
	    }

	}

	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	rapidxml::print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	//set m_strXML to string value
	SetXML(xml_as_string);


}

void Genome::CopyChromosomeFromParent(){

}


void Genome::SetXML(std::string input_xml){
	m_strXML = input_xml;
}

std::string Genome::GetXML(){
	return m_strXML;
}
