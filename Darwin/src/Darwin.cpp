//============================================================================
// Name        : Darwin.cpp
// Author      : Skidzobolder
// Version     :
// Description : main for Darwin
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>


#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;


int status;

void printStatus(){
	cout << "Status: ok16" << endl; // prints !!!Hello World!!!

}

void traverse_xml(const std::string& input_xml)
{
    // (input_xml contains the above XML)

    // make a safe-to-modify copy of input_xml
    // (you should never modify the contents of an std::string directly)
    vector<char> xml_copy(input_xml.begin(), input_xml.end());
    xml_copy.push_back('\0');

    // only use xml_copy from here on!
    xml_document<> doc;
    // we are choosing to parse the XML declaration
    // parse_no_data_nodes prevents RapidXML from using the somewhat surprising
    // behavior of having both values and data nodes, and having data nodes take
    // precedence over values when printing
    // >>> note that this will skip parsing of CDATA nodes <<<
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

    // alternatively, use one of the two commented lines below to parse CDATA nodes,
    // but please note the above caveat about surprising interactions between
    // values and data nodes (also read http://www.ffuts.org/blog/a-rapidxml-gotcha/)
    // if you use one of these two declarations try to use data nodes exclusively and
    // avoid using value()
    //doc.parse<parse_declaration_node>(&xml_copy[0]); // just get the XML declaration
    //doc.parse<parse_full>(&xml_copy[0]); // parses everything (slowest)

    // since we have parsed the XML declaration, it is the first node
    // (otherwise the first node would be our root node)
    string encoding = doc.first_node()->first_attribute("encoding")->value();
    // encoding == "utf-8"

    // we didn't keep track of our previous traversal, so let's start again
    // we can match nodes by name, skipping the xml declaration entirely
    xml_node<>* cur_node = doc.first_node("rootnode");
    string rootnode_type = cur_node->first_attribute("type")->value();
    // rootnode_type == "example"

    // go straight to the first evendeepernode
    cur_node = cur_node->first_node("childnode")->first_node("evendeepernode");
    string attr2 = cur_node->first_attribute("attr2")->value();
    // attr2 == "dog"
    std::cout << attr2 << endl;


    // and then to the second evendeepernode
    cur_node = cur_node->next_sibling("evendeepernode");
    attr2 = cur_node->first_attribute("attr2")->value();
    // now attr2 == "wolf"
    std::cout << attr2 << endl;

}


int main() {
	xml_document<> doc;

	// xml declaration
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	// root node
	xml_node<>* root = doc.allocate_node(node_element, "rootnode");
	root->append_attribute(doc.allocate_attribute("version", "1.0"));
	root->append_attribute(doc.allocate_attribute("type", "example"));
	doc.append_node(root);

	// child node
	xml_node<>* child = doc.allocate_node(node_element, "childnode");
	root->append_node(child);

	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	std::string xml_no_indent;
	// print_no_indenting is the only flag that print() knows about
	print(std::back_inserter(xml_no_indent), doc, print_no_indenting);
	// xml_no_indent now contains non-indented XML

	cout << xml_as_string << endl;

	std::ifstream t("C:\\workspaces\\Darwin\\Darwin\\src\\config.xml");
	std::stringstream buffer;
	buffer << t.rdbuf();


	std::cout << buffer.str() << endl;
	traverse_xml(buffer.str());


	printStatus();
	return 0;
}


