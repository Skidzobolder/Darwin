/*
 * Recombination.cpp
 *
 *  Created on: 16-feb.-2012
 *      Author: woutergl
 */

#include "Recombination.h"

void Recombination::Clear(){
	vstrParents.clear();
}

void Recombination::AddParent(const std::string& input_xml){
	vstrParents.push_back(input_xml);
}

std::string	Recombination::RecombinedGenomeXML(){


	return std::string("");
}
