/*!
 * \project_name EBU Player
 * \file ebucoreParser.cpp
 * \brief EBUCore Parser body
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 20 august 2012
 * \dateOfUpdate 26 september 2012
 * This software is published in LGPLv3.0
 *
 */

// include files and libraries
#include "ebucoreParser.hpp"


// class constructor
ebucoreParser::ebucoreParser(void)
{
	// EBUcore Schemas folder
	std::string dir = std::string("./EBUCoreSchema");
	// Schemas filenames
	std::vector<std::string> files = std::vector<std::string>();
	// Get the schemas filenames
	getSchemas(dir,files);
	//prepare dublin core attributes
	DCAttr.push_back(DCAttribute());
	// extract all schemas
	for (unsigned int i = 0;i < files.size();i++) {
		std::cout << files[i] << std::endl;
		std::cout<< "start parsing..."<<std::endl;
		extractSchema(dir+"/"+files[i]);
	}
	// clean the stack and reuse it for skeleton generation
	ebucoreStack.clear();
	generateSkeleton();
}

// class destructor
ebucoreParser::~ebucoreParser(void)
{
	ebucoreStack.clear();
	ebucoremodel.clear();
}

#ifdef _WIN32
	int ebucoreParser::getSchemas(std::string dir, std::vector<std::string> &files)
	{
		 WIN32_FIND_DATA FindFileData;
		 wchar_t * FileName = genericFeatures::string2wchar_t(dir);
		 HANDLE hFind = FindFirstFile(FileName, &FindFileData);
		if (isExtension(genericFeatures::wchar_t2string(FindFileData.cFileName), "xsd")) {
			files.push_back(genericFeatures::wchar_t2string(FindFileData.cFileName));
		}
		 while (FindNextFile(hFind, &FindFileData)) {
			std::string path(genericFeatures::wchar_t2string(FindFileData.cFileName));
			if (isExtension(path, "xsd")) {
				files.push_back(path);
			}
		}
		 return 0;
	}
#else
	// get the path to the schemas
	int ebucoreParser::getSchemas (std::string dir, std::vector<std::string> &files) {
		DIR *dp;
		struct dirent *dirp;
		// if opening folder fail then
		if((dp = opendir(dir.c_str())) == NULL) {
			// display error message
			std::cout << "Error(" << errno << ") opening " << dir << std::endl;
			return errno;
		}
		// while not the end of folder
		while ((dirp = readdir(dp)) != NULL) {
			// if it is am xsd file then
			if (dirp->d_type == DT_REG && isExtension(dirp->d_name, "xsd"))
				// save the schema filename inside the vector
				files.push_back(std::string(dirp->d_name));
		}
		closedir(dp); // close folder
		return 0;
	}
#endif

// extract a schema
void ebucoreParser::extractSchema(std::string pathtofile) {
	// init xercesc
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xsd = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xsd->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	// extract xml DOM document and element
	xercesc::DOMDocument* schema_doc  = dom_file->parseURI(pathtofile.c_str());
	xercesc::DOMElement*  schema_root = schema_doc->getDocumentElement();
	// put the cursor at the beginning
	schema_root = schema_root->getFirstElementChild();
	schema_root = schema_root->getNextElementSibling()->getNextElementSibling();
	// generate and store the ebucore root
	ebucoremodel.push_back(constructSchema(schema_root));
}

// wrap element attribute
ebucoreParser::AttributeStruct ebucoreParser::packAttribute(xercesc::DOMElement * el) {
	// prepare the attribute struct
	ebucoreParser::AttributeStruct attr;
	// grab the attributes
	std::string name (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("name"))));
	std::string ref (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("ref"))));
	std::string type (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))));	
	std::string byDefault (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("default"))));
	// wrap the attribute
	attr.name = ((name.size()>0)?name:ref);
	attr.type = ((type.size()>0)?type:"string");
	attr.hasDefaultValue = ((byDefault.size()>0)?true:false);
	attr.defaultValue = ((byDefault.size()>0)?byDefault:"NULL");
	// wrap the attribute enumeration
	attr.hasEnumeration = false;
	std::vector<std::string> attrEnum;
	attr.Enumeration = attrEnum;
	// return the attribute
	return attr;
}

// generate the attributes of an element
std::list<ebucoreParser::AttributeStruct> ebucoreParser::generateAttributes(std::string father, xercesc::DOMElement * el) {
	// groupName to store the attributeGroups
	std::vector<std::string> groupName;
	// attributes list
	std::list<ebucoreParser::AttributeStruct> attributes;
	// when father type hasn't a name, that means the type is implicit
	if (father =="") {
		// we must save the current position
		xercesc::DOMElement * tmp = el->getFirstElementChild();
		// and looking for the implicit father complexType (without name)
		if ((std::string)(xercesc::XMLString::transcode(tmp->getTagName())) == "complexType" ) {
			// when the complexType is identified, put the cursor inside the first child
			tmp = tmp->getFirstElementChild();
			// while xercesc pointer not null
			while (tmp != 0) {
				// if attribute is a simpleContent
				if ((std::string)(xercesc::XMLString::transcode(tmp->getTagName())) == "simpleContent") {
					// grab the first child of the first child of the current element
					xercesc::DOMElement * localTmp = tmp->getFirstElementChild()->getFirstElementChild();
					// if xercersc pointer not null
					while (localTmp != 0) {
						// if attribute is a simple attribute
						if ((std::string)(xercesc::XMLString::transcode(localTmp->getTagName())) == "attribute" ) {
							// wrap and push the attribute
							attributes.push_back(packAttribute(localTmp));			
						// if attribute is an attributeGroup						
						} else if ((std::string)(xercesc::XMLString::transcode(localTmp->getTagName())) == "attributeGroup" ) {
							// push the attributeGroup name inside the attributeGroup stack
							groupName.push_back(removePrefix ((std::string)(xercesc::XMLString::transcode(localTmp->getAttribute (xercesc::XMLString::transcode("ref")))),":" ));
						}
						// next element...
						localTmp = localTmp->getNextElementSibling();
					}
				// if attribute is a complexContent
				} else if ((std::string)(xercesc::XMLString::transcode(tmp->getTagName())) == "complexContent") {
					// grab the first child of the current element
					xercesc::DOMElement * localTmp = tmp->getFirstElementChild();
					// if it is a DublinCore attribute
					if (isDCSimpleType((std::string)(xercesc::XMLString::transcode(localTmp->getAttribute (xercesc::XMLString::transcode("base")))))) {
						//push the dublincore attribute
						attributes.push_back(DCAttribute());
					} else {
						// if it is not a dublincore attribute, that means that is an attributeGroup or external attribute
						groupName.push_back(removePrefix ((std::string)(xercesc::XMLString::transcode(localTmp->getAttribute (xercesc::XMLString::transcode("base")))),":" ));
					}
					// enter inside the current element
					localTmp = localTmp->getFirstElementChild();
					// in plus of external attribute and attributeGroup treatments, we can have 
					// several local attributes and attributeGroups to solve 
					// if xercesc pointer is not null
					while (localTmp != 0) {
					// if it is a simple attribute
						if ((std::string)(xercesc::XMLString::transcode(localTmp->getTagName())) == "attribute" ) {
							// wrap and push the attribute
							attributes.push_back(packAttribute(localTmp));
						// if it is an attributeGroup
						} else if ((std::string)(xercesc::XMLString::transcode(localTmp->getTagName())) == "attributeGroup" ) {
							// save the attributeGroup name/reference...
							groupName.push_back(removePrefix ((std::string)(xercesc::XMLString::transcode(localTmp->getAttribute (xercesc::XMLString::transcode("ref")))),":"));
						}
						// next element...
						localTmp = localTmp->getNextElementSibling();
					}				
				// if attribute is an attributeGroup
				} else if ((std::string)(xercesc::XMLString::transcode(tmp->getTagName())) == "attributeGroup"){
					// save the attributeGroup name/reference
					groupName.push_back(removePrefix ((std::string)(xercesc::XMLString::transcode(tmp->getAttribute (xercesc::XMLString::transcode("ref")))), ":" ));				
				// if attribute is a simple attribute
				} else if ((std::string)(xercesc::XMLString::transcode(tmp->getTagName())) == "attribute") {
					// wrap and push the attribute
					attributes.push_back(packAttribute(tmp));
				}
				// next...
				tmp = tmp->getNextElementSibling();		
			}
		}
	}

	// We don't know if the father is an implicit complexType who references
	// an external attributeGroup, so we consider it as an external references
	if (father!="") { groupName.push_back(father);}

	// for each external attributeGroup, build the set of attributes
	for (int i=0;i<(int)groupName.size();i++) {
		// pointer starter...
		xercesc::DOMElement * tmpEl = el;
		// looking for the root of the schema document
		while ((std::string)(xercesc::XMLString::transcode((tmpEl->getParentNode())->getNodeName())) != "schema") {
			tmpEl = dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode());
		}
		
		// grab the first child of the schema
		tmpEl=(dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode()))->getFirstElementChild();
		
		// looking for a complexType or an attributeGroup with the right name 
		while (((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" && xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != groupName.at(i) ) || ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "attributeGroup" && xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != groupName.at(i)) ) {
			tmpEl = tmpEl->getNextElementSibling();
		}
		
		//grab the first child
		tmpEl=tmpEl->getFirstElementChild();	

		// while xerces pointer not null
		do {
			// and if the element is an attribute
			if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "attribute") {
				// wrap and push the attribute
				attributes.push_back(packAttribute(tmpEl));
			}
			// next
			tmpEl = tmpEl->getNextElementSibling();
		} while (tmpEl != 0);
	}

	return attributes;

}

// generate the children of an elements
std::list<ebucoreParser::ElementStruct> ebucoreParser::generateChildren(std::string father, xercesc::DOMElement * el, int level) {
	// instantiate the children list
	std::list<ebucoreParser::ElementStruct> children;
	// copy the current position of xercesc pointer
	xercesc::DOMElement * tmpEl = el;
	// while xercesc pointer not null, element different of complexType with the proper name
	while (tmpEl != 0  && ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" || xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father )) {
		tmpEl = tmpEl->getNextElementSibling();// next
	}
	// grab the first child of proper element
	tmpEl=tmpEl->getFirstElementChild();
	// looking for the sequence of elements
	while (tmpEl != 0 && (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence") {
		tmpEl = tmpEl->getNextElementSibling(); // next
	}
	// if pointer not null, grab the first child of the current element
	if (tmpEl != 0) {
		tmpEl=tmpEl->getFirstElementChild();
	}
	
	// while xerces point not null, loop
	while (tmpEl != 0) {
		// if current tag kind is element
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element") {
			// prepare the element information
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			std::string type (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("type"))));
			std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
			std::string minimum (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
			std::string maximum (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
			// instantiate the children struct
			ebucoreParser::ElementStruct internalChildren;
			// grab the right name/references
			internalChildren.name = (name.size()>0)?name:ref;
			// identify the cardinality and the type of element
			internalChildren.minCardinality = ((minimum.size()>0)?atoi(minimum.c_str()):1);
			internalChildren.maxCardinality = ((maximum.size()>0)?isUnbounded(maximum):1);
			internalChildren.type = ( (type.size()>0) ? type : "" );
			// if type is not standard
			if (!isStandardType(type)) {
				// if type is not null
				if (type.size()>0) {
					//generate the attribute list of the current element
					internalChildren.attribute = (isDCSimpleType(type)) ? DCAttr : generateAttributes(removePrefix(internalChildren.type, ":"), tmpEl);
					// and correct his type if required
					internalChildren.type = ( (isDCSimpleType(type)) ? DCType() : internalChildren.type );
				} else {
					// generate the attribute list of the current element and correct his type if required
					internalChildren.attribute = (isDCSimpleType(internalChildren.name))? DCAttr : generateAttributes("", tmpEl);
					internalChildren.type = ( (isDCSimpleType(internalChildren.name)) ? DCType() : "") ;
				}
			}
			// if element type is ebucore and the type not present in the stack, then
			if (isEBUCoreType(internalChildren.type) && !groupExist(internalChildren.type)) {
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateChildren(removePrefix(internalChildren.type,":"), el, level+1);					
				ebucoreStack.pop_back(); // pop the type
			}
			// push the children list
			children.push_back(internalChildren);
		// if the current tag king is group, then
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group") {
			// generate the children of this group and then add it to current list of children
			generateGroupChildren(children, xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))), el);	
		}
		tmpEl = tmpEl->getNextElementSibling(); // next
	}

	return children;

}

// include external elements (from groups)
void ebucoreParser::generateGroupChildren(std::list<ElementStruct> children, std::string father, xercesc::DOMElement * el) {
	// store the current position of xercesc pointer	
	xercesc::DOMElement * tmpEl = el;
	// looking for the group who contains the other elements with the father name
	while (tmpEl != 0  && !((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group" && xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) == removePrefix(father,":") )) {
		tmpEl = tmpEl->getNextElementSibling();
	}
	// enter inside the group
	tmpEl=tmpEl->getFirstElementChild();
	// looking for the sequence
	while (tmpEl != 0 && (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence") {
		tmpEl = tmpEl->getNextElementSibling();
	}
	// if pointer not null, grab the first child
	if (tmpEl != 0) {
		tmpEl=tmpEl->getFirstElementChild();
	}
	// while xerces pointer not null,
	while (tmpEl != 0) {
		// if it's an element
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element") {
			// prepare the element informations
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			std::string type (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("type"))));
			std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
			std::string minimum (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
			std::string maximum (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
			// instantiate the list of children
			ebucoreParser::ElementStruct internalChildren;
			// identify the element name
			internalChildren.name = (name.size()>0)?name:ref;
			// identify the cardinality and type
			internalChildren.minCardinality = ((minimum.size()>0)?atoi(minimum.c_str()):1);
			internalChildren.maxCardinality = ((maximum.size()>0)?isUnbounded(maximum):1);
			internalChildren.type = ( (type.size()>0) ? type : "" );
			// if it is not a standard type
			if (!isStandardType(type)) {
				// and if type is not null
				if (type.size()>0) {
					// generate attributes
					internalChildren.attribute = (isDCSimpleType(type)) ? DCAttr : generateAttributes(removePrefix(internalChildren.type, ":"), tmpEl);
					// and correct the type is required
					internalChildren.type = ( (isDCSimpleType(type)) ? DCType() : internalChildren.type );
				} else {
					// generate attributes and correct the type is required
					internalChildren.attribute = (isDCSimpleType(internalChildren.name))? DCAttr : generateAttributes("", tmpEl);
					internalChildren.type = ( (isDCSimpleType(internalChildren.name)) ? DCType() : "") ;		
				}
			}
			
			// if it is an ebucore element and the type of this element is not present in the element type stack
			if (isEBUCoreType(internalChildren.type) && !groupExist(internalChildren.type)) {
				// push the type
				ebucoreStack.push_back(internalChildren.type);
				// generate children
				internalChildren.children = generateChildren(removePrefix(internalChildren.type,":"), el, 1);	
				// pop the type				
				ebucoreStack.pop_back();
			}
			//push the internal child
			children.push_back(internalChildren);
		}
		// next
		tmpEl = tmpEl->getNextElementSibling();
	}
}

// pre-generate and pack the unique dublin core attribute
ebucoreParser::AttributeStruct ebucoreParser::DCAttribute(void) {
	AttributeStruct attr;
	attr.name = "xml:lang";
	attr.type = "string";
	attr.hasDefaultValue = false;
	attr.hasEnumeration = false;
	return attr;
}

std::string ebucoreParser::DCType(void) {
	return "DublinCore";
}

/*std::vector<std::string> ebucoreParser::listEnumeration(xercesc::DOMElement * el) {
	std::vector<std::string> listEnum;
	while (el != 0) {
		std::cout<<"Enum :: "<<(std::string)xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("value")))<<std::endl; 
		listEnum.push_back((std::string)xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("value"))));
		el = el->getNextElementSibling();
	}
	return listEnum;
}*/

// construct an ebucore skeleton
ebucoreParser::ElementStruct ebucoreParser::constructSchema(xercesc::DOMElement * el) {
	// prepare the element information
	std::string str (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("name"))));
	std::string type (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))));
	std::string ref (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("ref"))));
	std::string minimum (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
	std::string maximum (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
	// instantiate a new root
	ebucoreParser::ElementStruct root;
	// grab the name, the cardinalities, the type, attributes and children
	root.name = (str.size()>0)?str:ref;
	root.minCardinality = ((minimum.size()>0)?atoi(minimum.c_str()):1);
	root.maxCardinality = ((maximum.size()>0)?isUnbounded(maximum):1);
	root.type = ((type.size()>0)?type:"undefined");
	// store the current element type inside the stack to avoid infinite loops
	ebucoreStack.push_back(root.type);
	// generate the attributes of the current element
	root.attribute = ebucoreParser::generateAttributes(removePrefix (type, ":"), el);
	// generate the children of the current element
	root.children = generateChildren(removePrefix (type, ":"), el,1);	
	// pop the current element type outside of the stack
	ebucoreStack.pop_back();

	return root;
}

// test if it is dublincore type
bool ebucoreParser::isDCSimpleType(std::string str) {
	std::string target("dc:"); // dublincore prefix
	return (std::string::npos != str.find(target)) ? true :false ;
}

// test if it is standard type
bool ebucoreParser::isStandardType(std::string str) {
	// array of standard types
	const std::string elementType[] = {"string","nonNegativeInteger","boolean","long","integer","date","time","duration","hexBinary","anyURI","ID","IDREFS"};
	for (int i=0;i<12;i++) {
		if (elementType[i] == str) {return true;}
	}
  return false;
}

// test if it is ebucore type
bool ebucoreParser::isEBUCoreType(std::string str) {
	std::string target("ebucore:");
	return (std::string::npos != str.find(target)) ? true :false ;
}

// test if element is unbounded or not
int ebucoreParser::isUnbounded(std::string maximum) {
	return ((maximum == "unbounded")?-1:std::atoi(maximum.c_str()));
}

// test if a filename has a particular extension
bool ebucoreParser::isExtension(std::string str, std::string extension) {
	// find last "." position in a string
	int lastindex = str.find_last_of(".");
	// copy what comes after last dot
	std::string str2 = str.substr (lastindex+1,str.size()-(lastindex+1));
	//loop through each character and make it lower-case. stop when you hit '\0'.
	for(int i = 0; str2[i] != '\0'; i++){
		str2[i] = tolower(str2[i]);
	}
	// compare the file extension to the extension pattern
  return (extension.compare(str2) == 0) ? true : false;
}

// test if an ebucore type is present in the ebucoreStack
bool ebucoreParser::groupExist(std::string str) {
	for (std::vector<std::string>::iterator it = ebucoreStack.begin() ; it != ebucoreStack.end(); ++it) {
		if (*it == str) { return true; }
	}
  return false;
}

std::string ebucoreParser::removePrefix(std::string str, std::string prefix) {
	// find last "." position in a string
	int lastindex = str.find_last_of(prefix);
	// copy what comes after last dot
	std::string str2 = str.substr (lastindex+1,str.size()-(lastindex+1));
	// compare the file extension to the extension pattern
  return str2;
}

void ebucoreParser::generateSkeletonElement(ebucoreParser::ElementStruct skeleton) {
  //ebucoreStack.push_back(skeleton.name);
	//std::cout<<"generating an element..."<<std::endl;
	//std::cout<<"name "<<skeleton.name<<std::endl;
	for (std::list<ebucoreParser::ElementStruct>::iterator it=skeleton.children.begin() ; it != skeleton.children.end(); ++it) {
		ebucoreParser::ElementStruct child = *it;
		//std::cout<<"inside the loop"<<std::endl;
		//std::cout<<"child name : "<<child.name<<" and it has a minimal cardinality of : "<<child.minCardinality<<std::endl;
		if (child.minCardinality>0) {
			//std::cout<<"go deeper..."<<std::endl;
			generateSkeletonElement(child);
		}
	}	
	//ebucoreStack.pop_back();
}

void ebucoreParser::generateSkeleton(void) {
	out.open ("example.xml");
	generateSkeletonElement(ebucoremodel.at(0));
	out.close();
}
