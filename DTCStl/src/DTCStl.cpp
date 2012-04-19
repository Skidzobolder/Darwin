//============================================================================
// Name        : DTCStl.cpp
// Author      : Wouter Glorieux
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <String>
#include <vector>
#include <fstream>
#include <math.h>

class data // 3 vertices of each triangle
{
public:
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
};


void StringExplode(std::string str, std::string separator, std::vector<std::string>* results){
    std::size_t found;
    found = str.find_first_of(separator);
    while(found != std::string::npos){
        if(found > 0){
            results->push_back(str.substr(0,found));
        }
        str = str.substr(found+1);
        found = str.find_first_of(separator);
    }
    if(str.length() > 0){
        results->push_back(str);
    }
}

int main(int argc, char *argv[]) {

	std::string strFileName = "";

	int nMode = 0;

	bool bVerbose = false;
	if(argc == 4){
		strFileName= argv[1];
		 if(argv[2] == std::string("-volume")){
			 nMode = 1;
		 }
		 else if (argv[2] == std::string("-centermass")){
			 nMode = 2;
		 }
		bVerbose = true;
	}
	else if(argc == 3){
		strFileName= argv[1];
		 if(argv[2] == std::string("-volume")){
			 nMode = 1;
		 }
		 else if (argv[2] == std::string("-centermass")){
			 nMode = 2;
		 }
	}
	else{
		std::cout << "Usage: DTCStl.exe inputFilename [-volume , -centermass] [-verbose]" << std::endl;
		exit(1);
	}


    // ifstream is used for reading files
    // We'll read from a file called Sample.dat
    std::ifstream inf(strFileName.c_str());

    // If we couldn't open the input file stream for reading
    if (!inf)
    {
        // Print an error and exit
        std::cerr << strFileName << " could not be opened for reading!" << std::endl;
        exit(2);
    }


    std::vector<data> vcData;
    // While there's still stuff left to read
    while (inf)
    {
        // read stuff from the file into a string and print it
        std::string strInput;
        std::getline(inf, strInput);
        std::vector<std::string> vstrData;
        if(strInput != ""){
        	StringExplode(strInput, " ", &vstrData);

        	if(vstrData.at(0) == std::string("vertex")){
        		data cData;
        		cData.x1 = atof(vstrData.at(1).c_str());
        		cData.x2 = atof(vstrData.at(2).c_str());
        		cData.x3 = atof(vstrData.at(3).c_str());
        		vstrData.clear();

        		std::getline(inf, strInput);
        		StringExplode(strInput, " ", &vstrData);
        		cData.y1 = atof(vstrData.at(1).c_str());
        		cData.y2 = atof(vstrData.at(2).c_str());
        		cData.y3 = atof(vstrData.at(3).c_str());
        		vstrData.clear();

        		std::getline(inf, strInput);
        		StringExplode(strInput, " ", &vstrData);
        		cData.z1 = atof(vstrData.at(1).c_str());
        		cData.z2 = atof(vstrData.at(2).c_str());
        		cData.z3 = atof(vstrData.at(3).c_str());
        		vstrData.clear();

        		vcData.push_back(cData);

        	}


        }
    }


    int numTriangles = vcData.size(); // pull in the STL file and determine number of triangles
    data* triangles = new data[numTriangles];


    // fill the triangles array with the data in the STL file
    for(unsigned int i=0; i < vcData.size(); i++){
    	triangles[i] = vcData.at(i);
    }


    double totalVolume = 0, currentVolume;
    double xCenter = 0, yCenter = 0, zCenter = 0;

    for (int i = 0; i < numTriangles; i++)
    {
        totalVolume += currentVolume = (triangles[i].x1*triangles[i].y2*triangles[i].z3 - triangles[i].x1*triangles[i].y3*triangles[i].z2 - triangles[i].x2*triangles[i].y1*triangles[i].z3 + triangles[i].x2*triangles[i].y3*triangles[i].z1 + triangles[i].x3*triangles[i].y1*triangles[i].z2 - triangles[i].x3*triangles[i].y2*triangles[i].z1) / 6;
        xCenter += ((triangles[i].x1 + triangles[i].x2 + triangles[i].x3) / 4) * currentVolume;
        yCenter += ((triangles[i].y1 + triangles[i].y2 + triangles[i].y3) / 4) * currentVolume;
        zCenter += ((triangles[i].z1 + triangles[i].z2 + triangles[i].z3) / 4) * currentVolume;
    }

    if(bVerbose){
    	std::cout << "Total Volume = " << totalVolume << std::endl;
    	std::cout << "X center = " << xCenter/totalVolume << std::endl;
    	std::cout << "Y center = " << yCenter/totalVolume << std::endl;
    	std::cout << "Z center = " << zCenter/totalVolume << std::endl;
    }


    delete[] triangles;


    int nScore = 0;
    switch (nMode)
    {
        case 1:

            break;
        case 2:

            break;

        default:
            std::cout << "Unknown mode";
            nScore = 0;
            break;
    }

    if(bVerbose){
    	std::cout << "\nSCORE: " << nScore << std::endl;
    }


	return nScore;
}
