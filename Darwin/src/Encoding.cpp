/*
 * Encoding.cpp
 *
 *  Created on: 21-feb.-2012
 *      Author: woutergl
 */

#include "Encoding.h"

std::string Encoding::RandomData(rapidxml::xml_node<>* geneNode){

	return "base encoding";
}

std::string BinaryEncoding::RandomData(rapidxml::xml_node<>* geneNode){

	int nLow = 0;
	int nHigh = 1;
	int bits = 16;	//default number of random bits

	//check if there is a "bits" attribute in the geneNode
	if(geneNode->first_attribute("bits")){
		bits = atoi(geneNode->first_attribute("bits")->value());
	}

	int bit ;

	std::stringstream ss;//create a stringstream
	for(int i = 0 ; i < bits ; i++){
		bit = (rand() % (nHigh - nLow + 1)) + nLow;
		ss << bit;//add number to the stream
	}

	return ss.str();//return a string with the contents of the stream

}

std::string ValueEncoding::RandomData(rapidxml::xml_node<>* geneNode){

	int nLow = 0;
	int nHigh = 100;

	//check if there is a "min" attribute in the geneNode
	if(geneNode->first_attribute("min")){
		nLow = atoi(geneNode->first_attribute("min")->value());
	}
	//check if there is a "max" attribute in the geneNode
	if(geneNode->first_attribute("max")){
		nHigh = atoi(geneNode->first_attribute("max")->value());
	}

	int data = (rand() % (nHigh - nLow + 1)) + nLow;

	std::stringstream ss;//create a stringstream
	ss << data;//add number to the stream
	return ss.str();//return a string with the contents of the stream

}

std::string TreeEncoding::RandomData(rapidxml::xml_node<>* geneNode){

	int nLow = 0;
	int nHigh = 100;

	int data = (rand() % (nHigh - nLow + 1)) + nLow;

	   std::stringstream ss;//create a stringstream
	   ss << data;//add number to the stream
	   return ss.str();//return a string with the contents of the stream

}

std::string IntegerEncoding::RandomData(rapidxml::xml_node<>* geneNode){

	int nLow = 0;
	int nHigh = 100;

	//check if there is a "min" attribute in the geneNode
	if(geneNode->first_attribute("min")){
		nLow = atoi(geneNode->first_attribute("min")->value());
	}
	//check if there is a "max" attribute in the geneNode
	if(geneNode->first_attribute("max")){
		nHigh = atoi(geneNode->first_attribute("max")->value());
	}

	int data = (rand() % (nHigh - nLow + 1)) + nLow;

	std::stringstream ss;//create a stringstream
	ss << data;//add number to the stream
	return ss.str();//return a string with the contents of the stream

}

std::string AlphanumEncoding::RandomData(rapidxml::xml_node<>* geneNode){

	int nLow = 0;
	int nHigh = 100;

	int data = (rand() % (nHigh - nLow + 1)) + nLow;

	   std::stringstream ss;//create a stringstream
	   ss << data;//add number to the stream
	   return ss.str();//return a string with the contents of the stream

}
