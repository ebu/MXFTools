/*!
 * \project_name EBU Player
 * \file metadataEngine.cpp
 * \brief metadata engine
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 11 february 2013
 * \copyright This software is published under MPL v2.0
 *
 */


#include "metadataEngine.hpp"


metadataEngine::metadataEngine
(
	Gtk::Label * statusRef,
	Gtk::Label * sourceRef, 
	Gtk::Label * sourceLabelRef,
	Gtk::Label * sizeRef, 
	Gtk::Label * sizeValueRef,
	Gtk::Box * lineOneRef, 
	Gtk::Box * lineTwoRef,
	Gtk::ScrolledWindow * FirstScrolledWindowBoxRef,
	Gtk::Box * SecondScrolledWindowBoxRef,
	Gtk::Box * boxEntriesRef,
	Gtk::Box * boxStatusRef,
	Gtk::Expander * expanderRootRef,
	Gtk::Viewport * viewportTreeRef,
	Gtk::Switch * SwitchEBUCoreRef,
	Gtk::Paned * metadataPanedWindowRef,
	Gtk::Window * win
) 
{
	// init the viewport size
	viewportTreeMinimumWidth = 0;	
	// set colors
	black.set("black");
	white.set("white");
	whiteghost.set("whiteghost");
	red.set("red");
	blue.set("blue");
	// init the informational widgets
	status = statusRef;
	source = sourceRef;
	sourceLabel = sourceLabelRef;
	size = sizeRef; 
	sizeValue = sizeValueRef;
	lineOne = lineOneRef;
	lineTwo = lineTwoRef;
	FirstScrolledWindowBox = FirstScrolledWindowBoxRef;
	SecondScrolledWindowBox = SecondScrolledWindowBoxRef;
	boxEntries = boxEntriesRef; 
	boxStatus = boxStatusRef;
	expanderRoot = expanderRootRef;
	viewportTree = viewportTreeRef;
	EBUCoreSet = SwitchEBUCoreRef;
	panedWindow = metadataPanedWindowRef;
	window = win;
	window->set_size_request(350,-1);
		// Connect the button clicked signals
	EBUCoreSet->property_active().signal_changed().connect
	(
		sigc::mem_fun
		(
			*this,
			&metadataEngine::on_EBUCoreSet_changed
		)
	);
	// init the sdk wrapper
	SDKWrapper = new EBUCoreFeatures();
	
}

metadataEngine::~metadataEngine
(
	void
)
{	
	delete SDKWrapper;
}

void metadataEngine::on_EBUCoreSet_changed
(
	void
)
{
	constructEditableNode(elReferences.at(previousnodepos));
}

void metadataEngine::setRightSide
(
	std::string filename,
	bool createNew
)
{
	if (genericFeatures::getFileSize(filename) > 0 and !createNew) 
 	{
 		status->set_text("EBUCore metadata detected");
 	 	sourceLabel->set_text(genericFeatures::removeSuffix(filename, "/"));
 	 	sizeValue->set_text
 	 		(
 	 			genericFeatures::getSizeUnit
 	 			(
 	 				genericFeatures::getFileSize(filename)
 	 			)
 	 		);
 		FirstScrolledWindowBox->show();
	} else if (genericFeatures::getFileSize(filename) == 0 and createNew) {
 		status->set_text("Creating a new EBUCore instance");
 	 	sourceLabel->set_text(genericFeatures::removeSuffix(filename, "/"));
 	 	sizeValue->set_text
 	 		(
 	 			genericFeatures::getSizeUnit
 	 			(
 	 				genericFeatures::getFileSize(filename)
 	 			)
 	 		);
 		FirstScrolledWindowBox->show();
	} else {
 		status->set_text("No EBUCore metadata detected");
 	 	sourceLabel->set_text("no source");
		sizeValue->set_text("0 byte");
		FirstScrolledWindowBox->hide();
	}
	// make the switch sensitive or not
	EBUCoreSet->set_sensitive((genericFeatures::getFileSize(filename) > 0 or createNew));
}

unsigned int metadataEngine::getViewportSize
(
	void
)
{
	return viewportTreeMinimumWidth;
}



void metadataEngine::constructTreeViewNew
(
	std::string MXFFile
) 
{
	
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation * impl = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	// Create a document & writer
	xercesc::DOMDocument * doc = impl->createDocument
	(
		0,// root element namespace URI
		xercesc::XMLString::transcode("ebuCoreMain"),// root element name
		0 // document type object (DTD)
	);
	xercesc::DOMElement * dom_root = doc->getDocumentElement();
	dom_root->setAttribute(xercesc::XMLString::transcode("version"), xercesc::XMLString::transcode("1.4"));
	xercesc::DOMElement*  elCoreMetadata = doc->createElement(xercesc::XMLString::transcode("coreMetadata"));
	dom_root->appendChild(elCoreMetadata);
	xercescdoc = doc;
	// refresh the viewport and the first expander
	// to display properly the new bunch of metadata
	viewportTree->remove();
	expanderRoot->remove();
	previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
	// enable html tag support
	expanderRoot->set_use_markup(true);
	// colorize the filename	
	viewportTreeMinimumWidth = 0;
	expanderRoot->set_margin_left(0);
	expanderRoot->override_color(blue, Gtk::STATE_FLAG_NORMAL);
	expanderRoot->set_label(genericFeatures::removeSuffix(MXFFile, "/"));
	expanderRoot->set_name("0");
	expanderRoot->set_expanded(false);
	expanderRoot->property_expanded().signal_changed().connect
	(
		sigc::bind<Gtk::Expander *>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataEngine::on_openExpander_changed
			), 
			expanderRoot
		)
	);
	viewportTreeMinimumWidth = ((genericFeatures::removeSuffix(MXFFile, "/")).size()*8 > viewportTreeMinimumWidth)? (genericFeatures::removeSuffix(MXFFile, "/")).size()*8 : viewportTreeMinimumWidth;
	expanderRoot->set_margin_left(0);
	// build the new xml tree
	elReferences.clear();
	elSchemaRef.clear();
	schemaPath = "";
	elSchemaRef.push_back(schemaPath);
	// store the DOM element address
	elReferences.push_back(dom_root);
	previouslabel = false;
	delete boxEntries;
 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	recursiveConstructTreeView(dom_root, expanderRoot,0);
	boxEntries->show();
	// add the expander to the viewport
	viewportTree->add(*expanderRoot);
	viewportTree->set_size_request(viewportTreeMinimumWidth,-1);
	SecondScrolledWindowBox->add(*boxEntries);
	ebucorename = genericFeatures::removeSuffix(MXFFile, "/");
	//panedWindow->set_position(viewportTreeMinimumWidth);
	std::cout<<"Size of viewport"<<viewportTreeMinimumWidth<<std::endl;
	std::cout<<"Size of the left child"<<panedWindow->get_child1()->get_width()<<std::endl;
	std::cout<<"Size of the right child"<<panedWindow->get_child2()->get_width()<<std::endl;
	std::cout<<"Position of the slider"<<panedWindow->get_position()<<std::endl;
	FirstScrolledWindowBox->set_size_request(panedWindow->get_child1()->get_width(),-1);
	SecondScrolledWindowBox->set_size_request(panedWindow->get_child2()->get_width(),-1);
	panedWindow->set_position(viewportTreeMinimumWidth);
	window->set_size_request(viewportTreeMinimumWidth+700,-1);
}

void metadataEngine::reconstructTreeView
(
	void
) 
{
	// Create a document & writer
	xercesc::DOMDocument * doc = xercescdoc;
	xercesc::DOMElement * dom_root = doc->getDocumentElement();
	// refresh the viewport and the first expander
	// to display properly the new bunch of metadata
	viewportTree->remove();
	expanderRoot->remove();
	previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
	// enable html tag support
	expanderRoot->set_use_markup(true);
	// colorize the filename	
	viewportTreeMinimumWidth = 0;
	expanderRoot->set_margin_left(0);
	expanderRoot->override_color(blue, Gtk::STATE_FLAG_NORMAL);
	expanderRoot->set_label(ebucorename);
	expanderRoot->set_name("0");
	expanderRoot->set_expanded(false);
	expanderRoot->property_expanded().signal_changed().connect
	(
		sigc::bind<Gtk::Expander *>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataEngine::on_openExpander_changed
			), 
			expanderRoot
		)
	);
	viewportTreeMinimumWidth = ((ebucorename).size()*8 > viewportTreeMinimumWidth)? (ebucorename).size()*8 : viewportTreeMinimumWidth;
	expanderRoot->set_margin_left(0);
	// build the new xml tree
	elReferences.clear();
	elSchemaRef.clear();
	schemaPath = "";
	elSchemaRef.push_back(schemaPath);
	// store the DOM element address
	elReferences.push_back(dom_root);
	previouslabel = false;
	delete boxEntries;
 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	recursiveConstructTreeView(dom_root, expanderRoot,0);
	boxEntries->show();
	// add the expander to the viewport
	viewportTree->add(*expanderRoot);
	viewportTree->set_size_request(viewportTreeMinimumWidth,-1);
	SecondScrolledWindowBox->add(*boxEntries);
}

void metadataEngine::constructTreeViewFromXML
(
	std::string XMLfile
)
{
	// Initialize xerces-c to read an xml file
	// and then parse it
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xml = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xml->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	xercesc::DOMDocument* dom_doc  = dom_file->parseURI(XMLfile.c_str());
	xercescdoc = dom_doc;
	xercesc::DOMElement*  dom_root = dom_doc->getDocumentElement();
	// refresh the viewport and the first expander
	// to display properly the new bunch of metadata
	viewportTree->remove();
	expanderRoot->remove();
	previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
	// enable html tag support
	expanderRoot->set_use_markup(true);
	// colorize the filename	
	viewportTreeMinimumWidth = 0;
	expanderRoot->set_margin_left(0);
	expanderRoot->override_color(blue, Gtk::STATE_FLAG_NORMAL);
	expanderRoot->set_label(genericFeatures::removeSuffix(XMLfile, "/"));
	expanderRoot->set_name("0");
	expanderRoot->set_expanded(false);
	expanderRoot->property_expanded().signal_changed().connect
	(
		sigc::bind<Gtk::Expander *>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataEngine::on_openExpander_changed
			), 
			expanderRoot
		)
	);
	viewportTreeMinimumWidth = ((genericFeatures::removeSuffix(XMLfile, "/")).size()*8 > viewportTreeMinimumWidth)? (genericFeatures::removeSuffix(XMLfile, "/")).size()*8 : viewportTreeMinimumWidth;
	expanderRoot->set_margin_left(0);
	// build the new xml tree
	elReferences.clear();
	elSchemaRef.clear();
	schemaPath = "";
	elSchemaRef.push_back(schemaPath);
	// store the DOM element adress
	elReferences.push_back(dom_root);
	previouslabel = false;
	delete boxEntries;
 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	recursiveConstructTreeView(dom_root, expanderRoot,0);
	boxEntries->show();
	// add the expander to the viewport
	viewportTree->add(*expanderRoot);
	viewportTree->set_size_request(viewportTreeMinimumWidth,-1);
	SecondScrolledWindowBox->add(*boxEntries);
	ebucorename = genericFeatures::removeSuffix(XMLfile, "/");
	window->set_size_request(viewportTreeMinimumWidth+700,-1);
}

void metadataEngine::constructTreeViewFromMXF
(
	std::string MXFFile
) 
{
	SDKWrapper->UnwrapEBUCore(MXFFile);
	if (SDKWrapper->getEBUCore() != 0) {
		xercesc::XMLPlatformUtils::Initialize();
		xercescdoc = SDKWrapper->getEBUCore();
		xercesc::DOMElement*  dom_root = ((SDKWrapper->getEBUCore())->getDocumentElement());
		//xercesc::XMLPlatformUtils::Terminate();
		// refresh the viewport and the first expander
		// to display properly the new bunch of metadata
		viewportTree->remove(); // clear viewport
		expanderRoot->remove(); // clear expander content
		previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
		// enable html tag support
		expanderRoot->set_use_markup(true);
		// set font color of the label expander
		viewportTreeMinimumWidth = 0;
		expanderRoot->set_margin_left(0);
		expanderRoot->override_color(blue, Gtk::STATE_FLAG_NORMAL); // set color
		expanderRoot->set_label(genericFeatures::removeSuffix(MXFFile, "/")); // set the label
		expanderRoot->set_name("0"); // set the name
		expanderRoot->set_expanded(false); // force the expander to be not expanded
		expanderRoot->property_expanded().signal_changed().connect
		(
			sigc::bind<Gtk::Expander *>
			(
				sigc::mem_fun
				(
					*this, 
					&metadataEngine::on_openExpander_changed
				), 
				expanderRoot
			)
		);
		viewportTreeMinimumWidth = ((genericFeatures::removeSuffix(MXFFile, "/")).size()*8 > viewportTreeMinimumWidth)? (genericFeatures::removeSuffix(MXFFile, "/")).size()*8 : viewportTreeMinimumWidth;
		// build the XML tree
		elReferences.clear();
		elSchemaRef.clear();
		schemaPath = "";
		elSchemaRef.push_back(schemaPath);
		previouslabel = false;
		delete boxEntries;
	 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
		recursiveConstructTreeView(dom_root, expanderRoot,0);
		boxEntries->show();
		// add the new expander to the viewport
		viewportTree->add(*expanderRoot);
		viewportTree->set_size_request(viewportTreeMinimumWidth,-1);
		SecondScrolledWindowBox->add(*boxEntries);		
		ebucorename = genericFeatures::removeSuffix(MXFFile, "/");
	window->set_size_request(viewportTreeMinimumWidth+700,-1);
	}
}

void metadataEngine::embedEBUCore
(
	std::string MXFFilename,
	bool setHeader,
	bool isDark,
	bool isSidecar
)
{
	SDKWrapper->WrapEBUCore(MXFFilename, setHeader, isDark, isSidecar);
}
		
void metadataEngine::embedEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreFilename,
	bool setHeader,
	bool isDark,
	bool isSidecar
)
{
	std::cout<<"Try to embed metadata"<<std::endl;
	SDKWrapper->WrapEBUCore(MXFFilename,XMLEBUCoreFilename,setHeader,isDark,isSidecar);
	std::cout<<"Finish to embed metadata"<<std::endl;
}
	
		
void metadataEngine::removeEBUCore
(
	std::string MXFFilename
)
{
	SDKWrapper->eraseEBUCore(MXFFilename);
}
		
void metadataEngine::removeEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreFilename
)
{
	SDKWrapper->eraseEBUCore(MXFFilename,XMLEBUCoreFilename);
}
		
void metadataEngine::exportMXF2XMLEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreOutputFilename
)
{
	SDKWrapper->UnwrapEBUCore(MXFFilename,XMLEBUCoreOutputFilename);
}
		
void metadataEngine::exportXMLEBUCore
(
	std::string XMLEBUCoreOutputFilename
)
{
	//xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* impl = xercesc::DOMImplementationRegistry::getDOMImplementation(0);
	
	    // Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document. 
    xercesc::DOMLSSerializer *serializer = ((xercesc::DOMImplementationLS*)impl)->createLSSerializer(); 

    // Make the output more human readable by inserting line feeds. 
    if (serializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true)) 
        serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true); 

    // The end-of-line sequence of characters to be used in the XML being written out.  
    serializer->setNewLine(xercesc::XMLString::transcode("\r\n"));  

    // Specify the target for the XML output. 
   xercesc::LocalFileFormatTarget *formatTarget = new xercesc::LocalFileFormatTarget(XMLEBUCoreOutputFilename.c_str()); 

    // Create a new empty output destination object. 
    xercesc::DOMLSOutput *output = ((xercesc::DOMImplementationLS*)impl)->createLSOutput(); 

    // Set the stream to our target. 
    output->setByteStream(formatTarget); 

    // Write the serialized output to the destination. 
    serializer->write(xercescdoc, output); 

    // Cleanup. 
    serializer->release(); 
    delete formatTarget; 
    output->release(); 
}

void metadataEngine::reportMetadata
(
	std::string MXFInputFilename,
	std::string XMLOutputFilename
)
{
	SDKWrapper->reportEBUCoreSTMetadata(MXFInputFilename, XMLOutputFilename);
}


void metadataEngine::reportMux
(
	std::string MXFInputFilename,
	std::string XMLOutputFilename
)
{
	SDKWrapper->reportEBUCoreSTMux(MXFInputFilename, XMLOutputFilename);
}


void metadataEngine::reportDeep
(
	std::string MXFInputFilename,
	std::string XMLOutputFilename
)
{
	SDKWrapper->reportEBUCoreSTDeep(MXFInputFilename, XMLOutputFilename);
}

///////////////////////////////
//////////////////////////////
///////////////////////////////
////////////////////////////

void metadataEngine::recursiveConstructTreeView
(
	xercesc::DOMElement * el, 
	Gtk::Expander * seed, 
	int depth
)
{
	// create a new box to encapsulte the tree levels
	Gtk::Box * root = manage(new Gtk::VBox());

	// configure the new box
	configureEncapsultedBox(root);	
	do {
		viewportTreeMinimumWidth = (((xercesc::XMLString::stringLen(el->getTagName())*7)+(depth*16)) > viewportTreeMinimumWidth) ? ((xercesc::XMLString::stringLen(el->getTagName())*7)+(depth*16)) : viewportTreeMinimumWidth;
		unsigned int lengthPathToRemove = 1;
		if (el->hasChildNodes() and el->getChildElementCount() != 0) {
			// create a new expander to store the node children
			Gtk::Expander * node = manage( new Gtk::Expander(xercesc::XMLString::transcode(el->getTagName())) );
			// parse the schema path
			schemaPath = schemaPath+"#"+xercesc::XMLString::transcode(el->getTagName());
			lengthPathToRemove += xercesc::XMLString::stringLen(el->getTagName());
			elSchemaRef.push_back(schemaPath);
			// set a name/id to the node
//			node->set_name(std::to_string(cptnode++));
			node->set_name(std::to_string(elReferences.size()));
			// store the DOM element adress
			elReferences.push_back(el);
			// configure the new expander
			configureExpander(node);
			// end encapsultation level
			finishEncapsulation(root,node);
			// visit the first children of the current node
			recursiveConstructTreeView(el->getFirstElementChild(), node ,depth+1);
		} else {
			Gtk::EventBox *eventLabel = manage(new Gtk::EventBox);
			eventLabel->set_name(std::to_string(elReferences.size()));
			// store the DOM element adress
			elReferences.push_back(el);
			// create a new label to store the node children
			Gtk::Label * label = manage( new Gtk::Label(xercesc::XMLString::transcode(el->getTagName())));
			// parse the schema path
			schemaPath = schemaPath+"#"+xercesc::XMLString::transcode(el->getTagName());
			lengthPathToRemove += xercesc::XMLString::stringLen(el->getTagName());
			elSchemaRef.push_back(schemaPath);
			/// configure the new label
			configureLabel(label);
			/// configure the new eventbox and encapsulate the label
			configureEventLabel(eventLabel,label);
			// end encapsultation level
			finishEncapsulation(root,eventLabel);
		}
		// remove last schema path floor
		schemaPath.erase(schemaPath.size()-lengthPathToRemove, lengthPathToRemove);
		// next node at this level
		el = el->getNextElementSibling();
	} while (el != 0);
	// add and show...
	seed->add(*root);
	root->show();
}

// configure the gtk box encapsulated in the gtk expander
void metadataEngine::configureEncapsultedBox
(
	Gtk::Box * b
) 
{
	// define the vertical alignment
	b->set_valign(Gtk::ALIGN_START);
	// define the left margin (treeview indentation)
	b->set_margin_left(b->get_margin_left()+16);
}

// configure the gtk expander before to encapsulate it into a Gtk::Box
void metadataEngine::configureExpander
(
	Gtk::Expander * e
) 
{
		// signal handler to detect what expander has been expanded
	e->property_expanded().signal_changed().connect
	(
		sigc::bind<Gtk::Expander *>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataEngine::on_openExpander_changed
			), 
			e
		)
	);
	e->set_margin_left(0);
	// override font color
	e->override_color (black, Gtk::STATE_FLAG_NORMAL);
	// use the pango markup language
	e->set_use_markup(true);
	// set the expander label
	e->set_label("<b>"+e->get_label()+"</b>");
}

// configure the gtk expander before to encapsulate it into a Gtk::Box
void metadataEngine::configureEventLabel
(
	Gtk::EventBox * evLabel, 
	Gtk::Label * label
)
{
	// add the label to its eventbox
	evLabel->add(*label);
	// connect mouse detection events
	evLabel->signal_enter_notify_event().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
				*this, 
				&metadataEngine::on_enter_label
			),
			evLabel
		)
	);
	evLabel->signal_leave_notify_event().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
				*this, 
				&metadataEngine::on_leave_label
			),
			evLabel
		)
	);
	evLabel->signal_button_press_event().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
			
				*this, 
				&metadataEngine::on_press_label
			),
			evLabel,
			label
		)
	);
	// initialize eventBox background color
	evLabel->override_background_color (white, Gtk::STATE_FLAG_NORMAL);
}

// configure the gtk label before to encapsulate it into a Gtk::Box
void metadataEngine::configureLabel
(
	Gtk::Label * l
) 
{
	l->set_justify(Gtk::JUSTIFY_LEFT);
	l->set_halign(Gtk::ALIGN_START);
	l->override_color(black, Gtk::STATE_FLAG_NORMAL);
	l->set_markup("<b>"+l->get_text()+"</b>");
	l->set_margin_left(l->get_margin_left()+18);
}

void metadataEngine::finishEncapsulation
(
	Gtk::Box * root, 
	Gtk::Expander * node
)
{
	node->show();
	root->add(*node);
}

void metadataEngine::finishEncapsulation
(
	Gtk::Box * root, 
	Gtk::EventBox * evLabel
)
{
	root->add(*evLabel);
	evLabel->show_all();
}

void metadataEngine::constructEditableNode
(
	xercesc::DOMElement * el
)
{
	// grab the boxEntry child
	std::vector< Gtk::Widget* > childbox = boxEntries->get_children();
	// and delete it
	if (childbox.size()>0) {
		boxEntries->remove(*childbox.at(0));
		delete childbox.at(0);
	}
		
	Gtk::Box * entryLevel = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,10));
	entryLevel->set_size_request(120, 150);
	entryLevel->set_border_width(10);
	
	Gtk::Entry * editableEntry = manage(new Gtk::Entry());
	editableEntry->set_text(xercesc::XMLString::transcode(el->getTagName()));
	editableEntry->drag_highlight();
	editableEntry->set_has_frame(false);
	editableEntry->set_editable(false);
	editableEntry->show();

	Gtk::Label * nonEditableEntry = manage(new Gtk::Label(xercesc::XMLString::transcode(el->getTagName())));
	nonEditableEntry->set_alignment (Gtk::ALIGN_START,Gtk::ALIGN_CENTER);	
	nonEditableEntry->set_padding(4,4);
	nonEditableEntry->override_color(black, Gtk::STATE_FLAG_NORMAL);
	nonEditableEntry->drag_highlight();
	nonEditableEntry->show();
	

	Gtk::Box * nodeTypeBox = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,10));
	Gtk::Label * nodeTypeLabel = manage(new Gtk::Label("Type"));
	Gtk::Label * nodeTypeValue = manage(new Gtk::Label());
	nodeTypeLabel->show();
	nodeTypeValue->show();
	nodeTypeBox->add(*nodeTypeLabel);
	nodeTypeBox->add(*nodeTypeValue);
	nodeTypeBox->show();

	Gtk::Label * tagTitle = manage(new Gtk::Label("Tag name"));
	tagTitle->set_justify(Gtk::JUSTIFY_LEFT);
	tagTitle->set_halign(Gtk::ALIGN_START);
	tagTitle->show();		
	Gtk::Label * nodeContent = manage(new Gtk::Label());
	nodeContent->set_justify(Gtk::JUSTIFY_LEFT);
	nodeContent->set_halign(Gtk::ALIGN_START);
	nodeContent->show();


	if (el->hasChildNodes()) {
		
		// visit the first children of the current node
		if (el->getChildElementCount() != 0) {
			nodeTypeValue->set_text("Node");
			nodeContent->set_text("Node children");
		// if the node hasn't children then create a text box
		} else {
			// if it's a text node
			if (el->getFirstChild() != 0) {
			nodeTypeValue->set_text("Leaf");
			nodeContent->set_text("Node text");
			}
		}
	} else {
		nodeTypeValue->set_text("Lonely node");
		nodeContent->set_text("Empty node");
		Gtk::EventBox *eventLabel = manage(new Gtk::EventBox);
		// create a new label to store the node children
		Gtk::Label * label = manage( new Gtk::Label(xercesc::XMLString::transcode(el->getTagName())) );
		/// configure the new label
		configureLabel(label);
		/// configure the new eventbox and encapsulate the label
		configureEventLabel(eventLabel,label);
		entryLevel->add(*eventLabel);
	}
	entryLevel->add(*tagTitle);
	if (EBUCoreSet->get_active()) {
		entryLevel->add(*editableEntry);
		delete nonEditableEntry;
	} else {
		entryLevel->add(*nonEditableEntry);
		delete editableEntry;
	}
	entryLevel->add(*nodeTypeBox);

	Gtk::Label * nodeAttributeTitle = manage(new Gtk::Label("Node attributes"));
	nodeAttributeTitle->set_justify(Gtk::JUSTIFY_LEFT);
	nodeAttributeTitle->set_halign(Gtk::ALIGN_START);
	nodeAttributeTitle->show();

	entryLevel->add(*nodeAttributeTitle);

	//if (el->getAttributes()->getLength()>0) { 
	configureNodeAttributesTreeview(el->getAttributes(),entryLevel);
	entryLevel->add(*nodeContent);
	//}

	if (nodeContent->get_text() == "Empty node") {
		if (ebucoreRef.children.begin() != ebucoreRef.children.end()) 
		{
			nodeContent->set_text("Node children");
			configureNodeChildrenTreeview(el->getFirstElementChild(),entryLevel);
		} else {
			configureNodeText(xercesc::XMLString::transcode(el->getTextContent()),entryLevel);
		}		
	} else if (nodeContent->get_text() == "Node text") {
		configureNodeText(xercesc::XMLString::transcode(el->getTextContent()),entryLevel);
	} else if (nodeContent->get_text() == "Node children") {
		configureNodeChildrenTreeview(el->getFirstElementChild(),entryLevel);
	}
	entryLevel->show();
	boxEntries->add(*entryLevel);
}

void metadataEngine::on_openExpander_changed
(
	Gtk::Expander * exp
) 
{
	// update the previous textual breadcrumb if required
	if (previousnode->get_label() != "!###ImAnEmptyExpander###!" and !previouslabel) {		
		previousnode->override_color(black, Gtk::STATE_FLAG_NORMAL);
		previousnode->set_label(previousnode->get_label());
	} else if (previouslabel) {
		previousnodelabel->override_color(black, Gtk::STATE_FLAG_NORMAL);
		previouslabel = !previouslabel;
	}
	exp->override_color(red, Gtk::STATE_FLAG_NORMAL);
	exp->set_label(exp->get_label());
	previousnode = exp;
	previousnodepos = atoi(exp->get_name().c_str());
	ebucoreRef = SDKWrapper->getReference(elSchemaRef.at(previousnodepos));
	constructEditableNode(elReferences.at(previousnodepos));
	if (previousnode->get_name() == "0") {
		boxStatus->show();
		boxEntries->hide();
	} else {
		boxStatus->hide();
		boxEntries->show();
	}
}

bool metadataEngine::on_enter_label
(
	GdkEventCrossing*, 
	Gtk::EventBox *evLabel
)
{
	// reset the event box background color
	evLabel->unset_background_color(Gtk::STATE_FLAG_NORMAL);
	return true;
}

bool metadataEngine::on_leave_label
(
	GdkEventCrossing*, 
	Gtk::EventBox *evLabel
)
{
	// set the event box background color as white
	evLabel->override_background_color (white, Gtk::STATE_FLAG_NORMAL);
	return true;
}

bool metadataEngine::on_press_label
(
	GdkEventButton* event, 
	Gtk::EventBox *evLabel, 
	Gtk::Label * txtLabel
) 
{
	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 1) ) {
		if (previousnode->get_label() != "!###ImAnEmptyExpander###!" and !previouslabel) {
			previousnode->override_color(black, Gtk::STATE_FLAG_NORMAL);
			previousnode->set_label(previousnode->get_label());
			previouslabel = !previouslabel;
		} else if (previouslabel) {
			previousnodelabel->override_color(black, Gtk::STATE_FLAG_NORMAL);
		}
		txtLabel->override_color(red,Gtk::STATE_FLAG_NORMAL);
		previousnodepos = atoi(evLabel->get_name().c_str());
		previousnodelabel = txtLabel;
		ebucoreRef = SDKWrapper->getReference(elSchemaRef.at(previousnodepos));
		constructEditableNode(elReferences.at(previousnodepos));
		if (previousnode->get_name() == "0") {
			boxStatus->show();boxEntries->hide();
		} else {
			boxStatus->hide();boxEntries->show();
		}
		// signal has been handled.		
    return true;
  }
  return false;
}

bool metadataEngine::on_press_sample
(
	GdkEventButton* event, 
	Gtk::EventBox *evLabel, 
	Gtk::Label * txtLabel
) 
{
	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 1) ) {
    return true;
  }
  return false;
}

void metadataEngine::configureNodeAttributesTreeview
(
	xercesc::DOMNamedNodeMap *dom_attrs, 
	Gtk::Box * nodebox
) 
{

	Gtk::Box * AttributesTreeviewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	AttributesTreeviewBox->set_size_request(120, 150);

	Gtk::ScrolledWindow * metadataNodeAttributesScrolledWindow = manage(new Gtk::ScrolledWindow());
	metadataNodeAttributesScrolledWindow->set_size_request(120, 150);

	Gtk::TreeView * metadataNodeAttributesTreeview = manage(new Gtk::TreeView());
	metadataNodeAttributesTreeview->drag_highlight();

//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  metadataNodeAttributesScrolledWindow->add(*metadataNodeAttributesTreeview);

  //Only show the scrollbars when they are necessary:
  metadataNodeAttributesScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Glib::RefPtr<Gtk::ListStore> metadataNodeAttributesStore;
 // Glib::RefPtr<Gtk::TreeSelection> metadataNodeAttributesTreeviewSelection;

	metadataNodeAttributesStore.clear();
	// create the data model
	metadataNodeAttributesStore = Gtk::ListStore::create(metadataNodeAttributesColumns);
	// set the data model
  metadataNodeAttributesTreeview->set_model(metadataNodeAttributesStore);
	//Add the TreeView's view columns
  metadataNodeAttributesTreeview->append_column("", metadataNodeAttributesColumns.metadataNodeAttributeIdCol);
  metadataNodeAttributesTreeview->append_column("Name", metadataNodeAttributesColumns.metadataNodeAttributeNameCol);
  metadataNodeAttributesTreeview->append_column("Value", metadataNodeAttributesColumns.metadataNodeAttributeValueCol);
  metadataNodeAttributesTreeview->append_column("Bgcolor", metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol);

	Gtk::TreeView::Column* pColumn;
	for(guint i = 0; i < 3; i++)
  { 
		pColumn = metadataNodeAttributesTreeview->get_column(i);
		Gtk::CellRenderer* cellRenderer = metadataNodeAttributesTreeview->get_column_cell_renderer(i);
		pColumn->add_attribute(cellRenderer->property_cell_background(), metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol);
    pColumn->set_reorderable();
		switch(i) {
			case 1 : pColumn->set_sort_column(metadataNodeAttributesColumns.metadataNodeAttributeNameCol); break;
			case 2 : pColumn->set_sort_column(metadataNodeAttributesColumns.metadataNodeAttributeValueCol); break;
		}
  }
	pColumn = metadataNodeAttributesTreeview->get_column(3);
	pColumn->set_visible(false);
	//All the items to be reordered with drag-and-drop
	// Set the visibility state of the headers. 
	metadataNodeAttributesTreeview->set_headers_visible(true);
	// Allow the column title buttons to be clicked
	metadataNodeAttributesTreeview->set_headers_clickable(true);
	// Resize all columns to their optimal width. 
	metadataNodeAttributesTreeview->columns_autosize();
	// all element are reordorable
	metadataNodeAttributesTreeview->set_reorderable();
  metadataNodeAttributesTreeview->set_rules_hint();
	// grab the treeview selection
	metadataNodeAttributesTreeviewSelection = metadataNodeAttributesTreeview->get_selection();
	// connect signal to dectect when tree selection change
	//metadataNodeAttributesTreeview->signal_button_press_event().connect(sigc::mem_fun(*this,
  //            &metadataWindow::on_metadata_selection_changed),false);

	attributeList.clear();
	int num= dom_attrs->getLength();
	if (num != 0) {
		for(int i=0; i<num;i++) {
			Gtk::TreeModel::Row row = *(metadataNodeAttributesStore->prepend());
			xercesc::DOMAttr* dom_attr = (xercesc::DOMAttr*) dom_attrs->item(i);
			row[metadataNodeAttributesColumns.metadataNodeAttributeIdCol] = i;
			row[metadataNodeAttributesColumns.metadataNodeAttributeNameCol] = xercesc::XMLString::transcode(dom_attr->getName());
			row[metadataNodeAttributesColumns.metadataNodeAttributeValueCol] = xercesc::XMLString::transcode(dom_attr->getValue());
			row[metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol] = (i%2==0)?"white":"ghostwhite";
			attributeList.push_back(xercesc::XMLString::transcode(dom_attr->getName()));
			attributeList.push_back(xercesc::XMLString::transcode(dom_attr->getValue()));
		}		
	}

	AttributesTreeviewBox->add(*metadataNodeAttributesScrolledWindow);
	
	AttributesTreeviewBox->show();
	metadataNodeAttributesScrolledWindow->show();
	metadataNodeAttributesTreeview->show();

	nodebox->add(*AttributesTreeviewBox);

	if (EBUCoreSet->get_active()) {
		configureNodeAttributeEditionButtonsTreeview(nodebox, metadataNodeAttributesStore);
	}
}

void metadataEngine::configureNodeChildrenTreeview
(
	xercesc::DOMElement * children, 
	Gtk::Box * nodebox
) 
{

	Gtk::Box * ChildrenTreeviewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	ChildrenTreeviewBox->set_size_request(120, 150);

	Gtk::ScrolledWindow * metadataNodeChildrenScrolledWindow = manage(new Gtk::ScrolledWindow());
	metadataNodeChildrenScrolledWindow->set_size_request(120, 150);

	Gtk::TreeView * metadataNodeChildrenTreeview = manage(new Gtk::TreeView());
	metadataNodeChildrenTreeview->drag_highlight();

//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  metadataNodeChildrenScrolledWindow->add(*metadataNodeChildrenTreeview);

  //Only show the scrollbars when they are necessary:
  metadataNodeChildrenScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Glib::RefPtr<Gtk::ListStore> metadataNodeChildrenStore;
  //Glib::RefPtr<Gtk::TreeSelection> metadataNodeChildrenTreeviewSelection;
	
	metadataNodeChildrenStore.clear();
	// create the data model
	metadataNodeChildrenStore = Gtk::ListStore::create(metadataNodeChildrenColumns);
	// set the data model
  metadataNodeChildrenTreeview->set_model(metadataNodeChildrenStore);

	//Add the TreeView's view columns
  metadataNodeChildrenTreeview->append_column("", metadataNodeChildrenColumns.metadataNodeChildrenIdCol);
  metadataNodeChildrenTreeview->append_column("Name", metadataNodeChildrenColumns.metadataNodeChildrenNameCol);
  metadataNodeChildrenTreeview->append_column("Bgcolor", metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol);

	Gtk::TreeView::Column* pColumn;
	for(guint i = 1; i < 2; i++)
  { 
		pColumn = metadataNodeChildrenTreeview->get_column(i);
		Gtk::CellRenderer* cellRenderer = metadataNodeChildrenTreeview->get_column_cell_renderer(i);
		pColumn->add_attribute(cellRenderer->property_cell_background(), metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol);
    pColumn->set_reorderable();
		switch(i) {
			case 1 : pColumn->set_sort_column(metadataNodeChildrenColumns.metadataNodeChildrenNameCol); break;
		}
  }

	pColumn = metadataNodeChildrenTreeview->get_column(2);
	pColumn->set_visible(false);
	
	//All the items to be reordered with drag-and-drop
	// Set the visibility state of the headers. 
	metadataNodeChildrenTreeview->set_headers_visible(true);
	// Allow the column title buttons to be clicked
	metadataNodeChildrenTreeview->set_headers_clickable(true);
	// Resize all columns to their optimal width. 
	metadataNodeChildrenTreeview->columns_autosize();
	// all element are reordorable
	metadataNodeChildrenTreeview->set_reorderable();
  metadataNodeChildrenTreeview->set_rules_hint();
	// grab the treeview selection
	metadataNodeChildrenTreeviewSelection = metadataNodeChildrenTreeview->get_selection();
	int i=0;
	for (xercesc::DOMElement * nodechildren = children; nodechildren != 0	; nodechildren = nodechildren->getNextElementSibling()) {
  	Gtk::TreeModel::Row row = *(metadataNodeChildrenStore->prepend());
		// track position
		row[metadataNodeChildrenColumns.metadataNodeChildrenIdCol] = i;
		row[metadataNodeChildrenColumns.metadataNodeChildrenNameCol] = xercesc::XMLString::transcode(nodechildren->getTagName());
		row[metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol] = (i%2==0)?"white":"ghostwhite";
		i++;
	}

	ChildrenTreeviewBox->show();
	metadataNodeChildrenScrolledWindow->show();
	metadataNodeChildrenTreeview->show();

	ChildrenTreeviewBox->add(*metadataNodeChildrenScrolledWindow);

	nodebox->add(*ChildrenTreeviewBox);

	if (EBUCoreSet->get_active()) {
		configureNodeEditionButtonsTreeview(nodebox, metadataNodeChildrenStore);
	}
}

void metadataEngine::configureNodeText
(
	std::string nodetext, 
	Gtk::Box * nodebox
)
{

	Gtk::Box * nodeTextViewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	nodeTextViewBox->set_size_request(120, 150);

	Gtk::ScrolledWindow * nodeTextViewScrolledWindow = manage(new Gtk::ScrolledWindow());
	nodeTextViewScrolledWindow->set_size_request(120, 150);

	Gtk::TextView * nodeTextView = manage (new Gtk::TextView());
	nodeTextView->override_background_color(((EBUCoreSet->get_active())? white : whiteghost), Gtk::STATE_FLAG_NORMAL);
	
	nodeTextView->drag_highlight();
	nodeTextView->set_pixels_above_lines(2);
	nodeTextView->set_left_margin(2);
	nodeTextView->set_right_margin(2);

	nodeTextView->set_editable(EBUCoreSet->get_active());
  nodeTextView->set_cursor_visible(EBUCoreSet->get_active());
	nodeTextView->set_wrap_mode(Gtk::WRAP_WORD);

	metadataTextBuffer = Gtk::TextBuffer::create();
 	metadataTextBuffer->set_text(nodetext);
	// signal
	metadataTextBuffer->signal_end_user_action().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
				*this,
				&metadataEngine::on_textnode_changed
			),
			metadataTextBuffer->get_text()
		)
	);

	 nodeTextView->set_buffer(metadataTextBuffer);
  nodeTextViewScrolledWindow->add(*nodeTextView);
	nodeTextView->show();
  //Only show the scrollbars when they are necessary:
  nodeTextViewScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	nodeTextViewBox->add(*nodeTextViewScrolledWindow);
	nodeTextViewScrolledWindow->show();
	nodebox->add(*nodeTextViewBox);
	nodeTextViewBox->show();
}

void metadataEngine::on_textnode_changed
(
	std::string text
)
{
	elReferences.at(previousnodepos)->setTextContent(xercesc::XMLString::transcode(text.c_str()));
	reconstructTreeView();

	std::cout<<"some text is typing..."<<std::endl;
}

void metadataEngine::configureNodeAttributeEditionButtonsTreeview
(
	Gtk::Box * nodebox, 
	Glib::RefPtr<Gtk::ListStore> attributeStore
)
{
	Gtk::Box * EditButtonBox = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));
	EditButtonBox->set_hexpand (true);	
	EditButtonBox->set_halign (Gtk::ALIGN_END);	
	
	Gtk::Button *addButton = manage(new Gtk::Button());
	Gtk::Button *removeButton = manage(new Gtk::Button());

	Gtk::Image *addImage = manage(new Gtk::Image(Gtk::Stock::ADD,Gtk::ICON_SIZE_MENU));
	Gtk::Image *removeImage = manage(new Gtk::Image(Gtk::Stock::REMOVE,Gtk::ICON_SIZE_MENU));

	addButton->set_image(*addImage);
	removeButton->set_image(*removeImage);

	addButton->signal_clicked().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
				*this, 
				&metadataEngine::addNodeAttribute
			),
			attributeStore
		)
	);
	removeButton->signal_clicked().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
				*this,
				&metadataEngine::removeNodeAttribute
			),
			attributeStore
		)
	);

	EditButtonBox->add(*addButton);
	EditButtonBox->add(*removeButton);

	addButton->show();
	removeButton->show();
	EditButtonBox->show();

	nodebox->add(*EditButtonBox);
}

void metadataEngine::configureNodeEditionButtonsTreeview
(
	Gtk::Box * nodebox, 
	Glib::RefPtr<Gtk::ListStore> nodeStore
) 
{
	Gtk::Box * EditButtonBox = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));
	EditButtonBox->set_hexpand (true);	
	EditButtonBox->set_halign (Gtk::ALIGN_END);	
	
	Gtk::Button *addButton = manage(new Gtk::Button());
	Gtk::Button *removeButton = manage(new Gtk::Button());

	Gtk::Image *addImage = manage(new Gtk::Image(Gtk::Stock::ADD,Gtk::ICON_SIZE_MENU));
	Gtk::Image *removeImage = manage(new Gtk::Image(Gtk::Stock::REMOVE,Gtk::ICON_SIZE_MENU));

	addButton->set_image(*addImage);
	removeButton->set_image(*removeImage);

	addButton->signal_clicked().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
				*this,
				&metadataEngine::addNode
			),
			nodeStore
		)
	);
	removeButton->signal_clicked().connect
	(
		sigc::bind
		(
			sigc::mem_fun
			(
				*this, &metadataEngine::removeNode
			),
			nodeStore
		)
	);

	EditButtonBox->add(*addButton);
	EditButtonBox->add(*removeButton);

	addButton->show();
	removeButton->show();
	EditButtonBox->show();

	nodebox->add(*EditButtonBox);
}

void metadataEngine::on_attribute_changed
(
	void
)
{
	switch (addComboBoxText->get_active_row_number()) 
	{
		case 0:
		{
			attributeValueBox->hide();
			break;
		} 
		default :
		{
			attributeValueBox->show();
			for (std::list<EBUCoreFeatures::AttributeStruct>::iterator it=ebucoreRef.attribute.begin() ; it != ebucoreRef.attribute.end(); ++it) 
			{
				EBUCoreFeatures::AttributeStruct attr = *it;
				if (attr.name == addComboBoxText->get_active_text())
				{
					std::string attribute = get_attribute_value(attr.name);
					
					delete attributeValueEntry;
					// instanciate a new entry
					attributeValueEntry = new customEntry
					(
						attr.type, "Set the "+attr.name+" value", attribute
					);
					attributeValueEntry->show();
					delete attributeValueSample;
					attributeValueSample = attributeValueEntry->get_sample();
					attributeValueSample->show();
					delete attributeValueDescription;
					attributeValueDescription = attributeValueEntry->get_description();
					attributeValueDescription->show();
					attributeValueBox->add(*attributeValueEntry);
					attributeValueBox->add(*attributeValueDescription);
					attributeValueBox->add(*attributeValueSample);
					break;
				}
			}
		}
	}
}

std::string metadataEngine::get_attribute_value
(
	std::string name
) 
{
	for (unsigned int i=0; i<attributeList.size()/2;i++) {
		if (attributeList.at(2*i) == name) {
			return attributeList.at((2*i)+1);
		}
	}
	return "";
}

void metadataEngine::on_node_changed
(
	void
)
{
	switch (addComboBoxText->get_active_row_number()) 
	{
		case 0:
		{
			attributeValueBox->hide();
			break;
		} 
		default :
		{
			attributeValueBox->show();
			for (std::list<EBUCoreFeatures::ElementStruct>::iterator it=ebucoreRef.children.begin() ; it != ebucoreRef.children.end(); ++it) 
			{
				EBUCoreFeatures::ElementStruct elem = *it;
				if (elem.name == addComboBoxText->get_active_text())
				{
					delete attributeValueEntry;
					// instanciate a new entry
					attributeValueEntry = new customEntry
					(
						elem.type, "Set the "+elem.name+" value"
					);
					attributeValueEntry->show();
					delete attributeValueSample;
					attributeValueSample = attributeValueEntry->get_sample();
					attributeValueSample->show();
					delete attributeValueDescription;
					attributeValueDescription = attributeValueEntry->get_description();
					attributeValueDescription->show();
					attributeValueBox->add(*attributeValueEntry);
					attributeValueBox->add(*attributeValueDescription);
					attributeValueBox->add(*attributeValueSample);
					break;
				}
			}
		}
	}
}

void metadataEngine::addNodeAttribute
(
	Glib::RefPtr<Gtk::ListStore> metadataStore
) 
{	
  	//add to the treeview
	metadataAssistant = new Gtk::Assistant();
	metadataAssistant->set_modal(true);
	metadataAssistant->set_title("Add a node attribute");

	Gtk::Box * page1 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));

////////////// page 1
	Gtk::Box * page1hbox1 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));	
	Gtk::Box * page1hbox2 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,1));
	Gtk::Box * page1hbox3 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));
	attributeValueBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,3));

	Gtk::AspectFrame * page1hbox1space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox2space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox3space1 = manage(new Gtk::AspectFrame());


	Gtk::Label * attributeNameTitle = manage(new Gtk::Label("Attribute :"));
	addComboBoxText = manage (new Gtk::ComboBoxText(false));
	addComboBoxText->append("Select an attribute");
	addComboBoxText->set_active(0);
	for (std::list<EBUCoreFeatures::AttributeStruct>::iterator it=ebucoreRef.attribute.begin() ; it != ebucoreRef.attribute.end(); ++it) 
	{
		EBUCoreFeatures::AttributeStruct attr = *it;
		addComboBoxText->append(attr.name);
	}
	addComboBoxText->signal_changed().connect
	(
		sigc::mem_fun
		(
			*this,
			&metadataEngine::on_attribute_changed
		)
	);
	
	page1hbox1->add(*attributeNameTitle);
	page1hbox1->add(*page1hbox1space1);
	page1hbox1->add(*addComboBoxText);
	page1hbox1->show_all_children();
	page1hbox1->show();

	page1hbox2->add(*page1hbox2space1);
	page1hbox2->show_all_children();
	page1hbox2->show();
	
	Gtk::Label * attributeValueTitle = manage(new Gtk::Label("Value :")); 
	// instanciate a new entry
	attributeValueEntry = new customEntry
	(
		"string", "Set the value"
	);
	page1hbox3->add(*attributeValueTitle);
	page1hbox3->add(*page1hbox3space1);
	page1hbox3->add(*attributeValueEntry);
	page1hbox3->show_all_children();
	
	attributeValueDescription = manage(new Gtk::Label("Description..."));
	attributeValueSample = manage(new Gtk::Label("Sample..."));
	
	attributeValueBox->add(*page1hbox3);
	attributeValueBox->add(*attributeValueDescription);
	attributeValueBox->add(*attributeValueSample);
	attributeValueBox->show_all_children();
	attributeValueBox->hide();

	page1->add(*page1hbox1);
	page1->add(*page1hbox2);
	page1->add(*attributeValueBox);
	page1->show();
	//page1->show_all_children();


	metadataAssistant->append_page(*page1);
	metadataAssistant->set_page_complete(*page1, true);	
	metadataAssistant->set_page_type(*page1, Gtk::ASSISTANT_PAGE_CONFIRM);


	metadataAssistant->signal_apply().connect
	(
		sigc::bind<Glib::RefPtr<Gtk::ListStore> >
		(
			sigc::mem_fun
			(
				*this,
				&metadataEngine::on_addAttribute_assistant_apply
			),
			metadataStore
		)
	);
	metadataAssistant->signal_cancel().connect
	(
		sigc::mem_fun
		(
			*this,
			&metadataEngine::on_assistant_cancel
		)
	);
	metadataAssistant->signal_close().connect
	(
		sigc::mem_fun
		(
			*this,
			&metadataEngine::on_assistant_close
		)
	);

	//metadataAssistant->show_all_children();
	metadataAssistant->show();
  	//add to the treeview
}

void metadataEngine::removeNodeAttribute
(
	Glib::RefPtr<Gtk::ListStore> metadataStore
) 
{
	Gtk::TreeModel::iterator iter = metadataNodeAttributesTreeviewSelection->get_selected();
(elReferences.at(previousnodepos))->removeAttribute(xercesc::XMLString::transcode(((Glib::ustring)((*iter)[metadataNodeAttributesColumns.metadataNodeAttributeNameCol])).c_str()));
	metadataStore->erase(iter);
}

bool metadataEngine::cardinalityAllows
(
	std::string name,
	int max, 
	Glib::RefPtr<Gtk::ListStore> store
)
{
	if (max >= 0) {
		int cpt = 0;
		unsigned int nb = store->children().size();
		for (unsigned int i = 0; i<nb; i++)
		{
			if (store->children()[i][metadataNodeChildrenColumns.metadataNodeChildrenNameCol] == name) {
				cpt++;
			}
		}
		if (cpt == 0 or cpt<max) {
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
}

void metadataEngine::addNode
(
	Glib::RefPtr<Gtk::ListStore> metadataStore
)
{

 	//add to the treeview
	metadataAssistant = new Gtk::Assistant();
	metadataAssistant->set_modal(true);
	metadataAssistant->set_title("Add a node");

	Gtk::Box * page1 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2));
	Gtk::Box * page1hbox1 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));	
	Gtk::Box * page1hbox2 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,1));

	Gtk::AspectFrame * page1hbox1space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox2space1 = manage(new Gtk::AspectFrame());

	Gtk::Label * attributeNameTitle = manage(new Gtk::Label("Node element :"));
	addComboBoxText = manage (new Gtk::ComboBoxText(false));
	addComboBoxText->append("Select an element");
	addComboBoxText->set_active(0);
	unsigned int cpt = 0;
	for (std::list<EBUCoreFeatures::ElementStruct>::iterator it=ebucoreRef.children.begin() ; it != ebucoreRef.children.end(); ++it) 
	{
		EBUCoreFeatures::ElementStruct elem = *it;
		if (cardinalityAllows(elem.name, elem.maxCardinality, metadataStore)) {
			addComboBoxText->append(elem.name);
			cpt++;
		}
	}
	
	page1hbox1->add(*attributeNameTitle);
	page1hbox1->add(*page1hbox1space1);
	page1hbox1->add(*addComboBoxText);
	page1hbox1->show_all_children();
	page1hbox1->show();

	page1hbox2->add(*page1hbox2space1);
	page1hbox2->show_all_children();
	page1hbox2->show();

	page1->add(*page1hbox1);
	page1->add(*page1hbox2);
	page1->show();


	metadataAssistant->append_page(*page1);
	metadataAssistant->set_page_complete(*page1, ((cpt == 0)?false:true));	
	metadataAssistant->set_page_type(*page1, Gtk::ASSISTANT_PAGE_CONFIRM);


	metadataAssistant->signal_apply().connect
	(
		sigc::bind<Glib::RefPtr<Gtk::ListStore> >
		(
			sigc::mem_fun
			(
				*this,
				&metadataEngine::on_addNode_assistant_apply
				
			),
			metadataStore
		)
	);
	metadataAssistant->signal_cancel().connect
	(
		sigc::mem_fun
		(
			*this,
			&metadataEngine::on_assistant_cancel
		)
	);
	metadataAssistant->signal_close().connect
	(
		sigc::mem_fun
		(
			*this,
			&metadataEngine::on_assistant_close
		)
	);

	metadataAssistant->show();
}

void metadataEngine::removeNode
(
	Glib::RefPtr<Gtk::ListStore> metadataStore
)
{
	std::cout<<"I'm removing a node"<<std::endl;
}

void metadataEngine::on_addAttribute_assistant_apply
(
	Glib::RefPtr<Gtk::ListStore> metadataStore
)
{
	unsigned int nb = metadataStore->children().size();
	bool add = true;
	for (unsigned int i = 0; i<nb; i++)
	{
		//Gtk::TreeModel::Row row = *(metadataNodeChildrenStore->prepend());
		if (metadataStore->children()[i][metadataNodeAttributesColumns.metadataNodeAttributeNameCol] == addComboBoxText->get_active_text()) {
			metadataStore->children()[i][metadataNodeAttributesColumns.metadataNodeAttributeValueCol] = attributeValueEntry->get_value();
			metadataStore->children()[i][metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol] = "lightgreen";
			add=false;
			attributeList.at((2*(nb-i-1))+1) = attributeValueEntry->get_value();
			break;
		}
	}
	if (add) {
		Gtk::TreeModel::Row row = *(metadataStore->prepend());
		row[metadataNodeAttributesColumns.metadataNodeAttributeIdCol] = nb;
		row[metadataNodeAttributesColumns.metadataNodeAttributeNameCol] = addComboBoxText->get_active_text();
		row[metadataNodeAttributesColumns.metadataNodeAttributeValueCol] = attributeValueEntry->get_value();
		row[metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol] = "lightgreen";
		attributeList.push_back(addComboBoxText->get_active_text());
		attributeList.push_back(attributeValueEntry->get_value());
	}	
	elReferences.at(previousnodepos)->setAttribute
	(
		xercesc::XMLString::transcode
		(
			addComboBoxText->get_active_text().c_str()
		), 
		xercesc::XMLString::transcode
		(
			(attributeValueEntry->get_value()).c_str()
		)
	);
}

void metadataEngine::on_assistant_cancel
(
	void
)
{
	metadataAssistant->hide();
	metadataAssistant->remove();
	delete metadataAssistant;
}

void metadataEngine::on_assistant_close
(
	void
)
{
	metadataAssistant->hide();
	metadataAssistant->remove();
	delete metadataAssistant;
}

void metadataEngine::on_addNodeText_assistant
(
	Gtk::Box * text, 
	Gtk::Box * children
)
{
	std::cout<< "I'm trying to change the second assistant page"<<std::endl;
	std::cout<< "I have "<<metadataAssistant->get_n_pages()<<" pages"<<std::endl;
	std::cout<< "I'm the page number "<<metadataAssistant->get_current_page()<<std::endl;
	metadataAssistant->set_title((addNode_type)?"add a node text": "add a node children");
	addNode_type = !addNode_type;
}

void metadataEngine::on_addNode_assistant_apply
(
	Glib::RefPtr<Gtk::ListStore> metadataStore
)
{
	unsigned int nb = metadataStore->children().size();
	Gtk::TreeModel::Row row = *(metadataStore->prepend());
	row[metadataNodeChildrenColumns.metadataNodeChildrenIdCol] = nb;
	row[metadataNodeChildrenColumns.metadataNodeChildrenNameCol] = addComboBoxText->get_active_text();
	row[metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol] = "lightgreen";
	std::string tmp = addComboBoxText->get_active_text();
	
	bool istext = false;
	for (std::list<EBUCoreFeatures::ElementStruct>::iterator it=ebucoreRef.children.begin() ; it != ebucoreRef.children.end(); ++it) 
	{
		EBUCoreFeatures::ElementStruct elem = *it;
		if (elem.name == addComboBoxText->get_active_text())
		{
			if (elem.children.size() > 0)
			{
				istext = false;
			} else {
				istext = true;
			}
			break;
		}		
	}

	if (istext) {
		xercesc::DOMElement*  elm = xercescdoc->createElement(xercesc::XMLString::transcode(tmp.c_str()));	
		xercesc::DOMText* el = xercescdoc->createTextNode(xercesc::XMLString::transcode(""));
		elm->appendChild(el);
		elReferences.at(previousnodepos)->appendChild(elm);
	} else {
		xercesc::DOMElement*  el = xercescdoc->createElement(xercesc::XMLString::transcode(tmp.c_str()));		
		elReferences.at(previousnodepos)->appendChild(el);
	}
	reconstructTreeView();
	std::cout<< "I pressed apply button"<< std::endl;
}
