/*
 * Mutation.cpp
 *
 *  Created on: 3-feb.-2012
 *      Author: woutergl
 */

#include "Mutation.h"


/***************************************************************************************************************
 * Base Class Mutation
 ***************************************************************************************************************/

std::string Mutation::BitString(){
	return m_Node->value();
}
std::string Mutation::FlipBits(){
	return m_Node->value();
}
std::string Mutation::Boundary(){
	return m_Node->value();
}
std::string Mutation::Uniform(){
	return m_Node->value();
}
std::string Mutation::Gaussian(){
	return m_Node->value();
}
std::string Mutation::Duplication(){
	return m_Node->value();
}
std::string Mutation::Deletion(){
	return m_Node->value();
}
std::string Mutation::Insertion(){
	return m_Node->value();
}


/***************************************************************************************************************
 * Derived Class BitMutation
 ***************************************************************************************************************/

std::string BitMutation::BitString(){
	std::string strValue = m_Node->value();

	int nRandom = rand() % strValue.size();
	(strValue.at(nRandom) =='1')?strValue.at(nRandom) = '0':strValue.at(nRandom) = '1';

	return strValue;
}
std::string BitMutation::FlipBits(){
	std::string strValue = m_Node->value();

	for(unsigned int i = 0; i < strValue.size(); i++){
		(strValue.at(i) =='1')?strValue.at(i) = '0':strValue.at(i) = '1';
	}

	return strValue;
}
std::string BitMutation::Boundary(){

	std::string strValue = m_Node->value();
	std::stringstream ss;//create a stringstream

	int bit = rand() % 2;

	for(unsigned int i = 0; i < strValue.size(); i++){
		ss << bit;//add bit to the stream
	}

	return ss.str();//return a string with the contents of the stream
}
std::string BitMutation::Uniform(){
	BinaryEncoding cBinaryEncoding;
	return cBinaryEncoding.RandomData(m_Node);
}
/*std::string BitMutation::Gaussian(){

	//std::cout << box_muller(0, 2.0) << std::endl;
	return "";
}*/
std::string BitMutation::Duplication(){
	std::string strValue = m_Node->value();

	//duplicate a random part of the string
	int nBegin = rand() % strValue.size();
	int nEnd = (rand() % (strValue.size()-1 - nBegin + 1)) + nBegin;
	strValue.insert(nBegin, strValue, nBegin, nEnd-nBegin+1); // insert substring of strValue from index [nBegin,nEnd) into strValue at index nBegin

	//if there is a bits attribute, cut off the string at that length
	if(m_Node->first_attribute("bits")){
		int bits = atoi(m_Node->first_attribute("bits")->value());
		strValue = strValue.substr(0, bits);
	}

	return strValue;
}
std::string BitMutation::Deletion(){
	std::string strValue = m_Node->value();

	//delete a random part of the string
	int nBegin = rand() % strValue.size();
	int nEnd = (rand() % (strValue.size()-1 - nBegin + 1)) + nBegin;
	strValue = strValue.erase(nBegin, nEnd-nBegin+1);

	//if there is a bits attribute, fill the rest of string with random bits
	if(m_Node->first_attribute("bits")){
		std::stringstream ss;//create a stringstream
		int bit;
		for(int i = 0; i < (nEnd-nBegin+1); i++){
			bit = rand() % 2;
			ss << bit;//add bit to the stream
		}
		strValue.append(ss.str());
	}

	return strValue;
}
std::string BitMutation::Insertion(){
	std::string strValue = m_Node->value();

	//insert random bits into the string
	int nBegin = rand() % strValue.size();
	int nEnd = (rand() % (strValue.size()-1 - nBegin + 1)) + nBegin;

	std::stringstream ss;//create a stringstream
	int bit;

	for(int i = 0; i < (nEnd-nBegin+1); i++){
		bit = rand() % 2;
		ss << bit;//add bit to the stream
	}

	strValue.insert(nBegin, ss.str()); // insert ss.str() at index nBegin

	//if there is a bits attribute, cut off the string at that length
	if(m_Node->first_attribute("bits")){
		int bits = atoi(m_Node->first_attribute("bits")->value());
		strValue = strValue.substr(0, bits);
	}

	return strValue;
}


/***************************************************************************************************************
 * Derived Class IntegerMutation
 ***************************************************************************************************************/
std::string IntegerMutation::BitString(){
	std::string strValue = m_Node->value();

	int nRandom = rand() % strValue.size();
	int nNewBit = rand() % 10;

	std::stringstream ss;//create a stringstream
	ss << nNewBit;
	strValue.replace(nRandom, 1, ss.str());

	//make sure new value is not smaller than min boundary
	if(m_Node->first_attribute("min")){
		if( atoi(strValue.c_str()) < atoi(m_Node->first_attribute("min")->value()) ){
			return m_Node->value();
		}
	}

	//make sure new value is not greater than max boundary
	if(m_Node->first_attribute("max")){
		if( atoi(strValue.c_str()) > atoi(m_Node->first_attribute("max")->value()) ){
			return m_Node->value();
		}
	}


	return strValue;
}
/*std::string IntegerMutation::FlipBits(){
	return "";
}*/
std::string IntegerMutation::Boundary(){

	std::string strValue = m_Node->value();

	std::stringstream ss;//create a stringstream

	if(m_Node->first_attribute("min") && m_Node->first_attribute("max")){
		//pick one of the 2 boundaries at random
		int nRandom = rand() % 2;
		(nRandom == 0) ? ss << m_Node->first_attribute("min")->value() : ss << m_Node->first_attribute("max")->value();
		return ss.str();//return a string with the contents of the stream
	}
	else if(m_Node->first_attribute("min")){
		 ss << m_Node->first_attribute("min")->value();
		return ss.str();//return a string with the contents of the stream
	}
	else if(m_Node->first_attribute("max")){
		ss << m_Node->first_attribute("max")->value();
		return ss.str();//return a string with the contents of the stream
	}
	else
	{
		return strValue;
	}

}
std::string IntegerMutation::Uniform(){
	IntegerEncoding cIntegerEncoding;
	return cIntegerEncoding.RandomData(m_Node);
}
std::string IntegerMutation::Gaussian(){
	return "";
}
std::string IntegerMutation::Duplication(){
	return "";
}
std::string IntegerMutation::Deletion(){
	return "";
}
std::string IntegerMutation::Insertion(){
	return "";
}


/***************************************************************************************************************
 * Derived Class DoubleMutation
 ***************************************************************************************************************/
std::string DoubleMutation::BitString(){
	std::string strValue = m_Node->value();

	int nRandom = rand() % strValue.size();	//position to be replaced
	int nNewBit = rand() % 10;				//new character

	std::stringstream ss;//create a stringstream
	ss << nNewBit;

	//make sure the decimal point is not replaced
	if(strValue.at(nRandom) != '.'){
		strValue.replace(nRandom, 1, ss.str());
	}

	//make sure new value is not smaller than min boundary
	if(m_Node->first_attribute("min")){
		if( atof(strValue.c_str()) < atof(m_Node->first_attribute("min")->value()) ){
			return m_Node->value();
		}
	}

	//make sure new value is not greater than max boundary
	if(m_Node->first_attribute("max")){
		if( atof(strValue.c_str()) > atof(m_Node->first_attribute("max")->value()) ){
			return m_Node->value();
		}
	}

	return strValue;
}
/*std::string DoubleMutation::FlipBits(){
	return "";
}*/
std::string DoubleMutation::Boundary(){
	std::string strValue = m_Node->value();

	std::stringstream ss;//create a stringstream

	if(m_Node->first_attribute("min") && m_Node->first_attribute("max")){
		//pick one of the 2 boundaries at random
		int nRandom = rand() % 2;
		(nRandom == 0) ? ss << m_Node->first_attribute("min")->value() : ss << m_Node->first_attribute("max")->value();
		return ss.str();//return a string with the contents of the stream
	}
	else if(m_Node->first_attribute("min")){
		 ss << m_Node->first_attribute("min")->value();
		return ss.str();//return a string with the contents of the stream
	}
	else if(m_Node->first_attribute("max")){
		ss << m_Node->first_attribute("max")->value();
		return ss.str();//return a string with the contents of the stream
	}
	else
	{
		return strValue;
	}
}
std::string DoubleMutation::Uniform(){
	DoubleEncoding cDoubleEncoding;
	return cDoubleEncoding.RandomData(m_Node);
}
std::string DoubleMutation::Gaussian(){
	return "";
}
std::string DoubleMutation::Duplication(){
	return "";
}
std::string DoubleMutation::Deletion(){
	return "";
}
std::string DoubleMutation::Insertion(){
	return "";
}


/***************************************************************************************************************
 * Derived Class AlphanumMutation
 ***************************************************************************************************************/
std::string AlphanumMutation::BitString(){
	std::string strValue = m_Node->value();

	// make sure there is a string
	if(strValue.size() > 0 ){
		AlphanumEncoding cAlphanumEncoding;
		std::vector<char> characters = cAlphanumEncoding.GetChars();

		int nRandom = rand() % strValue.size();
		int nNewBit = rand() % characters.size();

		std::stringstream ss;//create a stringstream
		ss << characters.at(nNewBit);
		strValue.replace(nRandom, 1, ss.str());
	}

	return strValue;
}
/*std::string AlphanumMutation::FlipBits(){
	return "";
}*/
/*std::string AlphanumMutation::Boundary(){
	return "";
}*/
std::string AlphanumMutation::Uniform(){
	AlphanumEncoding cAlphanumEncoding;
	return cAlphanumEncoding.RandomData(m_Node);
}
std::string AlphanumMutation::Gaussian(){
	return "";
}
std::string AlphanumMutation::Duplication(){
	return "";
}
std::string AlphanumMutation::Deletion(){
	return "";
}
std::string AlphanumMutation::Insertion(){
	return "";
}


/***************************************************************************************************************
 * Derived Class CustomMutation
 ***************************************************************************************************************/
std::string CustomMutation::BitString(){
		std::string strValue = m_Node->value();

		// make sure there is a string
		if(strValue.size() > 0 ){
			CustomEncoding cCustomEncoding;
			cCustomEncoding.SetChars(m_Node->parent()->first_attribute("chars")->value());
			std::vector<char> characters = cCustomEncoding.GetChars();

			int nRandom = rand() % strValue.size();
			int nNewBit = rand() % characters.size();

			std::stringstream ss;//create a stringstream
			ss << characters.at(nNewBit);
			strValue.replace(nRandom, 1, ss.str());
		}

		return strValue;
}
/*std::string CustomMutation::FlipBits(){
	return "";
}*/
/*std::string CustomMutation::Boundary(){
	return "";
}*/
std::string CustomMutation::Uniform(){
	CustomEncoding cCustomEncoding;
	cCustomEncoding.SetChars(m_Node->parent()->first_attribute("chars")->value());
	return cCustomEncoding.RandomData(m_Node);
}
std::string CustomMutation::Gaussian(){
	return "";
}
std::string CustomMutation::Duplication(){
	return "";
}
std::string CustomMutation::Deletion(){
	return "";
}
std::string CustomMutation::Insertion(){
	return "";
}


/***************************************************************************************************************
 * Derived Class TreeMutation
 ***************************************************************************************************************/
/*std::string TreeMutation::BitString(){
	return "";
}*/
/*std::string TreeMutation::FlipBits(){
	return "";
}*/
/*std::string TreeMutation::Boundary(){
	return "";
}*/
std::string TreeMutation::Uniform(){
	return "";
}
std::string TreeMutation::Gaussian(){
	return "";
}
std::string TreeMutation::Duplication(){
	return "";
}
std::string TreeMutation::Deletion(){
	return "";
}
std::string TreeMutation::Insertion(){
	return "";
}

/***************************************************************************************************************
 boxmuller.c           Implements the Polar form of the Box-Muller
                         Transformation

                      (c) Copyright 1994, Everett F. Carter Jr.
                          Permission is granted by the author to use
			  this software for any application provided this
			  copyright notice is preserved.

*/

/* ranf() is uniform in 0..1 */
float ranf(){

	int nLow = 0;
	int nHigh = 10000;
	int data = (rand() % (nHigh - nLow + 1)) + nLow;

	return (float)data/nHigh;
}


float box_muller(float m, float s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * ranf() - 1.0;
			x2 = 2.0 * ranf() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}
