/*
 * Evolver.h
 *
 *  Created on: 3-feb.-2012
 *      Author: Skidzobolder
 */

#ifndef EVOLVER_H_
#define EVOLVER_H_

#include "Globals.h"

#include "Genome.h"
#include "Rosetta.h"
#include "Recombination.h"



class Evolver
{
private:

	std::string m_strPath; 			//Path to working directory

    std::string m_strTitle;			//Title of this job
    std::string m_strDescription; 	//detailed description of this job
    GenomeType m_eGenomeType;       //Type of genomes for this job
    int m_nChromosomes;				//number of chromosomes each genome has

    int m_nPopulationSize;			//total number of offspring in every generation
    int m_nMaxGenerations;        	//maximum number of generations to calculate (-1 for unlimited)
    int m_nMaxParents;              //maximum number of parents for recombination (must be greater of equal to 1)
    RecombinationType m_eRecombination;  //Method used to pick parents for recombination

    double m_dTruncation  ;         //percentage of population that gets to have offspring (0% -> 100%) (100%: every genome has offspring)
    NormalizationType m_eNormalization; //Normalization method used in truncation

    bool m_bElitism;				//Elitism enabled or not?

    Recombination m_cRecombination;

    void RWS(std::vector<Parent> selection, Rosetta* population);
    void SUS(std::vector<Parent> selection, Rosetta* population);
    void Tournament(std::vector<Parent> selection, Rosetta* population);


public:
    Evolver() // default constructor
    {
    	 srand(time(0)); // set initial seed value to system clock
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
    std::vector<Parent> MakeSelection(Rosetta* population);


};




#endif /* EVOLVER_H_ */
