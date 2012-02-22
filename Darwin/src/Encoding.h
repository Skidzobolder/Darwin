/*
 * Encoding.h
 *
 *  Created on: 21-feb.-2012
 *      Author: woutergl
 */

#ifndef ENCODING_H_
#define ENCODING_H_

#include "Globals.h"

class Encoding
{
private:



public:

    Encoding() // default constructor
    {    }
    virtual ~Encoding() // default deconstructor
    {    }

    virtual std::string RandomData(rapidxml::xml_node<>* geneNode);

};

class BinaryEncoding : public Encoding
{
private:



public:

	BinaryEncoding() // default constructor
    {    }
	virtual ~BinaryEncoding() // default deconstructor
    {    }

	virtual std::string RandomData(rapidxml::xml_node<>* geneNode);

};


class ValueEncoding : public Encoding
{
private:



public:

    ValueEncoding() // default constructor
    {    }
    virtual ~ValueEncoding() // default deconstructor
    {    }

    virtual std::string RandomData(rapidxml::xml_node<>* geneNode);

};

class TreeEncoding : public Encoding
{
private:



public:

	TreeEncoding() // default constructor
    {    }
	virtual ~TreeEncoding() // default deconstructor
    {    }

    virtual std::string RandomData(rapidxml::xml_node<>* geneNode);

};

class IntegerEncoding : public Encoding
{
private:



public:

	IntegerEncoding() // default constructor
    {    }
    virtual ~IntegerEncoding() // default deconstructor
    {    }

    virtual std::string RandomData(rapidxml::xml_node<>* geneNode);

};

class AlphanumEncoding : public Encoding
{
private:



public:

	AlphanumEncoding() // default constructor
    {    }
    virtual ~AlphanumEncoding() // default deconstructor
    {    }

    virtual std::string RandomData(rapidxml::xml_node<>* geneNode);

};

#endif /* ENCODING_H_ */
