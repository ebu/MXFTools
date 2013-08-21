#ifndef CPP_EBUCOREPARSER_CLASS_H
#define CPP_EBUCOREPARSER_CLASS_H

/*!
 * \project_name EBU Player
 * \file EBUCoreParser.hpp
 * \brief EBUCore Schema Parser specifications
 * \details This class is used to parse and load the EBUCore Schema
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 20 august 2012
 * \dateOfUpdate 26 september 2012
 * \copyright GNU GPLv3
 *
 */

#ifdef _WIN32
	#include <windows.h>
	#include "genericFeatures.hpp"
#else
	#include <sys/types.h>
	#include <dirent.h>
#endif
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <xercesc/dom/DOM.hpp>

/*! \class ebucoreParser
 * \brief this class represent the ebucore parser
 *
 * This class parses and loads the ebucore elements and their attributes
 */

class ebucoreParser {
	public:
  ebucoreParser(void);
	~ebucoreParser(void);
protected:

	struct AttributeStruct {
		std::string name;
		std::string type;
		bool hasDefaultValue;
		std::string defaultValue;
		bool hasEnumeration;
		std::vector<std::string> Enumeration;
	};

	struct ElementStruct {
		std::string name;
		std::string type;
		std::list<AttributeStruct> attribute;
		std::list<ElementStruct> children;		
		int minCardinality;
		int maxCardinality;
		bool leaf;
	};

	std::vector<ElementStruct> ebucoremodel; // ebucore model 
	std::vector<std::string> ebucoreStack;  //ebucore stack 

	int getSchemas (std::string dir, std::vector<std::string> &files);
	bool isExtension(std::string str, std::string extension);
	bool isDCSimpleType(std::string str);
	bool isStandardType(std::string str);
	bool isEBUCoreType(std::string str);
	int isUnbounded(std::string maximum);

	bool groupExist(std::string str);
	std::string removePrefix(std::string str, std::string prefix);
	void extractSchema(std::string pathtofile);

	AttributeStruct DCAttribute(void);
	std::list<AttributeStruct> DCAttr;

	std::string DCType(void);
	AttributeStruct packAttribute(xercesc::DOMElement * el);
	std::list<AttributeStruct> generateAttributes(std::string father, xercesc::DOMElement * el);
	std::list<ElementStruct> generateChildren(std::string father, xercesc::DOMElement * el, int level);
	void generateGroupChildren(std::list<ElementStruct> children, std::string father, xercesc::DOMElement * el);

	std::vector<std::string> listEnumeration(xercesc::DOMElement * el);
	ElementStruct constructSchema(xercesc::DOMElement * el);
	void generateSkeletonElement(ElementStruct skeleton);
	void generateSkeleton(void);
std::ofstream out;
	
};

#endif
