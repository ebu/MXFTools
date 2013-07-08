/*!
 * \project_name EBU Player
 * \file EBUCoreFeatures.cpp
 * \brief EBUCore wrapping features
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 28 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */


#include "EBUCoreFeatures.hpp"


EBUCoreFeatures::EBUCoreFeatures
(
	void
) 
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
		//std::cout<< "start parsing..."<<std::endl;
		extractSchema(dir+"/"+files[i]);
	}
	// clean the stack and reuse it for skeleton generation
	ebucoreStack.clear();
	generateSkeleton();

}

EBUCoreFeatures::~EBUCoreFeatures
(
	void
)
{
	ebucoreStack.clear();
	ebucoremodel.clear();	
}

void EBUCoreFeatures::UnwrapEBUCore
(
	std::string MXFFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// extract ebucore metadata
	try
	{
		dom_doc = &EBUSDK::EBUCore::ExtractEBUCoreMetadata
		(
			MXFFilename.c_str(), 
			&progress_cb
		);
		 dom_root = dom_doc->getDocumentElement();
	}
	catch (std::exception& e)
	{
		std::cout << "Standard exception: " << e.what() << std::endl;
	}
}

void EBUCoreFeatures::UnwrapEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreOutputFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// extract ebucore metadata
	EBUSDK::EBUCore::ExtractEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		XMLEBUCoreOutputFilename.c_str(), 
		&progress_cb
	);
}

void EBUCoreFeatures::eraseEBUCore
(
	std::string MXFFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// remove ebucore metadatas
	EBUSDK::EBUCore::RemoveEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		&progress_cb
	);
}

void EBUCoreFeatures::eraseEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreOutputFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// extract ebucore metadatas
	EBUSDK::EBUCore::ExtractEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		XMLEBUCoreOutputFilename.c_str(), 
		&progress_cb
	);
	// remove ebucore metadatas
	EBUSDK::EBUCore::RemoveEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		&progress_cb
	);
}

void EBUCoreFeatures::WrapEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreFilename,
	bool setHeader,
	bool isDark,
	bool isSidecar
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	EBUSDK::EBUCore::EmbedEBUCoreMetadata
	(
		XMLEBUCoreFilename.c_str(), 
		MXFFilename.c_str(), 
		&progress_cb, 
		(EBUSDK::EBUCore::MetadataKind)(isSidecar) ? 
			EBUSDK::EBUCore::SIDECAR 
		: ((isDark) ? 
			EBUSDK::EBUCore::DARK : 
			EBUSDK::EBUCore::KLV_ENCODED
		), 
		false, 
		setHeader
	);
}

void EBUCoreFeatures::WrapEBUCore
(
	std::string MXFFilename,
	bool setHeader,
	bool isDark,
	bool isSidecar,
	std::string XMLEBUCoreFilename 
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	EBUSDK::EBUCore::EmbedEBUCoreMetadata
	(
		*dom_doc,
		XMLEBUCoreFilename.c_str(), 
		MXFFilename.c_str(), 
		&progress_cb, 
		(EBUSDK::EBUCore::MetadataKind)(isSidecar) ? 
			EBUSDK::EBUCore::SIDECAR 
		: ((isDark) ? 
			EBUSDK::EBUCore::DARK : 
			EBUSDK::EBUCore::KLV_ENCODED
		), 
		false, 
		setHeader
	);
}


void EBUCoreFeatures::reportEBUCoreSTMetadata
(
	std::string MXFFilename,
	std::string XMLFilename
)
{
	EBUSDK::Analyzer::AnalyzerConfig cfg;
	cfg.AnalysisType = EBUSDK::Analyzer::AnalyzerConfig::METADATA;
	cfg.MetadataAnalysisType = EBUSDK::Analyzer::AnalyzerConfig::LOGICAL;
	cfg.DeepIndexTableAnalysis = false;
	
	EBUSDK::Analyzer::AnalyzeMXFFile 	
	(
		MXFFilename.c_str(), 
		XMLFilename.c_str(),
		cfg
	);
}

void EBUCoreFeatures::reportEBUCoreSTMux
(
	std::string MXFFilename,
	std::string XMLFilename
)
{
	EBUSDK::Analyzer::AnalyzerConfig cfg;
	cfg.AnalysisType = EBUSDK::Analyzer::AnalyzerConfig::MXF_MUX;
	cfg.MetadataAnalysisType = EBUSDK::Analyzer::AnalyzerConfig::LOGICAL;
	cfg.DeepIndexTableAnalysis = false;
	
	
	EBUSDK::Analyzer::AnalyzeMXFFile 	
	(
		MXFFilename.c_str(), 
		XMLFilename.c_str(),
		cfg
	);
}



void EBUCoreFeatures::reportEBUCoreSTDeep
(
	std::string MXFFilename,
	std::string XMLFilename
)
{
	EBUSDK::Analyzer::AnalyzerConfig cfg;
	cfg.AnalysisType = EBUSDK::Analyzer::AnalyzerConfig::METADATA;
	cfg.MetadataAnalysisType = EBUSDK::Analyzer::AnalyzerConfig::LOGICAL;
	cfg.DeepIndexTableAnalysis = true;
	
	
	EBUSDK::Analyzer::AnalyzeMXFFile 	
	(
		MXFFilename.c_str(), 
		XMLFilename.c_str(),
		cfg
	);
}

xercesc::DOMDocument * EBUCoreFeatures::getEBUCore
(
	void
)
{
	return dom_doc;
}

bool EBUCoreFeatures::hasEBUCore
(
	void
)
{
	return (dom_doc != 0);
}

void EBUCoreFeatures::setEBUCore
(
	xercesc::DOMDocument * new_dom_doc
)
{
	dom_doc = new_dom_doc;
}


void EBUCoreFeatures::progress_cb
(
	float progress, 
	EBUSDK::EBUCore::ProgressCallbackLevel level, 
	const char *function, 
	const char *msg_format, 
	...
) 
{
	// append a newline to the msg_format string
	int len = strlen(msg_format);
	char* newline_msg_format =(char*)malloc((len+2) * sizeof(char));
	strncpy(newline_msg_format, msg_format, len); newline_msg_format[len] = 0;
	strncat(newline_msg_format, "\n", 1);

	va_list p_arg;
    va_start(p_arg, msg_format);
	switch (level) {
	case EBUSDK::EBUCore::INFO:
		bmx::log_info(newline_msg_format, p_arg); return;
	case EBUSDK::EBUCore::DEBUG:
	case EBUSDK::EBUCore::TRACE:
		bmx::log_debug(newline_msg_format, p_arg); return;
	case EBUSDK::EBUCore::ERROR:
		bmx::log_error(newline_msg_format, p_arg); return;
	case EBUSDK::EBUCore::WARN:
		bmx::log_warn(newline_msg_format, p_arg); return;
	default :
		bmx::log_warn(newline_msg_format, p_arg);
	};
	va_end(p_arg);

	free(newline_msg_format);
}

// get the path to the schemas
int EBUCoreFeatures::getSchemas
(
	std::string dir, 
	std::vector<std::string> &files
)
{
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
		if (dirp->d_type == DT_REG and genericFeatures::isExtension(dirp->d_name, "xsd"))
			// save the schema filename inside the vector
			files.push_back(std::string(dirp->d_name));
	}
	closedir(dp); // close folder
	return 0;
}

// extract a schema
void EBUCoreFeatures::extractSchema
(
	std::string pathtofile
) 
{
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
EBUCoreFeatures::AttributeStruct EBUCoreFeatures::packAttribute
(
	xercesc::DOMElement * el
) 
{
	// prepare the attribute struct
	EBUCoreFeatures::AttributeStruct attr;
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
std::list<EBUCoreFeatures::AttributeStruct> EBUCoreFeatures::generateAttributes
(
	std::string father, 
	xercesc::DOMElement * el
) 
{
	// groupName to store the attributeGroups
	std::vector<std::string> groupName;
	// attributes list
	std::list<EBUCoreFeatures::AttributeStruct> attributes;
	// when father type hasn't a name, that means the type is implicit
	if (father =="") {
		// we must save the current position
		xercesc::DOMElement * tmp = el->getFirstElementChild();
		while (tmp != 0  and (std::string)(xercesc::XMLString::transcode(tmp->getTagName())) != "complexType") {
			tmp = tmp->getNextElementSibling();// next
		}
		// and looking for the implicit father complexType (without name)
		if (tmp != 0 and (std::string)(xercesc::XMLString::transcode(tmp->getTagName())) == "complexType" ) {
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
							groupName.push_back(genericFeatures::removePrefix ((std::string)(xercesc::XMLString::transcode(localTmp->getAttribute (xercesc::XMLString::transcode("ref")))),":" ));
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
						groupName.push_back(genericFeatures::removePrefix ((std::string)(xercesc::XMLString::transcode(localTmp->getAttribute (xercesc::XMLString::transcode("base")))),":" ));
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
							groupName.push_back(genericFeatures::removePrefix ((std::string)(xercesc::XMLString::transcode(localTmp->getAttribute (xercesc::XMLString::transcode("ref")))),":"));
						}
						// next element...
						localTmp = localTmp->getNextElementSibling();
					}				
				// if attribute is an attributeGroup
				} else if ((std::string)(xercesc::XMLString::transcode(tmp->getTagName())) == "attributeGroup"){
					// save the attributeGroup name/reference
					groupName.push_back(genericFeatures::removePrefix ((std::string)(xercesc::XMLString::transcode(tmp->getAttribute (xercesc::XMLString::transcode("ref")))), ":" ));
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
	if (father!="") {
		groupName.push_back(father);
	}

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
		while (((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" and xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != groupName.at(i) ) or ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "attributeGroup" and xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != groupName.at(i))) {
			tmpEl = tmpEl->getNextElementSibling();
		}
		//grab the first child
		tmpEl=tmpEl->getFirstElementChild();
		
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "complexContent") 
		{
			tmpEl=tmpEl->getFirstElementChild();
			if (!isDCSimpleType(xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("base"))))) 
			{
				groupName.push_back(genericFeatures::removePrefix ((std::string)(xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("base")))), ":" ));
			} else {
				attributes.push_back(DCAttribute());
			}
			tmpEl=tmpEl->getFirstElementChild();
		}
		else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "simpleContent") 
		{
			tmpEl=tmpEl->getFirstElementChild();
			// must be completed !!!!
			tmpEl=tmpEl->getFirstElementChild();
		}

		// while xerces pointer not null
		do {
			// and if the element is an attribute
			if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "attribute") {
				// wrap and push the attribute
				attributes.push_back(packAttribute(tmpEl));
			}
			else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "attributeGroup") 
			{
				groupName.push_back(genericFeatures::removePrefix ((std::string)(xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref")))), ":" ));
			}
			// next
			tmpEl = tmpEl->getNextElementSibling();
		} while (tmpEl != 0);
	}
	return attributes;

}

std::list<std::list<std::string>> EBUCoreFeatures::generateChoices
(
	std::string father,
	xercesc::DOMElement * el
) 
{
	//std::cout<<"I'm trying to generate choices for "<<father<<std::endl;
	std::list<std::list<std::string>> choices;
	// copy the current position of xercesc pointer
	if (father != "") 
	{
		xercesc::DOMElement * tmpEl = el;
		// while xercesc pointer not null, element different of complexType with the proper name
		while (tmpEl != 0  and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" or xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father )) 
		{
			tmpEl = tmpEl->getNextElementSibling();// next
		}
		if (tmpEl != 0) {
			// grab the first child of proper element
			tmpEl=tmpEl->getFirstElementChild();
			// looking for the choice/sequence of elements
			while (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice" and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence") 
			{
				tmpEl = tmpEl->getNextElementSibling(); // next
			}
			if (tmpEl != 0) 
			{
				// if not yet choice tag, then continu to look for choice of elements
				if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice") 
				{
					tmpEl=tmpEl->getFirstElementChild();
				}
				// looking for choice tag
				while (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice") 
				{
					tmpEl = tmpEl->getNextElementSibling(); // next
				}
				// if pointer not null, grab the first child of the current choice
				if (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "choice") 
				{
					tmpEl=tmpEl->getFirstElementChild();
				}
				// generate the choice list
				while (tmpEl != 0) 
				{
					std::list<std::string> tmp;
					// if it's element tag
					if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element")
					{
						tmp.push_back((std::string)(xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name")))));
					}
					// else, it's a sequence tag
					else
					{
						xercesc::DOMElement * local = tmpEl->getFirstElementChild();
						// looking for element tags inside sequence tag
						while (local != 0) 
						{
							if ((std::string)(xercesc::XMLString::transcode(local->getTagName())) == "element")
							{
								tmp.push_back((std::string)(xercesc::XMLString::transcode(local->getAttribute (xercesc::XMLString::transcode("name")))));
							}
							local = local->getNextElementSibling(); // next
						}
					}
					choices.push_back(tmp); // push the choice list
					tmpEl = tmpEl->getNextElementSibling(); // next
				}
			}
		}
	}
	return choices;
}

// generate the children of an elements
std::list<EBUCoreFeatures::ElementStruct> EBUCoreFeatures::generateLocalChildren
(
	std::string father, 
	xercesc::DOMElement * el, 
	xercesc::DOMElement * cursor
) 
{
	//std::cout<<(std::string)(xercesc::XMLString::transcode(cursor->getTagName()))<<std::endl;
	// instantiate the children list
	std::list<EBUCoreFeatures::ElementStruct> children;
	// copy the current position of xercesc pointer
	xercesc::DOMElement * tmpEl = cursor;
	
	while (tmpEl != 0  and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType")) 
	{
		tmpEl = tmpEl->getNextElementSibling();
	}
	
	if (tmpEl != 0) 
	{
		tmpEl=tmpEl->getFirstElementChild();
	}
	
	while (tmpEl != 0  and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence")) 
	{
		tmpEl = tmpEl->getNextElementSibling();
	}
	
	if (tmpEl != 0) 
	{
		tmpEl=tmpEl->getFirstElementChild();
	}
	
	while (tmpEl != 0) 
	{
		// if current tag kind is element
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element") {
			// prepare the element information
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			std::string type (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("type"))));
			std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
			std::string min (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
			std::string max (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
			// instantiate the children struct
			EBUCoreFeatures::ElementStruct internalChildren;
			// grab the right name/references
			internalChildren.name = (name.size()>0)?name:ref;
			// identify the cardinality and the type of element
			internalChildren.minCardinality = ((min.size()>0)?atoi(min.c_str()):1);
			internalChildren.maxCardinality = ((max.size()>0)?isUnbounded(max):1);
			internalChildren.type = ( (type.size()>0) ? type : "" );
			internalChildren.choices = generateChoices(((isEBUCoreType(internalChildren.type))?genericFeatures::removePrefix(internalChildren.type, ":"):""), el);
			// if type is not standard
			if (!isStandardType(type)) {
				// if type is not null
				if (type.size()>0) {
					//generate the attribute list of the current element
					internalChildren.attribute = (isDCSimpleType(type)) ? DCAttr : generateAttributes(genericFeatures::removePrefix(internalChildren.type, ":"), tmpEl);
					// and correct his type if required
					internalChildren.type = ( (isDCSimpleType(type)) ? DCType() : internalChildren.type );
				} else {
					// generate the attribute list of the current element and correct his type if required
					internalChildren.attribute = (isDCSimpleType(internalChildren.name))? DCAttr : generateAttributes("", tmpEl);
					internalChildren.type = ( (isDCSimpleType(internalChildren.name)) ? DCType() : "") ;
				}
			}
			// if element type is ebucore and the type not present in the stack, then
			if (isEBUCoreType(internalChildren.type) and !groupExist(internalChildren.type)) {
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateChildren(genericFeatures::removePrefix(internalChildren.type,":"), el);
				ebucoreStack.pop_back(); // pop the type
			}
			else if (internalChildren.type == "" and tmpEl->getFirstElementChild() != 0)
			{
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateLocalChildren("", el, tmpEl->getFirstElementChild());
				ebucoreStack.pop_back(); // pop the type
			}
			// push the children list
			children.push_back(internalChildren);
		// if the current tag king is group, then
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group") {
			// generate the children of this group and then add it to current list of children
			children = generateGroupChildren
			(
				children, 
				xercesc::XMLString::transcode
				(
					tmpEl->getAttribute 
					(
						xercesc::XMLString::transcode("ref")
					)
				), 
				el
			);
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "choice")
		{
			children =  generateChoiceChildren(children, tmpEl, el);
		}
		tmpEl = tmpEl->getNextElementSibling(); // next
	}
	// enter 
	return children;
}

// generate the children of an elements
std::list<EBUCoreFeatures::ElementStruct> EBUCoreFeatures::generateChildren
(
	std::string father, 
	xercesc::DOMElement * el
) 
{
	// instantiate the children list
	// extension should stay empty
	std::string localExtension("");
	std::list<EBUCoreFeatures::ElementStruct> children;
	// copy the current position of xercesc pointer
	xercesc::DOMElement * tmpEl = el;
	// while xercesc pointer not null, element different of complexType with the proper name
	while (tmpEl != 0  and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" or xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father )) {
		tmpEl = tmpEl->getNextElementSibling();// next
	}
	// grab the first child of proper element
	tmpEl=tmpEl->getFirstElementChild();
	// looking for the sequence of elements
	while (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice" and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence" and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexContent") 
	{
		tmpEl = tmpEl->getNextElementSibling(); // next
	}
	if (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "complexContent" and father != "")
	{
		tmpEl=tmpEl->getFirstElementChild();
		localExtension =(std::string)(xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("base"))));
		tmpEl=tmpEl->getFirstElementChild();
	}
	// if pointer not null, grab the first child of the current element
	if (tmpEl != 0)
	{
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
			std::string min (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
			std::string max (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
			// instantiate the children struct
			EBUCoreFeatures::ElementStruct internalChildren;
			// grab the right name/references
			internalChildren.name = (name.size()>0)?name:ref;
			// identify the cardinality and the type of element
			internalChildren.minCardinality = ((min.size()>0)?atoi(min.c_str()):1);
			internalChildren.maxCardinality = ((max.size()>0)?isUnbounded(max):1);
			internalChildren.type = ( (type.size()>0) ? type : "" );
			internalChildren.choices = generateChoices(((isEBUCoreType(internalChildren.type))?genericFeatures::removePrefix(internalChildren.type, ":"):""), el);
			
			// if type is not standard
			if (!isStandardType(type)) {
				// if type is not null
				if (type.size()>0) {
					//generate the attribute list of the current element
					internalChildren.attribute = (isDCSimpleType(type)) ? DCAttr : generateAttributes(genericFeatures::removePrefix(internalChildren.type, ":"), tmpEl);
					// and correct his type if required
					internalChildren.type = ( (isDCSimpleType(type)) ? DCType() : internalChildren.type );
				} else {
					// generate the attribute list of the current element and correct his type if required
					internalChildren.attribute = (isDCSimpleType(internalChildren.name))? DCAttr : generateAttributes("", tmpEl);
					internalChildren.type = ( (isDCSimpleType(internalChildren.name)) ? DCType() : "") ;
				}
			}
			// if element type is ebucore and the type not present in the stack, then
			if (isEBUCoreType(internalChildren.type) and !groupExist(internalChildren.type)) {
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateChildren(genericFeatures::removePrefix(internalChildren.type,":"), el);
				ebucoreStack.pop_back(); // pop the type
			}
			else if (internalChildren.type == "" and tmpEl->getFirstElementChild() != 0)
			{
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateLocalChildren("", el, tmpEl->getFirstElementChild());
				ebucoreStack.pop_back(); // pop the type
			}
			// push the children list
			children.push_back(internalChildren);
		// if the current tag king is group, then
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group") {
			// generate the children of this group and then add it to current list of children
			children = generateGroupChildren
			(
				children, 
				xercesc::XMLString::transcode
				(
					tmpEl->getAttribute 
					(
						xercesc::XMLString::transcode("ref")
					)
				), 
				el
			);
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "choice")
		{
			children =  generateChoiceChildren(children, tmpEl, el);
		}
		tmpEl = tmpEl->getNextElementSibling(); // next
	}
	
	if (localExtension != "" and localExtension != "dc:elementType") 
	{
		children = generateExtensionChildren
		(
			children, 
			genericFeatures::removePrefix(localExtension,":"), 
			el
		);
	}
/*	std::cout<<"fathertype : "<<father<<std::endl;
		for (std::list<EBUCoreFeatures::ElementStruct>::iterator it=children.begin() ; it != children.end(); ++it) 
		{
			EBUCoreFeatures::ElementStruct child = *it;
			std::cout<<"          : "<<child.name<<std::endl;
		}
	*/
	return children;
}

std::list<EBUCoreFeatures::ElementStruct> EBUCoreFeatures::generateExtensionChildren
(
	std::list<ElementStruct> brothers, 
	std::string father, 
	xercesc::DOMElement * el
) 
{
	
	std::string localExtension("");
	std::list<EBUCoreFeatures::ElementStruct> children = brothers;
	// store the current position of xercesc pointer	
	xercesc::DOMElement * tmpEl = el;
	// while xercesc pointer not null, element different of complexType with the proper name
	while (tmpEl != 0  and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" or xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father )) {
		tmpEl = tmpEl->getNextElementSibling();// next
	}
	// grab the first child of proper element
	tmpEl=tmpEl->getFirstElementChild();
	// looking for the sequence of elements
	while (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice" and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence" and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexContent") 
	{
		tmpEl = tmpEl->getNextElementSibling(); // next
	}
	if (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "complexContent" and father != "")
	{
		tmpEl=tmpEl->getFirstElementChild();
		localExtension =(std::string)(xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("base"))));
		tmpEl=tmpEl->getFirstElementChild();
	}
	// if pointer not null, grab the first child of the current element
	if (tmpEl != 0)
	{
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
			std::string min (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
			std::string max (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
			// instantiate the children struct
			EBUCoreFeatures::ElementStruct internalChildren;
			// grab the right name/references
			internalChildren.name = (name.size()>0)?name:ref;
			// identify the cardinality and the type of element
			internalChildren.minCardinality = ((min.size()>0)?atoi(min.c_str()):1);
			internalChildren.maxCardinality = ((max.size()>0)?isUnbounded(max):1);
			internalChildren.type = ( (type.size()>0) ? type : "" );
			internalChildren.choices = generateChoices(((isEBUCoreType(internalChildren.type))?genericFeatures::removePrefix(internalChildren.type, ":"):""), el);
			// if type is not standard
			if (!isStandardType(type)) {
				// if type is not null
				if (type.size()>0) {
					//generate the attribute list of the current element
					internalChildren.attribute = (isDCSimpleType(type)) ? DCAttr : generateAttributes(genericFeatures::removePrefix(internalChildren.type, ":"), tmpEl);
					// and correct his type if required
					internalChildren.type = ( (isDCSimpleType(type)) ? DCType() : internalChildren.type );
				} else {
					// generate the attribute list of the current element and correct his type if required
					internalChildren.attribute = (isDCSimpleType(internalChildren.name))? DCAttr : generateAttributes("", tmpEl);
					internalChildren.type = ( (isDCSimpleType(internalChildren.name)) ? DCType() : "") ;
				}
			}
			// if element type is ebucore and the type not present in the stack, then
			if (isEBUCoreType(internalChildren.type) and !groupExist(internalChildren.type)) {
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateChildren(genericFeatures::removePrefix(internalChildren.type,":"), el);
				ebucoreStack.pop_back(); // pop the type
			}
			else if (internalChildren.type == "" and tmpEl->getFirstElementChild() != 0)
			{
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateLocalChildren("", el, tmpEl->getFirstElementChild());
				ebucoreStack.pop_back(); // pop the type
			}
			// push the children list
			children.push_back(internalChildren);
		// if the current tag king is group, then
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group") {
			// generate the children of this group and then add it to current list of children
			children = generateGroupChildren
			(
				children, 
				xercesc::XMLString::transcode
				(
					tmpEl->getAttribute 
					(
						xercesc::XMLString::transcode("ref")
					)
				), 
				el
			);
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "choice")
		{
			children =  generateChoiceChildren(children, tmpEl, el);
		}
		tmpEl = tmpEl->getNextElementSibling(); // next
	}
	
	if (localExtension != "" and localExtension != "dc:elementType") 
	{
		children = generateExtensionChildren
		(
			children, 
			genericFeatures::removePrefix(localExtension,":"), 
			el
		);
	}
	return children;
}


// include external elements (from groups)
std::list<EBUCoreFeatures::ElementStruct> EBUCoreFeatures::generateGroupChildren
(
	std::list<ElementStruct> brothers, 
	std::string father, 
	xercesc::DOMElement * el
) 
{
	std::list<EBUCoreFeatures::ElementStruct> children = brothers;
	// store the current position of xercesc pointer	
	xercesc::DOMElement * tmpEl = el;
	// looking for the group who contains the other elements with the father name
	while (tmpEl != 0  and not ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group" and xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) == genericFeatures::removePrefix(father,":") )) {
		tmpEl = tmpEl->getNextElementSibling();
	}
	// enter inside the group
	tmpEl=tmpEl->getFirstElementChild();
	// looking for the sequence
	while (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence") {
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
			std::string min (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
			std::string max (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
			// instantiate the list of children
			EBUCoreFeatures::ElementStruct internalChildren;
			// identify the element name
			internalChildren.name = (name.size()>0)?name:ref;
			internalChildren.hasChoice = false;
			// identify the cardinality and type
			internalChildren.minCardinality = ((min.size()>0)?atoi(min.c_str()):1);
			internalChildren.maxCardinality = ((max.size()>0)?isUnbounded(max):1);
			internalChildren.type = ( (type.size()>0) ? type : "" );
			internalChildren.choices = generateChoices(((isEBUCoreType(internalChildren.type))?genericFeatures::removePrefix(internalChildren.type, ":"):""), el);
			// if it is not a standard type
			if (!isStandardType(type)) {
				// and if type is not null
				if (type.size()>0) {
					// generate attributes
					internalChildren.attribute = (isDCSimpleType(type)) ? DCAttr : generateAttributes(genericFeatures::removePrefix(internalChildren.type, ":"), tmpEl);
					// and correct the type is required
					internalChildren.type = ( (isDCSimpleType(type)) ? DCType() : internalChildren.type );
				} else {
					// generate attributes and correct the type is required
					internalChildren.attribute = (isDCSimpleType(internalChildren.name))? DCAttr : generateAttributes("", tmpEl);
					internalChildren.type = ( (isDCSimpleType(internalChildren.name)) ? DCType() : "") ;		
				}
			}
			
			// if it is an ebucore element and the type of this element is not present in the element type stack
			if (isEBUCoreType(internalChildren.type) and !groupExist(internalChildren.type)) {
				// push the type
				ebucoreStack.push_back(internalChildren.type);
				// generate children
				internalChildren.children = generateChildren(genericFeatures::removePrefix(internalChildren.type,":"), el);	
				// pop the type				
				ebucoreStack.pop_back();
			}
			else if (internalChildren.type == "" and tmpEl->getFirstElementChild() != 0)
			{
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateLocalChildren("", el, tmpEl->getFirstElementChild());
				ebucoreStack.pop_back(); // pop the type
			}
			//push the internal child
			children.push_back(internalChildren);
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group") {
			// generate the children of this group and then add it to current list of children
			children = generateGroupChildren
			(
				children, 
				xercesc::XMLString::transcode
				(
					tmpEl->getAttribute 
					(
						xercesc::XMLString::transcode("ref")
					)
				), 
				el
			);
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "choice")
		{
			children =  generateChoiceChildren(children, tmpEl, el);
		}
		// next
		tmpEl = tmpEl->getNextElementSibling();
	}
	return children;
}

// include external elements (from groups)
std::list<EBUCoreFeatures::ElementStruct> EBUCoreFeatures::generateChoiceChildren
(
	std::list<ElementStruct> brothers,
	xercesc::DOMElement * el,
	xercesc::DOMElement * parent
) 
{
	std::list<EBUCoreFeatures::ElementStruct> children = brothers;
	// store the current position of xercesc pointer	
	xercesc::DOMElement * tmpEl = el;
	// looking for the group who contains the other elements with the father name
	while (tmpEl != 0  and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice") {
		tmpEl = tmpEl->getNextElementSibling();
	} // choice
	// enter inside the group
	tmpEl=tmpEl->getFirstElementChild(); // element or sequence
	bool sequence = false;
	xercesc::DOMElement * memSequence;
	// while xerces pointer not null,
	while (tmpEl != 0) {
		// if it's a sequence of element
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "sequence") 
		{
			sequence = true;
			memSequence = tmpEl;
			tmpEl=tmpEl->getFirstElementChild();
		} 
		
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element") 
		{
			// prepare the element informations
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			std::string type (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("type"))));
			std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
			std::string min (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
			std::string max (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
			// instantiate the list of children
			EBUCoreFeatures::ElementStruct internalChildren;
			// identify the element name
			internalChildren.name = (name.size()>0)?name:ref;
			internalChildren.hasChoice = false;
			// identify the cardinality and type
			internalChildren.minCardinality = ((min.size()>0)?atoi(min.c_str()):1);
			internalChildren.maxCardinality = ((max.size()>0)?isUnbounded(max):1);
			internalChildren.type = ( (type.size()>0) ? type : "" );
			internalChildren.choices = generateChoices(((isEBUCoreType(internalChildren.type))?genericFeatures::removePrefix(internalChildren.type, ":"):""), el);
			// if it is not a standard type
			if (!isStandardType(type)) {
				// and if type is not null
				if (type.size()>0) {
					// generate attributes
					internalChildren.attribute = (isDCSimpleType(type)) ? DCAttr : generateAttributes(genericFeatures::removePrefix(internalChildren.type, ":"), tmpEl);
					// and correct the type is required
					internalChildren.type = ( (isDCSimpleType(type)) ? DCType() : internalChildren.type );
				} else {
					// generate attributes and correct the type is required
					internalChildren.attribute = (isDCSimpleType(internalChildren.name))? DCAttr : generateAttributes("", tmpEl);
					internalChildren.type = ( (isDCSimpleType(internalChildren.name)) ? DCType() : "") ;		
				}
			}
			// if it is an ebucore element and the type of this element is not present in the element type stack
			if (isEBUCoreType(internalChildren.type) and !groupExist(internalChildren.type)) {
				// push the type
				ebucoreStack.push_back(internalChildren.type);
				// generate children
				internalChildren.children = generateChildren(genericFeatures::removePrefix(internalChildren.type,":"), parent);	
				// pop the type
				ebucoreStack.pop_back();
			}
			else if (internalChildren.type == "" and tmpEl->getFirstElementChild() != 0)
			{
				ebucoreStack.push_back(internalChildren.type); // push the type
				// generate the children
				internalChildren.children = generateLocalChildren("", parent, tmpEl->getFirstElementChild());
				ebucoreStack.pop_back(); // pop the type
			}
			//push the internal child
			children.push_back(internalChildren);
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "group") {
			// generate the children of this group and then add it to current list of children
			children = generateGroupChildren
			(
				children, 
				xercesc::XMLString::transcode
				(
					tmpEl->getAttribute 
					(
						xercesc::XMLString::transcode("ref")
					)
				), 
				el
			);
		} else if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "choice")
		{
			children =  generateChoiceChildren(children, tmpEl, el);
		}
		// next
		tmpEl = tmpEl->getNextElementSibling();
		if (sequence and tmpEl == 0) {
			sequence = false;
			tmpEl = memSequence;
			tmpEl = tmpEl->getNextElementSibling();
		}
	}
	return children;
}



// pre-generate and pack the unique dublin core attribute
EBUCoreFeatures::AttributeStruct EBUCoreFeatures::DCAttribute
(
	void
) 
{
	AttributeStruct attr;
	attr.name = "xml:lang";
	attr.type = "string";
	attr.hasDefaultValue = false;
	attr.hasEnumeration = false;
	return attr;
}

std::string EBUCoreFeatures::DCType
(
	void
) 
{
	return "DublinCore";
}

/*std::vector<std::string> EBUCoreFeatures::listEnumeration(xercesc::DOMElement * el) {
	std::vector<std::string> listEnum;
	while (el != 0) {
		std::cout<<"Enum :: "<<(std::string)xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("value")))<<std::endl; 
		listEnum.push_back((std::string)xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("value"))));
		el = el->getNextElementSibling();
	}
	return listEnum;
}*/

EBUCoreFeatures::ElementStruct EBUCoreFeatures::getReference
(
	std::string path
) 
{
	std::vector<std::pair<std::string, EBUCoreFeatures::ElementStruct>> ebucoreType;
	EBUCoreFeatures::ElementStruct skeleton = ebucoremodel.at(0);
	path.erase(0,1); // delete first #
	
	while(path.size() != 0 and path != skeleton.name) 
	{
//		std::cout 
//		<< "current path : " <<path<<std::endl
//		<< "type         : " <<skeleton.type<<std::endl
//		<< "mem qty      : " <<ebucoreType.size()<< std::endl;
		
		unsigned int pos = path.find_first_of("#");
		if (path.substr(0,pos) == skeleton.name) 
		{
			if (hasTypeReference(ebucoreType, skeleton.type)) 
			{
				skeleton = loopTypeReference(ebucoreType, skeleton.type);
			} 
			else 
			{
				ebucoreType.push_back(std::make_pair(skeleton.type, skeleton));
			}
			path.erase(0,pos);
			if (path.size() > 0) {
				path.erase(0,1);
				pos = path.find_first_of("#");
				for (std::list<EBUCoreFeatures::ElementStruct>::iterator it=skeleton.children.begin() ; it != skeleton.children.end(); ++it) 
				{
					EBUCoreFeatures::ElementStruct child = *it;
					if (child.name == path.substr(0,pos)) {
						skeleton = child;
						break;
					}
				}
			}
		}
	}
	if (hasTypeReference(ebucoreType, skeleton.type)) 
	{
		skeleton = loopTypeReference(ebucoreType, skeleton.type);
	} 
	
//	std::cout<<"element   : "<<skeleton.name<<std::endl;
//	std::cout<<"type      : "<<skeleton.type<<std::endl;
//	std::cout<<"attributs : "<<skeleton.attribute.size()<<" attribute(s)."<<std::endl;
//	for (std::list<EBUCoreFeatures::AttributeStruct>::iterator it=skeleton.attribute.begin() ; it != skeleton.attribute.end(); ++it) 
//	{
//		EBUCoreFeatures::AttributeStruct attr = *it;
//		std::cout<<"          : "<<attr.name<<std::endl;
//	}
//	std::cout<<"enfants   : "<<skeleton.children.size()<<" child(ren)."<<std::endl;
//	for (std::list<EBUCoreFeatures::ElementStruct>::iterator it=skeleton.children.begin() ; it != skeleton.children.end(); ++it) 
//	{
//		EBUCoreFeatures::ElementStruct child = *it;
//		std::cout<<"          : "<<child.name<<std::endl;
//	}
	return skeleton;
	
	EBUCoreFeatures::ElementStruct empty;
	return empty;
}

bool EBUCoreFeatures::hasTypeReference
(
	std::vector<std::pair<std::string, EBUCoreFeatures::ElementStruct>> typelist,
	std::string newtype
) 
{
	for (unsigned int i=0;i<typelist.size();i++) 
	{
		if (typelist.at(i).first == newtype) 
		{
			return true;
		}
	}
	return false;
}


EBUCoreFeatures::ElementStruct EBUCoreFeatures::loopTypeReference
(
	std::vector<std::pair<std::string, EBUCoreFeatures::ElementStruct>> typelist,
	std::string newtype
) 
{
	EBUCoreFeatures::ElementStruct tmp;
	for (unsigned int i=0;i<typelist.size();i++) 
	{
		if (typelist.at(i).first == newtype) 
		{
			return typelist.at(i).second;
		}
	} 
	return tmp;
}

// construct an ebucore skeleton
EBUCoreFeatures::ElementStruct EBUCoreFeatures::constructSchema
(
	xercesc::DOMElement * el
) 
{
	// prepare the element information
	std::string str (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("name"))));
	std::string type (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))));
	std::string ref (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("ref"))));
	std::string min (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
	std::string max (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));
	// instantiate a new root
	EBUCoreFeatures::ElementStruct root;
	// grab the name, the cardinalities, the type, attributes and children
	root.name = (str.size()>0)?str:ref;
	root.hasChoice = false;
	root.minCardinality = ((min.size()>0)?atoi(min.c_str()):1);
	root.maxCardinality = ((max.size()>0)?isUnbounded(max):1);
	root.type = ((type.size()>0)?type:"undefined");
	// store the current element type inside the stack to avoid infinite loops
	ebucoreStack.push_back(root.type);
	// generate the attributes of the current element
	root.attribute = EBUCoreFeatures::generateAttributes(genericFeatures::removePrefix (type, ":"), el);
	// generate the children of the current element
	root.children = generateChildren(genericFeatures::removePrefix (type, ":"), el);	
	// pop the current element type outside of the stack
	ebucoreStack.pop_back();

	return root;
}

// test if it is dublincore type
bool EBUCoreFeatures::isDCSimpleType
(
	std::string str
) 
{
	std::string target("dc:"); // dublincore prefix
	return (std::string::npos != str.find(target)) ? true :false ;
}

// test if it is standard type
bool EBUCoreFeatures::isStandardType
(
	std::string str
) 
{
	// array of standard types
	const std::string elementType[] =
	{
		"string","nonNegativeInteger","boolean","long","integer","float",
		"date","time","duration","hexBinary","anyURI","ID","IDREFS"
	};
	
	for (int i=0;i<12;i++) {
		if (elementType[i] == str) {return true;}
	}
  return false;
}

// test if it is ebucore type
bool EBUCoreFeatures::isEBUCoreType
(
	std::string str
) 
{
	std::string target("ebucore:");
	return (std::string::npos != str.find(target)) ? true :false ;
}

// test if element is unbounded or not
int EBUCoreFeatures::isUnbounded
(
	std::string max
) 
{
	return ((max == "unbounded")?-1:std::atoi(max.c_str()));
}

// test if an ebucore type is present in the ebucoreStack
bool EBUCoreFeatures::groupExist
(
	std::string str
) 
{
	for (std::vector<std::string>::iterator it = ebucoreStack.begin() ; it != ebucoreStack.end(); ++it) {
		if (*it == str) { return true; }
	}
  return false;
}

/*
void EBUCoreFeatures::generateSkeletonElement
(
	EBUCoreFeatures::ElementStruct skeleton
) 
{
	out<<"name "<<skeleton.name<<std::endl;
	for (std::list<EBUCoreFeatures::ElementStruct>::iterator it=skeleton.children.begin() ; it != skeleton.children.end(); ++it) {
		EBUCoreFeatures::ElementStruct child = *it;
		if (child.children.size()>0) {
			generateSkeletonElement(child);
		} else {
			out<<"childname "<<child.name<<std::endl;
		}
	}
}


void EBUCoreFeatures::generateSkeleton
(
	void
) 
{
	out.open ("/home/marco/Dev/EBU-MP/example.txt");
	generateSkeletonElement(ebucoremodel.at(0));
	out.close();
}

*/
