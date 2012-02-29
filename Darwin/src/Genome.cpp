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

	Genome::Save(doc);
}

void Genome::addChromosome(const char* pchId, EncodingType encoding ){

	rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&m_strXML[0]);

    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");

	// chromosome node
    rapidxml::xml_node<>* chromosome = doc.allocate_node(rapidxml::node_element, "Chromosome");
	chromosome->append_attribute(doc.allocate_attribute("id", pchId));

	std::string strEncoding;
	switch (encoding)
	    {
	        case ENCODING_BINARY:
	        	strEncoding = "binary";
	            break;

	        case ENCODING_INTEGER:
	        	strEncoding = "integer";
	            break;

	        case ENCODING_DOUBLE:
	        	strEncoding = "double";
	            break;

	        case ENCODING_ALPHANUM:
	        	strEncoding = "alphanum";
	            break;

	        case ENCODING_CUSTOM:
	        	strEncoding = "custom";
	            break;

	        case ENCODING_TREE:
	        	strEncoding = "tree";
	            break;

	        default:
	            std::cout << "Unknown encoding type" << std::endl;
	            break;
	    }

	chromosome->append_attribute(doc.allocate_attribute("encoding", strEncoding.c_str()));
	rootNode->append_node(chromosome);

	Genome::Save(doc);

}

void Genome::addGene(const char* pchId, std::string strChromosomeId ){

    rapidxml::xml_document<> doc;
    std::vector<char> xml_copy(m_strXML.begin(), m_strXML.end());
    xml_copy.push_back('\0');

    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);

    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");

	for (rapidxml::xml_node<> *child = rootNode->first_node("Chromosome"); child; child = child->next_sibling("Chromosome"))
	{
	    if(child->first_attribute("id")->value() == strChromosomeId){
	    // gene node
		rapidxml::xml_node<>* gene = doc.allocate_node(rapidxml::node_element, "Gene");
		gene->append_attribute(doc.allocate_attribute("id", pchId ));
		child->append_node(gene);

		break;
	    }

	}

	Genome::Save(doc);

}

void Genome::addChromosomeAttribute(std::string strChromosomeId, std::string strName, std::string strValue){

    rapidxml::xml_document<> doc;
    std::vector<char> xml_copy(m_strXML.begin(), m_strXML.end());
    xml_copy.push_back('\0');

    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);

    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");

	for (rapidxml::xml_node<>* chromosomeNode = rootNode->first_node("Chromosome"); chromosomeNode; chromosomeNode = chromosomeNode->next_sibling("Chromosome"))
	{
	    if(chromosomeNode->first_attribute("id")->value() == strChromosomeId){
	    	char* pchName = doc.allocate_string(strName.c_str());
	    	char* pchValue = doc.allocate_string(strValue.c_str());
	    	chromosomeNode->append_attribute(doc.allocate_attribute(pchName, pchValue ));
	    	break;
	    }

	}

	Genome::Save(doc);

}
void Genome::addGeneAttribute(std::string strChromosomeId, std::string strGeneId, std::string strName, std::string strValue){
    rapidxml::xml_document<> doc;
    std::vector<char> xml_copy(m_strXML.begin(), m_strXML.end());
    xml_copy.push_back('\0');

    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);

    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");

	for (rapidxml::xml_node<>* chromosomeNode = rootNode->first_node("Chromosome"); chromosomeNode; chromosomeNode = chromosomeNode->next_sibling("Chromosome"))
	{
	    if(chromosomeNode->first_attribute("id")->value() == strChromosomeId){
	    	for (rapidxml::xml_node<>* geneNode = chromosomeNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
	    	{
	    		if(geneNode->first_attribute("id")->value() == strGeneId){
	    			char* pchName = doc.allocate_string(strName.c_str());
	    			char* pchValue = doc.allocate_string(strValue.c_str());
	    			geneNode->append_attribute(doc.allocate_attribute(pchName, pchValue ));
	    			break;
	    		}
	    	}
	    	break;
	    }
	}

	Genome::Save(doc);

}


void Genome::SetXML(std::string input_xml){
	m_strXML = input_xml;
}


void Genome::FillWithRandomData(){
    rapidxml::xml_document<> doc;
    std::vector<char> xml_copy(m_strXML.begin(), m_strXML.end());
    xml_copy.push_back('\0');

    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);
    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");

    std::string strRandomData;


	for (rapidxml::xml_node<>* child = rootNode->first_node("Chromosome"); child; child = child->next_sibling("Chromosome"))
	{
		std::string strEncoding = child->first_attribute("encoding")->value();
		if(strEncoding == "binary"){
			//std::cout << "putting random data in binary genes" << std::endl;
			BinaryEncoding cBinaryEncoding;
			Encoding& rEncoding = cBinaryEncoding;
			FillGenes(doc, child, rEncoding);
		}
		else if(strEncoding == "integer"){
			//std::cout << "putting random data in integer genes" << std::endl;
			IntegerEncoding cIntegerEncoding;
			Encoding& rEncoding = cIntegerEncoding;
			FillGenes(doc, child, rEncoding);
		}
		else if(strEncoding == "double"){
			//std::cout << "putting random data in double genes" << std::endl;
			DoubleEncoding cDoubleEncoding;
			Encoding& rEncoding = cDoubleEncoding;
			FillGenes(doc, child, rEncoding);
		}
		else if(strEncoding == "alphanum"){
			//std::cout << "putting random data in alphanum genes" << std::endl;
			AlphanumEncoding cAlphanumEncoding;
			Encoding& rEncoding = cAlphanumEncoding;
			FillGenes(doc, child, rEncoding);
		}
		else if(strEncoding == "custom"){
			//std::cout << "putting random data in custom genes" << std::endl;
			CustomEncoding cCustomEncoding;
			if(child->first_attribute("chars")){
				std::string	strCharacters = child->first_attribute("chars")->value();
				cCustomEncoding.SetChars(strCharacters);
			}
			Encoding& rEncoding = cCustomEncoding;
			FillGenes(doc, child, rEncoding);
		}
		else if(strEncoding == "tree"){
			//std::cout << "putting random data in tree genes" << std::endl;
			TreeEncoding cTreeEncoding;
			Encoding& rEncoding = cTreeEncoding;
			FillGenes(doc, child, rEncoding);
		}

		else{
			std::cout << "unknown encoding type" << std::endl;
		}


	}

	Genome::Save(doc);

}

void Genome::FillGenes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* chromosomeNode, Encoding& encoding){
	for (rapidxml::xml_node<>* geneNode = chromosomeNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
	{
		char* pchRandomData = doc.allocate_string(encoding.RandomData(geneNode).c_str());
		geneNode->value(pchRandomData);

	}

}

void Genome::Save(rapidxml::xml_document<>& doc){
	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	//set m_strXML to string value
	SetXML(xml_as_string);
}

void Genome::DoMutations(){
	//std::cout << "starting mutations" << std::endl;

	rapidxml::xml_document<> doc;
    std::vector<char> xml_copy(m_strXML.begin(), m_strXML.end());
    xml_copy.push_back('\0');

    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);
    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");

	for (rapidxml::xml_node<>* chromosomeNode = rootNode->first_node("Chromosome"); chromosomeNode; chromosomeNode = chromosomeNode->next_sibling("Chromosome"))
	{
		for (rapidxml::xml_node<>* geneNode = chromosomeNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
		{
			std::string strEncoding = chromosomeNode->first_attribute("encoding")->value();

			if(strEncoding == "binary"){
				BitMutation cBitMutation(geneNode);
				Mutation& rMutation = cBitMutation;
				GeneMutations(doc, geneNode, rMutation);
			}
			else if(strEncoding == "integer"){
				IntegerMutation cIntegerMutation(geneNode);
				Mutation& rMutation = cIntegerMutation;
				GeneMutations(doc, geneNode, rMutation);
			}
			else if(strEncoding == "double"){
				DoubleMutation cDoubleMutation(geneNode);
				Mutation& rMutation = cDoubleMutation;
				GeneMutations(doc, geneNode, rMutation);
			}
			else if(strEncoding == "alphanum"){
				AlphanumMutation cAlphanumMutation(geneNode);
				Mutation& rMutation =  cAlphanumMutation;
				GeneMutations(doc, geneNode, rMutation);
			}
			else if(strEncoding == "custom"){
				CustomMutation cCustomMutation(geneNode);
				Mutation& rMutation = cCustomMutation;
				GeneMutations(doc, geneNode, rMutation);
			}
			else if(strEncoding == "tree"){
				TreeMutation cTreeMutation(geneNode);
				Mutation& rMutation = cTreeMutation;
				GeneMutations(doc, geneNode, rMutation);
			}

			else{
				std::cout << "unknown encoding type" << std::endl;
			}
		}
	}

	Genome::Save(doc);
}

void Genome::GeneMutations(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* geneNode, Mutation& mutation){

		char* pchMutatedData;
/*
		pchMutatedData = doc.allocate_string(mutation.BitString().c_str());
		geneNode->value(pchMutatedData);

		pchMutatedData = doc.allocate_string(mutation.FlipBits().c_str());
		geneNode->value(pchMutatedData);

		pchMutatedData = doc.allocate_string(mutation.Boundary().c_str());
		geneNode->value(pchMutatedData);

		pchMutatedData = doc.allocate_string(mutation.Uniform().c_str());
		geneNode->value(pchMutatedData);
*/
		//pchMutatedData = doc.allocate_string(mutation.Gaussian().c_str());
		//geneNode->value(pchMutatedData);

		pchMutatedData = doc.allocate_string(mutation.Duplication().c_str());
		geneNode->value(pchMutatedData);

		//pchMutatedData = doc.allocate_string(mutation.Deletion().c_str());
		//geneNode->value(pchMutatedData);

		//pchMutatedData = doc.allocate_string(mutation.Insertion().c_str());
		//geneNode->value(pchMutatedData);

}
/*
void Genome::Mutations(Mutation& mutation){

	mutation.BitString();
	mutation.FlipBits();
	mutation.Boundary();
	mutation.Uniform();
	mutation.Gaussian();
	mutation.Duplication();
	mutation.Deletion();
	mutation.Insertion();

}
*/
