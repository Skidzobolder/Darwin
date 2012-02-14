/*
 * Evolver.h
 *
 *  Created on: 3-feb.-2012
 *      Author: Skidzobolder
 */

#ifndef EVOLVER_H_
#define EVOLVER_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cassert>


#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "Globals.h"

#include "Genome.h"
#include "Rosetta.h"

class Evolver
{
private:

	std::string m_strPath; 			//Path to working directory

    std::string m_strTitle;			//Title of this job
    std::string m_strDescription; 	//detailed description of this job
    GenomeType m_eGenomeType;       //Type of genomes for this job

    int m_nPopulationSize;			//total number of offspring in every generation
    int m_nMaxGenerations;        	//maximum number of generations to calculate (-1 for unlimited)
    double m_dHasOffspring;         //percentage of population that gets to have offspring (0% -> 100%) (100%: every genome has offspring)

    bool m_bElitism;				//Elitism enabled or not?



public:
    Evolver() // default constructor
    {
         m_nPopulationSize = 0;
         m_strTitle = "";

    }

    int GetPopulationSize() { return m_nPopulationSize; }
    std::string GetTitle() { return m_strTitle; }
    std::string GetPath() { return m_strPath; }

    void readConfig(std::string strFileName);
    void traverse_xml(const std::string& input_xml);
    void printSettings();

    void start();


};

struct Parent
{
    int nIndex;						//index in current population
    double dFitness;				//fitness
    double dChanceForReproduction;	//percentual chance that this parent is selected for reproduction
};


#endif /* EVOLVER_H_ */
