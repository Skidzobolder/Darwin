/*
 * Genome.cpp
 *
 *  Created on: 6-feb.-2012
 *      Author: woutergl
 */

#include "Genome.h"

void Genome::newGenome(const char* pchId ){

	using namespace rapidxml;

    xml_document<> doc;
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&m_strXML[0]);

	// xml declaration
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	// root node
	xml_node<>* root = doc.allocate_node(node_element, "Genome");

	root->append_attribute(doc.allocate_attribute("type", pchId));
	doc.append_node(root);

	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	//set m_strXML to string value
	SetXML(xml_as_string);


}

void Genome::addChromosome(const char* pchId ){
	using namespace rapidxml;

    xml_document<> doc;
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&m_strXML[0]);

    xml_node<>* rootNode = doc.first_node("Genome");

	// chromosome node
	xml_node<>* chromosome = doc.allocate_node(node_element, "Chromosome");
	chromosome->append_attribute(doc.allocate_attribute("id", pchId));
	rootNode->append_node(chromosome);

	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	//set m_strXML to string value
	SetXML(xml_as_string);

}

void Genome::addGene(const char* pchId, std::string strChromosomeId ){
	using namespace rapidxml;

    xml_document<> doc;
    std::vector<char> xml_copy(m_strXML.begin(), m_strXML.end());
    xml_copy.push_back('\0');

    doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

    xml_node<>* chromosome = doc.first_node("Genome");

	for (xml_node<> *child = chromosome->first_node("Chromosome"); child; child = child->next_sibling("Chromosome"))
	{


	    if(child->first_attribute("id")->value() == strChromosomeId){
	    // gene node
		xml_node<>* gene = doc.allocate_node(node_element, "Gene");
		gene->append_attribute(doc.allocate_attribute("id", pchId ));
		child->append_node(gene);

		break;
	    }

	}

	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	print(std::back_inserter(xml_as_string), doc);
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
