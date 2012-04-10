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

enum Corner
{
	NW = 1,			//North-West corner
	NE = 2,			//North-East corner
	SW = 3,	        //South-West corner
	SE = 4			//South-East corner
};

void StringExplode(std::string str, std::string separator, std::vector<std::string>* results);
point3D GetCorner(Corner corner, int i, int j, int k);
point3D GetXEdge(Corner corner, int i, int j, int k);
point3D GetYEdge(Corner corner, int i, int j, int k);
point3D GetZEdge(Corner corner, int i, int j, int k);
point3D GetMiddle(Corner corner, int i, int j, int k);

controlPoints GetControlPoints(int i, int j, int k);
std::string GetSCAD(int i, int j, int k);


grid sGrid;
bool mode2D = true;


std::vector<point3D> vsCorners;		//vector of co�rdinates of the corners
std::vector<point3D> vsXEdges;		//vector of co�rdinates of the horizontal edges
std::vector<point3D> vsYEdges;		//vector of co�rdinates of the vertical edges
std::vector<point3D> vsZEdges;		//vector of co�rdinates of the depth edges
std::vector<point3D> vsMiddles;		//vector of co�rdinates of the middles

int main(int argc, char *argv[]) {
	std::string strFileName = "";
	std::string strOutputFileName = "";


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
	std::ofstream output(strOutputFileName.c_str());

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

    rapidxml::xml_node<>* cornersNode = rootNode->first_node("Chromosome");
    rapidxml::xml_node<>* horizontalEdgesNode = cornersNode->next_sibling("Chromosome");
    rapidxml::xml_node<>* verticalEdgesNode = horizontalEdgesNode->next_sibling("Chromosome");
    rapidxml::xml_node<>* middlesNode = verticalEdgesNode->next_sibling("Chromosome");




	for (rapidxml::xml_node<>* geneNode = cornersNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
	{
		std::vector<std::string> vstrData;
		StringExplode(geneNode->value(), "|", &vstrData);
		point3D sPoint3D;
		sPoint3D.X = atof(vstrData.at(0).c_str());
		sPoint3D.Y = atof(vstrData.at(1).c_str());
		sPoint3D.Z = atof(vstrData.at(2).c_str());
		vsCorners.push_back(sPoint3D);
	}

	for (rapidxml::xml_node<>* geneNode = horizontalEdgesNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
	{
		std::vector<std::string> vstrData;
		StringExplode(geneNode->value(), "|", &vstrData);
		point3D sPoint3D;
		sPoint3D.X = atof(vstrData.at(0).c_str());
		sPoint3D.Y = atof(vstrData.at(1).c_str());
		sPoint3D.Z = atof(vstrData.at(2).c_str());
		vsXEdges.push_back(sPoint3D);
	}

	for (rapidxml::xml_node<>* geneNode = verticalEdgesNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
	{
		std::vector<std::string> vstrData;
		StringExplode(geneNode->value(), "|", &vstrData);
		point3D sPoint3D;
		sPoint3D.X = atof(vstrData.at(0).c_str());
		sPoint3D.Y = atof(vstrData.at(1).c_str());
		sPoint3D.Z = atof(vstrData.at(2).c_str());
		vsYEdges.push_back(sPoint3D);
	}

	for (rapidxml::xml_node<>* geneNode = middlesNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
	{
		std::vector<std::string> vstrData;
		StringExplode(geneNode->value(), "|", &vstrData);
		point3D sPoint3D;
		sPoint3D.X = atof(vstrData.at(0).c_str());
		sPoint3D.Y = atof(vstrData.at(1).c_str());
		sPoint3D.Z = atof(vstrData.at(2).c_str());
		vsMiddles.push_back(sPoint3D);
	}

	if(vsCorners.size() != ((sGrid.X+1) * (sGrid.Y+1))){
		std::cout << "Wrong number of corner genes." << std::endl;
	}

	if(vsXEdges.size() != (int)(sGrid.X*2*(sGrid.Y+1))){
		std::cout << "Wrong number of horizontal edge genes." << std::endl;
	}

	if(vsYEdges.size() != (int)(sGrid.Y*2*(sGrid.X+1))){
		std::cout << "Wrong number of horizontal edge genes." << std::endl;
	}

	if(vsMiddles.size() != (int)(sGrid.X*sGrid.Y*4)){
		std::cout << "Wrong number of middle genes." << std::endl;
	}


	std::cout << vsCorners.size() << " " << vsXEdges.size() << " " << vsYEdges.size() << " " << vsMiddles.size() << std::endl;


	std::cout << GetSCAD(2,2,0) << std::endl << std::endl;
	std::cout << GetSCAD(2,1,0) << std::endl << std::endl;
	std::cout << GetSCAD(2,3,0) << std::endl << std::endl;
	std::cout << GetSCAD(1,2,0) << std::endl << std::endl;
	std::cout << GetSCAD(3,2,0) << std::endl << std::endl;

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

point3D GetCorner(Corner corner, int i, int j, int k){
	point3D sPoint3D;
	int nIndex = 0;

	switch (corner)
	{
		case NW:
			nIndex = 1 + i + j*sGrid.X + j;
			break;
		case NE:
			nIndex = 1 + i + j*sGrid.X + j + 1;
			break;
		case SW:
			nIndex = 1 + i + j*sGrid.X + j + sGrid.X+1;
			break;
		case SE:
			nIndex = 1 + i + j*sGrid.X + j + sGrid.X+2;
			break;
		default:
			std::cout << "Unknown corner type" << std::endl;
			break;
	}

	//std::cout << corner << " " << i << "," << j << ": " << nIndex << std::endl;
	sPoint3D = vsCorners.at(nIndex);

	if(mode2D){
		sPoint3D.Z = 0;
	}

	return sPoint3D;
}


point3D GetXEdge(Corner corner, int i, int j, int k){
	point3D sPoint3D;
	int nIndex = 0;

	switch (corner)
	{
		case NW:
			nIndex = (i + j*sGrid.X)*2 + 1;
			break;
		case NE:
			nIndex = 1 + i + j*sGrid.X + j + 1;
			break;
		case SW:
			nIndex = 1 + i + j*sGrid.X + j + (sGrid.X*2)+1;
			break;
		case SE:
			nIndex = 1 + i + j*sGrid.X + j + (sGrid.X*2)+2;
			break;
		default:
			std::cout << "Unknown corner type" << std::endl;
			break;
	}

	//std::cout << corner << " " << i << "," << j << ": " << nIndex << std::endl;
	sPoint3D = vsXEdges.at(nIndex);

	if(mode2D){
		sPoint3D.Z = 0;
	}

	return sPoint3D;
}

point3D GetYEdge(Corner corner, int i, int j, int k){
	point3D sPoint3D;
	int nIndex = 0;

	switch (corner)
	{
		case NW:
			nIndex = (i + j*sGrid.X)*2 + 1;
			break;
		case NE:
			nIndex = 1 + i + j*sGrid.X + j + 1;
			break;
		case SW:
			nIndex = 1 + i + j*sGrid.X + j + (sGrid.X*2)+1;
			break;
		case SE:
			nIndex = 1 + i + j*sGrid.X + j + (sGrid.X*2)+2;
			break;
		default:
			std::cout << "Unknown corner type" << std::endl;
			break;
	}

	//std::cout << corner << " " << i << "," << j << ": " << nIndex << std::endl;
	sPoint3D = vsYEdges.at(nIndex);

	if(mode2D){
		sPoint3D.Z = 0;
	}

	return sPoint3D;
}
point3D GetZEdge(Corner corner, int i, int j, int k){
	point3D sPoint3D;
	int nIndex = 0;

	switch (corner)
	{
		case NW:
			nIndex = 1 + i + j*sGrid.X + j;
			break;
		case NE:
			nIndex = 1 + i + j*sGrid.X + j + 1;
			break;
		case SW:
			nIndex = 1 + i + j*sGrid.X + j + sGrid.X+1;
			break;
		case SE:
			nIndex = 1 + i + j*sGrid.X + j + sGrid.X+2;
			break;
		default:
			std::cout << "Unknown corner type" << std::endl;
			break;
	}

	//std::cout << corner << " " << i << "," << j << ": " << nIndex << std::endl;
	sPoint3D = vsZEdges.at(nIndex);

	if(mode2D){
		sPoint3D.Z = 0;
	}

	return sPoint3D;
}

point3D GetMiddle(Corner corner, int i, int j, int k){
	point3D sPoint3D;
	int nIndex = 0;

	switch (corner)
	{
		case NW:
			nIndex = (i + j*sGrid.X)*4 + 1 ;
			break;
		case NE:
			nIndex = (i + j*sGrid.X)*4 + 2 ;
			break;
		case SW:
			nIndex = (i + j*sGrid.X)*4 + 3 ;
			break;
		case SE:
			nIndex = (i + j*sGrid.X)*4 + 4 ;
			break;
		default:
			std::cout << "Unknown corner type" << std::endl;
			break;
	}

	//std::cout << corner << " " << i << "," << j << ": " << nIndex << std::endl;
	sPoint3D = vsMiddles.at(nIndex);

	if(mode2D){
		sPoint3D.Z = 0;
	}

	return sPoint3D;
}

controlPoints GetControlPoints(int i, int j, int k){
	controlPoints sControlPoints;

	sControlPoints.a1 = GetCorner(NW, i, j, k);
	sControlPoints.a2 = GetXEdge(NW, i, j, k);
	sControlPoints.a3 = GetXEdge(NE, i, j, k);
	sControlPoints.a4 = GetCorner(NE, i, j, k);

	sControlPoints.b1 = GetYEdge(NW, i, j, k);
	sControlPoints.b2 = GetMiddle(NW, i, j, k);
	sControlPoints.b3 = GetMiddle(NE, i, j, k);
	sControlPoints.b4 = GetYEdge(NE, i, j, k);

	sControlPoints.c1 = GetYEdge(SW, i, j, k);
	sControlPoints.c2 = GetMiddle(SW, i, j, k);
	sControlPoints.c3 = GetMiddle(SE, i, j, k);
	sControlPoints.c4 = GetYEdge(SE, i, j, k);

	sControlPoints.d1 = GetCorner(SW, i, j, k);
	sControlPoints.d2 = GetXEdge(SW, i, j, k);
	sControlPoints.d3 = GetXEdge(SE, i, j, k);
	sControlPoints.d4 = GetCorner(SE, i, j, k);

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
									<< "], steps=16, thickness=100);" << std::endl;

	return ss.str();
}
