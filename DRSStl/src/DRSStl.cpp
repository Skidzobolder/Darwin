//============================================================================
// Name        : DRSStl.cpp
// Author      : Wouter Glorieux
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include "rapidxml.hpp"
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <process.h>

struct point3D
{
	double X;
	double Y;
	double Z;
};

struct controlPoints
{
	point3D a1, a2, a3, a4;
	point3D b1, b2, b3, b4;
	point3D c1, c2, c3, c4;
	point3D d1, d2, d3, d4;
};

struct grid
{
	int X;
	int Y;
	int Z;
};



void StringExplode(std::string str, std::string separator, std::vector<std::string>* results);


controlPoints GetControlPoints(int i, int j, int k);
std::string GetSCAD(int i, int j, int k);
void SortControlPoints();
void WriteSTL(std::string outputFileName);
void SelectSegments();


grid sGrid;
bool abGrid[5][5];
bool mode2D = false;

point3D asPoints[16][16];			//2d array of co�rdinates of all points
std::vector<point3D> vsPoints;      //vector with all points


bool sortByX(const point3D &a, const point3D &b)
{
    return a.X < b.X;
}

bool sortByY(const point3D &a, const point3D &b)
{
    return a.Y > b.Y;
}


int main(int argc, char *argv[]) {
	std::string strFileName = "";
	std::string strOutputFileName = "";


	double dXmultiplier = 0.01;
	double dYmultiplier = 0.01;
	double dZmultiplier = 0.001;


	if(argc == 3){
		strFileName= argv[1];
		strOutputFileName= argv[2];
	}
	else{
		std::cout << "Usage: Tokamak.exe inputFilename outputFilename" << std::endl;
		std::exit(1);
	}

	//std::cout << "loading file: " << strFileName << std::endl;
	std::ifstream t(strFileName.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::string input_xml = buffer.str() ;
	//std::ofstream output(strOutputFileName.c_str());

    // make a safe-to-modify copy of input_xml
    // (you should never modify the contents of an std::string directly)
    std::vector<char> xml_copy(input_xml.begin(), input_xml.end());
    xml_copy.push_back('\0');

    // only use xml_copy from here on!
    rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);
    rapidxml::xml_node<>* rootNode = doc.first_node("Genome");


    sGrid.X = atoi(rootNode->first_attribute("gridX")->value());
    sGrid.Y = atoi(rootNode->first_attribute("gridY")->value());
    sGrid.Z = atoi(rootNode->first_attribute("gridZ")->value());





	for (rapidxml::xml_node<>* chromosomeNode = rootNode->first_node("Chromosome"); chromosomeNode; chromosomeNode = chromosomeNode->next_sibling("Chromosome"))
	{
		if(chromosomeNode->first_attribute("id")->value() == std::string("grid")){

			int j = 0;
			for (rapidxml::xml_node<>* geneNode = chromosomeNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
			{

				std::string strData = geneNode->value();
				for(unsigned int i = 0; i < strData.size(); i++){
					abGrid[j][i] = strData.at(i) == '1'? true : false ;
				}
				j++;
			}

		}
		else{
			for (rapidxml::xml_node<>* geneNode = chromosomeNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
			{
				std::vector<std::string> vstrData;
				StringExplode(geneNode->value(), "|", &vstrData);
				point3D sPoint3D;
				sPoint3D.X = atof(vstrData.at(0).c_str()) * dXmultiplier;
				sPoint3D.Y = atof(vstrData.at(1).c_str()) * dYmultiplier;
				if(!mode2D){
					sPoint3D.Z = atof(vstrData.at(2).c_str()) * dZmultiplier;
				}
				else{
					sPoint3D.Z = 0;
				}

				//sPoint3D.deltaX = 10.0;
				//sPoint3D.deltaY = 5.0;

				//std::cout << i << " " << j << std::endl;
				vsPoints.push_back(sPoint3D);

			}
		}
	}

	abGrid[0][0] = true;
    SelectSegments();

	SortControlPoints();

	WriteSTL(strOutputFileName);


	//std::cout << "done" << std::endl;
	return 0;
}


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



controlPoints GetControlPoints(int i, int j, int k){
	controlPoints sControlPoints;

	sControlPoints.a1 = asPoints[i*3][j*3];
	sControlPoints.a2 = asPoints[i*3+1][j*3];
	sControlPoints.a3 = asPoints[i*3+2][j*3];
	sControlPoints.a4 = asPoints[i*3+3][j*3];

	sControlPoints.b1 = asPoints[i*3][j*3+1];
	sControlPoints.b2 = asPoints[i*3+1][j*3+1];
	sControlPoints.b3 = asPoints[i*3+2][j*3+1];
	sControlPoints.b4 = asPoints[i*3+3][j*3+1];

	sControlPoints.c1 = asPoints[i*3][j*3+2];
	sControlPoints.c2 = asPoints[i*3+1][j*3+2];
	sControlPoints.c3 = asPoints[i*3+2][j*3+2];
	sControlPoints.c4 = asPoints[i*3+3][j*3+2];

	sControlPoints.d1 = asPoints[i*3][j*3+3];
	sControlPoints.d2 = asPoints[i*3+1][j*3+3];
	sControlPoints.d3 = asPoints[i*3+2][j*3+3];
	sControlPoints.d4 = asPoints[i*3+3][j*3+3];


	return sControlPoints;
}

std::string GetSCAD(int i, int j, int k=0){


	controlPoints sControlPoints;
	sControlPoints = GetControlPoints(i,j,k);


	std::stringstream ss;

	ss << "[[" << sControlPoints.a1.X << "," << sControlPoints.a1.Y << "," << sControlPoints.a1.Z << "], ["
			<< sControlPoints.a2.X <<"," << sControlPoints.a2.Y <<"," << sControlPoints.a2.Z << "], ["
			<< sControlPoints.a3.X << "," << sControlPoints.a3.Y << "," << sControlPoints.a3.Z << "], ["
			<< sControlPoints.a4.X << "," << sControlPoints.a4.Y << "," << sControlPoints.a4.Z << "]]";

	std::string gcp1 = ss.str();
	ss.str("");

	ss << "[[" << sControlPoints.b1.X << "," << sControlPoints.b1.Y << "," << sControlPoints.b1.Z << "], ["
			<< sControlPoints.b2.X <<"," << sControlPoints.b2.Y <<"," << sControlPoints.b2.Z << "], ["
			<< sControlPoints.b3.X << "," << sControlPoints.b3.Y << "," << sControlPoints.b3.Z << "], ["
			<< sControlPoints.b4.X << "," << sControlPoints.b4.Y << "," << sControlPoints.b4.Z << "]]";

	std::string gcp2 = ss.str();
	ss.str("");

	ss << "[[" << sControlPoints.c1.X << "," << sControlPoints.c1.Y << "," << sControlPoints.c1.Z << "], ["
			<< sControlPoints.c2.X <<"," << sControlPoints.c2.Y <<"," << sControlPoints.c2.Z << "], ["
			<< sControlPoints.c3.X << "," << sControlPoints.c3.Y << "," << sControlPoints.c3.Z << "], ["
			<< sControlPoints.c4.X << "," << sControlPoints.c4.Y << "," << sControlPoints.c4.Z << "]]";

	std::string gcp3 = ss.str();
	ss.str("");

	ss << "[[" << sControlPoints.d1.X << "," << sControlPoints.d1.Y << "," << sControlPoints.d1.Z << "], ["
			<< sControlPoints.d2.X <<"," << sControlPoints.d2.Y <<"," << sControlPoints.d2.Z << "], ["
			<< sControlPoints.d3.X << "," << sControlPoints.d3.Y << "," << sControlPoints.d3.Z << "], ["
			<< sControlPoints.d4.X << "," << sControlPoints.d4.Y << "," << sControlPoints.d4.Z << "]]";

	std::string gcp4 = ss.str();
	ss.str("");


	ss << "DisplayBezSurface([" << gcp1 <<  ", "
									<< gcp2 <<  ", "
									<< gcp3 << ", "
									<< gcp4
									<< "], steps=5, thickness=10);" << std::endl;

/*	ss << "DisplayBezControlFrame([" << gcp1 <<  ", "
									<< gcp2 <<  ", "
									<< gcp3 << ", "
									<< gcp4
									<< "], $fn=3);" << std::endl;

*/

	//std::cout << ss.str() << std::endl;
	return ss.str();
}

void SortControlPoints(){

	std::sort(vsPoints.begin(), vsPoints.end(), sortByX);

	for(int i=((sGrid.Y-1)*(sGrid.Y-1))-1; i >= 0; i--){
		std::sort(vsPoints.end()-(16), vsPoints.end(), sortByY);
		asPoints[i][15] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][14] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][13] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][12] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][11] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][10] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][9] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][8] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][7] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][6] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][5] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][4] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][3] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][2] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][1] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();
		asPoints[i][0] = vsPoints.at(vsPoints.size()-1);
		vsPoints.pop_back();

	}


}

void WriteSTL(std::string outputFileName){
	std::string strTempSCADfile = "temp.scad";
	std::ofstream output(strTempSCADfile.c_str());
	output << "include <bezierSurface.scad>" << std::endl;


	for(int i = 0; i < sGrid.X ; i++ ){
		for(int j = 0; j < sGrid.Y ; j++ ){
			if(abGrid[i][j] == true){
				output << GetSCAD(i,j,0) << std::endl << std::endl;
			}
		}
	}

	output.close();


	std::cout << "Writing stl: " << outputFileName << std::endl;
	spawnl(P_WAIT, "c:\\Darwin\\RosettaStones\\STL\\openscad.exe", "openscad.exe", "-o", outputFileName.c_str(), "temp.scad" , NULL);


}

void SelectSegments(){
	std::cout << std::endl;
	for(int i = 0; i < sGrid.X ; i++ ){
		for(int j = 0; j < sGrid.Y ; j++ ){

				int nLeft = i - 1;
				int nUp = j - 1;

				if(nLeft >= 0 && nUp >= 0){
					if(!(abGrid[nLeft][j] == true || abGrid[i][nUp] == true)){
						abGrid[i][j]= false;
					}
				}
				else if( nUp >= 0){
					if(abGrid[i][nUp] != true){
						abGrid[i][j]= false;
					}
				}
				else if(nLeft >= 0){
					if(abGrid[nLeft][j] != true){
						abGrid[i][j]= false;
					}
				}


			std::cout << "\t" << abGrid[i][j]  ;
		}

		std::cout << std::endl;
	}
}

