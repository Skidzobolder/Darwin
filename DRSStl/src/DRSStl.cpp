//============================================================================
// Name        : DRSStl.cpp
// Author      : Wouter Glorieux
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <list>
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

struct grid
{
	int X;
	int Y;
	int Z;
};
grid sGrid;

class cube
{
public:
	int i;
	int j;
	int k;

	double distanceFromSeed(){
		double xd = i-sGrid.X/2;
		double yd = j-sGrid.Y/2;
		double zd = k-sGrid.Z/2;
		double distance = sqrt(xd*xd + yd*yd + zd*zd);

		return distance;
	}

	  bool operator<(const cube& rhs) const
	  {
		/*int xd1 = i-rhs.i;
		int yd1 = j-rhs.j;
		int zd1 = k-rhs.k;
		int distance = xd1+yd1+zd1;

		int xd2 = i-sGrid.X/2;
		int yd2 = j-sGrid.Y/2;
		int zd2 = k-sGrid.Z/2;
		double distanceRhs = xd2+yd2+zd2;

		return distance > distanceRhs;

*/



	    if(i != rhs.i){
	    	return i < rhs.i;
	    }
	    else if(j != rhs.j){
	    	return j < rhs.j;
	    }
	    else{
	    	return k < rhs.k;
	    }

	  }

	  bool operator==(const cube& rhs) const
	  {
	    return i == rhs.i && j == rhs.j && k == rhs.k;
	  }

};

enum ORIENTATION
{
	XPOS = 1,
	XNEG = 2,
	YPOS = 3,
	YNEG = 4,
	ZPOS = 5,
	ZNEG = 6
};

struct segment
{
	double i;
	double j;
	double k;
	ORIENTATION orientation;
};


std::vector<segment> vsSegments;

void StringExplode(std::string str, std::string separator, std::vector<std::string>* results);
void InitializeGrid(point3D***asPoints);
std::string SquareToFacets(point3D a, point3D b, point3D c, point3D d, ORIENTATION orentation );
void MakeSTL(std::vector<segment> vsSegments, std::string outputFileName, point3D***asPoints);

std::list<cube> SelectCubes(bool*** abGrid, cube seed, int maxCubes);
std::vector<segment> SelectSegments(std::list<cube> listCubes);



//point3D asPoints[100][100][100];			//3d array of co�rdinates of all points
std::vector<point3D> vsPoints;      //vector with all points


int main(int argc, char *argv[]) {

	std::string strFileName = "";
	std::string strOutputFileName = "";


	double dXmultiplier = 1;
	double dYmultiplier = 1;
	double dZmultiplier = 1;


	if(argc == 3){
		strFileName= argv[1];
		strOutputFileName= argv[2];
	}
	else{
		std::cout << "Usage: DRSStl.exe inputFilename outputFilename" << std::endl;
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


    sGrid.X = 0;
    sGrid.Y = 0;
    sGrid.Z = 0;

    std::vector<std::string> vstrData;
    std::vector< std::vector<std::string> > vstrGrid;


    int k = 0;
	for (rapidxml::xml_node<>* chromosomeNode = rootNode->first_node("Chromosome"); chromosomeNode; chromosomeNode = chromosomeNode->next_sibling("Chromosome"))
	{

		if(chromosomeNode->first_attribute("id")->value() == std::string("grid")){

			int j = 0;
			for (rapidxml::xml_node<>* geneNode = chromosomeNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
			{

				std::string strData = geneNode->value();
				if( (int)strData.size() > sGrid.X){
						sGrid.X = strData.size();
				}
				vstrData.push_back(strData);

				for(unsigned int i = 0; i < strData.size(); i++){
					//abGrid[i][j][k] = strData.at(i) == '1'? true : false ;
					//std::cout << i << "-" << j << "-" << k << ": " << abGrid[i][j][k] << std::endl;
 				}
				j++;
				if(j > sGrid.Y){
					sGrid.Y = j;
				}
			}
			k++;
			if(k > sGrid.Z){
				sGrid.Z = k;
			}

			vstrGrid.push_back(vstrData);
			vstrData.clear();

		}
		else{
			for (rapidxml::xml_node<>* geneNode = chromosomeNode->first_node("Gene"); geneNode; geneNode = geneNode->next_sibling("Gene"))
			{
				std::vector<std::string> vstrData;
				StringExplode(geneNode->value(), "|", &vstrData);
				point3D sPoint3D;
				sPoint3D.X = atof(vstrData.at(0).c_str()) * dXmultiplier;
				sPoint3D.Y = atof(vstrData.at(1).c_str()) * dYmultiplier;
				sPoint3D.Z = atof(vstrData.at(2).c_str()) * dZmultiplier;

				vsPoints.push_back(sPoint3D);

			}
		}
	}

	//std::cout << sGrid.X << " " << sGrid.Y << " " << sGrid.Z << std::endl;
	const int x = sGrid.X;
	const int y = sGrid.Y;
	const int z = sGrid.Z;


	    //  Array Iterators
	    int i, j;


	   //  Allocate 3D Array
	    bool ***abGrid = new bool**[x];
	    point3D *** asPoints = new point3D**[x+1];

	   for(i = 0; i < x; i++)
	   {
	       abGrid[i] = new bool*[y];
	       for(j = 0; j < y; j++)
	        {
	           abGrid[i][j] = new bool[z];
	       }
	    }

	    //  initialize array elements
	    for(i = 0; i < x; i++)
	    {
	        for(j = 0; j < y; j++)
	        {
	            for(k = 0; k < z; k++)
	            {
	                abGrid[i][j][k] = false;
	            }
	        }
	    }


		   for(i = 0; i <= x; i++)
		   {
		       asPoints[i] = new point3D*[y+1];
		       for(j = 0; j <= y; j++)
		        {
		           asPoints[i][j] = new point3D[z+1];
		       }
		    }

		    //  initialize array elements
		    for(i = 0; i <= x; i++)
		    {
		        for(j = 0; j <= y; j++)
		        {
		            for(k = 0; k <= z; k++)
		            {
		                point3D sPoint3D;
		                sPoint3D.X = 0;
		                sPoint3D.Y = 0;
		                sPoint3D.Z = 0;
		                asPoints[i][j][k]= sPoint3D;
		            }
		        }
		    }



    //enable or disable cubes based on the genome
	for( unsigned int l = 0; l < vstrGrid.size(); l++){
		vstrData = vstrGrid.at(l);
		for(unsigned int m = 0; m <vstrData.size(); m++){
			std::string strData = vstrData.at(m);
			for(unsigned int o = 0; o < strData.size(); o++){
				//std::cout << o << "-" << m << "-" << l << std::endl;
				abGrid[o][m][l] = strData.at(o) == '1' ? true : false;
			}
		}
	}



	//Prevent bad stl's by disabling cubes that have a single edge in common
	for( i = 1; i < sGrid.X-1; i++){
		for(j = 1; j < sGrid.Y-1; j++){
			for(k = 1; k < sGrid.Z-1; k++){

					if(abGrid[i][j][k] && abGrid[i+1][j+1][k] && !abGrid[i+1][j][k] && !abGrid[i][j+1][k]){
						abGrid[i+1][j][k]=true;
						abGrid[i][j+1][k]=true;
						//abGrid[i+1][j+1][k] = false;
					}

					if(abGrid[i][j][k] && abGrid[i-1][j-1][k] && !abGrid[i-1][j][k] && !abGrid[i][j-1][k]){
						abGrid[i-1][j][k]=true;
						abGrid[i][j-1][k]=true;
						//abGrid[i-1][j-1][k] = false;
					}

					if( abGrid[i][j][k] && abGrid[i+1][j-1][k] && !abGrid[i+1][j][k] && !abGrid[i][j-1][k]){
						abGrid[i+1][j][k]=true;
						abGrid[i][j-1][k]=true;
						//abGrid[i+1][j-1][k] = false;
					}

					if( abGrid[i][j][k] && abGrid[i-1][j+1][k] && !abGrid[i-1][j][k] && !abGrid[i][j+1][k]){
						abGrid[i-1][j][k]=true;
					    abGrid[i][j+1][k]=true;
						//abGrid[i-1][j+1][k] = false;
					}


				//------------------------------------------------------------------------------------------------------------

					if( abGrid[i][j][k] && abGrid[i+1][j][k+1] && !abGrid[i+1][j][k] && !abGrid[i][j][k+1]){
						abGrid[i+1][j][k]=true;
						abGrid[i][j][k+1]=true;
						//abGrid[i+1][j][k+1] = false;
					}

					if(abGrid[i][j][k] && abGrid[i-1][j][k-1] && !abGrid[i-1][j][k] && !abGrid[i][j][k-1]){
						abGrid[i-1][j][k]=true;
						abGrid[i][j][k-1]=true;
						//abGrid[i-1][j][k-1] = false;
					}

					if(abGrid[i][j][k] && abGrid[i+1][j][k-1] && !abGrid[i+1][j][k] && !abGrid[i][j][k-1]){
						abGrid[i+1][j][k]=true;
						abGrid[i][j][k-1]=true;
						//abGrid[i+1][j][k-1] = false;
					}

					if(abGrid[i][j][k] && abGrid[i-1][j][k+1] && !abGrid[i-1][j][k] && !abGrid[i][j][k+1]){
						abGrid[i-1][j][k]=true;
						abGrid[i][j][k+1]=true;
						//abGrid[i-1][j][k+1] = false;
					}



				//------------------------------------------------------------------------------------------------------------


					if(abGrid[i][j][k] && abGrid[i][j+1][k+1] && !abGrid[i][j+1][k] && !abGrid[i][j][k+1]){
						abGrid[i][j+1][k]=true;
						abGrid[i][j][k+1]=true;
						//abGrid[i][j+1][k+1] = false;
					}

					if(abGrid[i][j][k] && abGrid[i][j-1][k-1] && !abGrid[i][j-1][k] && !abGrid[i][j][k-1]){
						abGrid[i][j-1][k]=true;
						abGrid[i][j][k-1]=true;
						//abGrid[i][j-1][k-1] = false;
					}

					if(abGrid[i][j][k] && abGrid[i][j+1][k-1] && !abGrid[i][j+1][k] && !abGrid[i][j][k-1]){
						abGrid[i][j+1][k]=true;
						abGrid[i][j][k-1]=true;
						//abGrid[i][j+1][k-1] = false;
					}

					if(abGrid[i][j][k] && abGrid[i][j-1][k+1] && !abGrid[i][j-1][k] && !abGrid[i][j][k+1]){
						abGrid[i][j-1][k]=true;
						abGrid[i][j][k+1]=true;
						//abGrid[i][j-1][k+1] = false;
					}
			}
		}
	}




	//let the stl grow from a seed in the middle
	cube seed;
	seed.i = (int) sGrid.X/2;
	seed.j = (int) sGrid.Y/2;
	seed.k = (int) sGrid.Z/2;

   std::list<cube> listCubes = SelectCubes(abGrid, seed, 100);
   std::vector<segment> vsSegments = SelectSegments(listCubes);

	InitializeGrid(asPoints);

	MakeSTL(vsSegments, strOutputFileName, asPoints);





	//  Deallocate 3D array abGrid
	for(i = 0; i < x; i++)
	{
	    for(j = 0; j < y; j++)
	    {
	        delete[] abGrid[i][j];
	    }
      delete[] abGrid[i];
    }
	delete[] abGrid;


	//  Deallocate 3D array asPoints
	for(i = 0; i <= x; i++)
	{
	    for(j = 0; j <= y; j++)
	    {
	        delete[] asPoints[i][j];
	    }
      delete[] asPoints[i];
    }
	delete[] asPoints;


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





void InitializeGrid(point3D***asPoints){
	int nPosition = 0;

	for(int i=0; i <= sGrid.X; i++){

		for (int j = 0 ; j <= sGrid.Y ; j++){

			for (int k = 0 ; k <= sGrid.Z; k++){

				int nSize = vsPoints.size();

				if(nPosition >= nSize){
					nPosition = 0;
				}
				point3D stmpPoint3D = vsPoints.at(nPosition);
				nPosition++;

				point3D sPoint3D;
				sPoint3D.X = i*1 + stmpPoint3D.X/20000;
				sPoint3D.Y = j*1+ stmpPoint3D.Y/20000;
				sPoint3D.Z = k*1+ stmpPoint3D.Z/20000;
				asPoints[i][j][k] = sPoint3D;
			}
		}
	}
}

std::string SquareToFacets(point3D a, point3D b, point3D c, point3D d, ORIENTATION orientation ){
	std::stringstream ss;
	std::string strFacet = "";

	switch (orientation)
	    {
	        case XPOS:
	        	strFacet = "1 0 0";
	            break;

	        case XNEG:
	        	strFacet = "-1 0 0";
	            break;

	        case YPOS:
	        	strFacet = "0 1 0";
	            break;

	        case YNEG:
	        	strFacet = "0 -1 0";
	            break;

	        case ZPOS:
	        	strFacet = "0 0 1";
	            break;

	        case ZNEG:
	        	strFacet = "0 0 -1";
	            break;

	        default:
	            std::cout << "Unknown orientation" << std::endl;
	            break;
	    }

	ss << "facet normal " << strFacet << std::endl;
	ss << "outer loop" << std::endl;
	if(orientation == XPOS || orientation == YNEG || orientation == ZPOS){
		ss << "vertex " << b.X << " " << b.Y << " " << b.Z << std::endl;
		ss << "vertex " << a.X << " " << a.Y << " " << a.Z << std::endl;
		ss << "vertex " << c.X << " " << c.Y << " " << c.Z << std::endl;
	}
	else if(orientation == XNEG || orientation == YPOS || orientation == ZNEG){
		ss << "vertex " << a.X << " " << a.Y << " " << a.Z << std::endl;
		ss << "vertex " << b.X << " " << b.Y << " " << b.Z << std::endl;
		ss << "vertex " << c.X << " " << c.Y << " " << c.Z << std::endl;
	}
	ss << "endloop" << std::endl;
	ss << "endfacet" << std::endl;

	ss << "facet normal " << strFacet << std::endl;
	ss << "outer loop" << std::endl;
	if(orientation == XPOS || orientation == YNEG || orientation == ZPOS){
		ss << "vertex " << b.X << " " << b.Y << " " << b.Z << std::endl;
		ss << "vertex " << c.X << " " << c.Y << " " << c.Z << std::endl;
		ss << "vertex " << d.X << " " << d.Y << " " << d.Z << std::endl;
	}
	else if(orientation == XNEG || orientation == YPOS || orientation == ZNEG){
		ss << "vertex " << b.X << " " << b.Y << " " << b.Z << std::endl;
		ss << "vertex " << d.X << " " << d.Y << " " << d.Z << std::endl;
		ss << "vertex " << c.X << " " << c.Y << " " << c.Z << std::endl;
	}
	ss << "endloop" << std::endl;
	ss << "endfacet" << std::endl;

	return ss.str();
}


void MakeSTL(std::vector<segment> vsSegments, std::string outputFileName, point3D***asPoints){
	std::ofstream output(outputFileName.c_str());
	output << "solid mystl" << std::endl;



    for(unsigned int l = 0; l < vsSegments.size(); l++ ){
    	//std::cout << vsSegments.at(l).i << "-" << vsSegments.at(l).j << "-" << vsSegments.at(l).k << " " << vsSegments.at(l).orientation << std::endl;

    	int i, j, k;
    	i = vsSegments.at(l).i;
    	j = vsSegments.at(l).j;
    	k = vsSegments.at(l).k;

    	switch (vsSegments.at(l).orientation)
    	    {
    	        case XPOS:
    	        	output << SquareToFacets(asPoints[i+1][j+1][k+1], asPoints[i+1][j+1][k], asPoints[i+1][j][k+1], asPoints[i+1][j][k], XPOS);
    	            break;

    	        case XNEG:
    	        	output << SquareToFacets(asPoints[i][j+1][k+1], asPoints[i][j+1][k], asPoints[i][j][k+1], asPoints[i][j][k], XNEG);
    	            break;

    	        case YPOS:
    	        	output << SquareToFacets(asPoints[i][j+1][k+1], asPoints[i+1][j+1][k+1], asPoints[i][j+1][k], asPoints[i+1][j+1][k], YPOS);
    	        	break;

    	        case YNEG:
    	        	output << SquareToFacets(asPoints[i][j][k+1], asPoints[i+1][j][k+1], asPoints[i][j][k], asPoints[i+1][j][k], YNEG);
    	            break;

    	        case ZPOS:
    	        	output << SquareToFacets(asPoints[i][j+1][k+1], asPoints[i+1][j+1][k+1], asPoints[i][j][k+1], asPoints[i+1][j][k+1], ZPOS);
    	            break;

    	        case ZNEG:
    	        	output << SquareToFacets(asPoints[i][j+1][k], asPoints[i+1][j+1][k], asPoints[i][j][k], asPoints[i+1][j][k], ZNEG);
    	            break;

    	        default:
    	            std::cout << "Unknown orientation" << std::endl;
    	            break;
    	    }



    }


	output << "endsolid mystl" << std::endl;
	output.close();

	//std::cout << "writing " << outputFileName << std::endl;

}

std::list<cube>::iterator FindCube(std::list<cube>* listCubes, cube Cube){
	std::list<cube>::iterator it;

	for( it = listCubes->begin(); it != listCubes->end(); it++ ) {
		if((it->i == Cube.i) && (it->j == Cube.j) && (it->k == Cube.k)){
			return it;
		}
	}
	return listCubes->end();

}


std::list<cube>::iterator FindCubePlace(std::list<cube>* listCubes, cube Cube){
	std::list<cube>::iterator it;

	for( it = listCubes->begin(); it != listCubes->end(); it++ ) {
		//std::cout << it->distanceFromSeed() << " " << Cube.distanceFromSeed() << std::endl;

		if(it->distanceFromSeed() > Cube.distanceFromSeed()){
			return it;
		}
	}

	return listCubes->end();

}


std::list<cube> SelectCubes(bool*** abGrid, cube seed, int maxCubes){
	cube sCube;
	std::list<cube> listCubes;

	abGrid[seed.i][seed.j][seed.k] = true;

	//std::cout << "seed: " << seed.i << "-" << seed.j << "-" << seed.k << std::endl;

	sCube.i = seed.i;
	sCube.j = seed.j;
	sCube.k = seed.k;
	listCubes.insert(listCubes.end(), sCube);



	bool bCubesAdded = true;

	std::list<cube>::iterator it;
	while(bCubesAdded && ((int)listCubes.size() < maxCubes)){
		//std::cout << setCubes.size() << std::endl;
		unsigned int nCubes = listCubes.size();

		bCubesAdded = false;
		for( it = listCubes.begin(); it != listCubes.end(); it++ ) {
			if(it->i+1 < sGrid.X && abGrid[it->i+1][it->j][it->k] == true){
				sCube.i = it->i+1;
				sCube.j = it->j;
				sCube.k = it->k;
				if((int)listCubes.size() < maxCubes && FindCube(&listCubes, sCube) == listCubes.end()){
					listCubes.insert(FindCubePlace(&listCubes, sCube), sCube);
				}

			}

			if(it->j+1 < sGrid.Y && abGrid[it->i][it->j+1][it->k] == true){
				sCube.i = it->i;
				sCube.j = it->j+1;
				sCube.k = it->k;
				if((int)listCubes.size() < maxCubes && FindCube(&listCubes, sCube) == listCubes.end()){
					listCubes.insert(FindCubePlace(&listCubes, sCube), sCube);
				}
			}

			if(it->k+1 < sGrid.Z && abGrid[it->i][it->j][it->k+1] == true){
				sCube.i = it->i;
				sCube.j = it->j;
				sCube.k = it->k+1;
				if((int)listCubes.size() < maxCubes && FindCube(&listCubes, sCube) == listCubes.end()){
					listCubes.insert(FindCubePlace(&listCubes, sCube), sCube);
				}
			}

			if(it->i-1 >= 0 && abGrid[it->i-1][it->j][it->k] == true){
				sCube.i = it->i-1;
				sCube.j = it->j;
				sCube.k = it->k;
				if((int)listCubes.size() < maxCubes && FindCube(&listCubes, sCube) == listCubes.end()){
					listCubes.insert(FindCubePlace(&listCubes, sCube), sCube);
				}
			}

			if(it->j-1 >= 0 && abGrid[it->i][it->j-1][it->k] == true){
				sCube.i = it->i;
				sCube.j = it->j-1;
				sCube.k = it->k;
				if((int)listCubes.size() < maxCubes && FindCube(&listCubes, sCube) == listCubes.end()){
					listCubes.insert(FindCubePlace(&listCubes, sCube), sCube);
				}
			}

			if(it->k-1 >= 0 && abGrid[it->i][it->j][it->k-1] == true){
				sCube.i = it->i;
				sCube.j = it->j;
				sCube.k = it->k-1;
				if((int)listCubes.size() < maxCubes && FindCube(&listCubes, sCube) == listCubes.end()){
					listCubes.insert(FindCubePlace(&listCubes, sCube), sCube);
				}
			}
		}


		if(nCubes < listCubes.size()){
			bCubesAdded = true;
		}
	}


/*	for( it = listCubes.begin(); it != listCubes.end(); it++ ) {
		std::cout << it->i << "-" << it->j << "-" << it->k << " : " << it->distanceFromSeed() << std::endl;
	}
*/
	//std::cout << setCubes.size() << std::endl;
	return listCubes;
}




std::vector<segment> SelectSegments(std::list<cube> listCubes){
	std::vector<segment> vsSegments;
	std::list<cube>::iterator it;
	std::list<cube>::const_iterator cit;

	for( it = listCubes.begin(); it != listCubes.end(); it++ ) {
		//std::cout << it->i << "-" << it->j << "-" << it->k << std::endl;

		segment sSegment;
		sSegment.i = it->i;
		sSegment.j = it->j;
		sSegment.k = it->k;

		cube adjCube;
		adjCube.i = it->i+1;
		adjCube.j = it->j;
		adjCube.k = it->k;

		cit = FindCube(&listCubes, adjCube) ;
		if(cit == listCubes.end()){
			sSegment.orientation = XPOS;
			vsSegments.push_back(sSegment);
		}

		adjCube.i = it->i;
		adjCube.j = it->j+1;
		adjCube.k = it->k;

		cit = FindCube(&listCubes, adjCube) ;
		if(cit == listCubes.end()){
			sSegment.orientation = YPOS;
			vsSegments.push_back(sSegment);
		}

		adjCube.i = it->i;
		adjCube.j = it->j;
		adjCube.k = it->k+1;

		cit = FindCube(&listCubes, adjCube) ;
		if(cit == listCubes.end()){
			sSegment.orientation = ZPOS;
			vsSegments.push_back(sSegment);
		}

		adjCube.i = it->i-1;
		adjCube.j = it->j;
		adjCube.k = it->k;

		cit = FindCube(&listCubes, adjCube) ;
		if(cit == listCubes.end()){
			sSegment.orientation = XNEG;
			vsSegments.push_back(sSegment);
		}

		adjCube.i = it->i;
		adjCube.j = it->j-1;
		adjCube.k = it->k;

		cit = FindCube(&listCubes, adjCube) ;
		if(cit == listCubes.end()){
			sSegment.orientation = YNEG;
			vsSegments.push_back(sSegment);
		}

		adjCube.i = it->i;
		adjCube.j = it->j;
		adjCube.k = it->k-1;

		cit = FindCube(&listCubes, adjCube) ;
		if(cit == listCubes.end()){
			sSegment.orientation = ZNEG;
			vsSegments.push_back(sSegment);
		}

	}

	return vsSegments;

}

