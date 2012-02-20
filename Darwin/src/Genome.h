/*
 * Genome.h
 *
 *  Created on: 6-feb.-2012
 *      Author: Skidzobolder
 */

#ifndef GENOME_H_
#define GENOME_H_

#include "Globals.h"

class Genome
{
private:

	std::string m_strXML;

public:
	//rapidxml::xml_document<> m_Genome;

    Genome() // default constructor
    {


    }
    ~Genome() // default deconstructor
    {
    	//std::cout << "genome class destroyed" << std::endl;

    }

    void newGenome(const char* pchId );
    void addChromosome(const char* pchId );
    void addGene(const char* pchId , std::string strChromosomeId );

    void CopyChromosomeFromParent();

    void SetXML(std::string input_xml);
    std::string GetXML();
};


#endif /* GENOME_H_ */
