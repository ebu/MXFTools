/*!
 * \project_name EBU Player
 * \file metadataWindow.cpp
 * \brief EBUCore Metadata window
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 20 august 2012
 * \dateOfUpdate 21 march 2012
 * \copyright This software is published under MPL v2.0
 *
 */

// include files and libraries
#include "metadataWindow.hpp"

// class constructor
metadataWindow::metadataWindow
(
	BaseObjectType* cobject, 
	const Glib::RefPtr<Gtk::Builder>& refGlade
)
: 
	Gtk::Window(cobject),
	m_refGlade(refGlade)
{
	refGladeButton(refGlade); // " " " " buttons
	refGladeWidgets(refGlade);// " " " " widgets
	initMetadataMenuPopup();
	initMetadataWindow();
	initRightSide();
	connectSignals(); // clicked buttons
	engine = new metadataEngine
	(
		status,
		source, 
		sourceLabel,
		size, 
		sizeValue,
		lineOne, 
		lineTwo,
		FirstScrolledWindowBox,
		SecondScrolledWindowBox,
		boxEntries,
		boxStatus,
		expanderRoot,
		viewportTree,
		switchEBUCoreEdition
	);
	set_has_resize_grip();
}

// class destructor
metadataWindow::~metadataWindow
(
	void
)
{
	// remove the previous temporary xml file to
	// avoid to reload some previous metadata
	// std::remove((Glib::get_current_dir ()+"/mxftmpmetadata.xml").c_str());
}

void metadataWindow::refGladeButton
(
	const Glib::RefPtr<Gtk::Builder>& refGlade
) 
{
	/* metadata control buttons */
	refGlade->get_widget
	(
		"importEBUCore", 
		importEBUCore
	);	
	refGlade->get_widget
	(
		"exportEBUCore", 
		exportEBUCore
	);
	refGlade->get_widget
	(
		"removeEBUCore", 
		removeEBUCore
	);
	refGlade->get_widget
	(
		"switchEBUCoreEdition", 
		switchEBUCoreEdition
	);	
}

void metadataWindow::refGladeWidgets
(
	const Glib::RefPtr<Gtk::Builder>& refGlade
) 
{
	/* instantiate widgets for metadata window*/
	refGlade->get_widget
	(
		"expanderRoot",
		expanderRoot
	);
	refGlade->get_widget
	(
		"viewportTree",
		viewportTree
	);
	refGlade->get_widget
	(
		"FirstScrolledWindowBox",
		FirstScrolledWindowBox
	);
	refGlade->get_widget
	(
		"SecondScrolledWindowBox",
		SecondScrolledWindowBox
	);
	refGlade->get_widget
	(
		"boxEntries",
		boxEntries
	);
	refGlade->get_widget
	(
		"boxStatus",
		boxStatus
	);
}

void metadataWindow::connectSignals
(
	void
)
{
}

void metadataWindow::on_importEBUCore_clicked
(
	bool EBUCoreKind,
	bool isRemote
)
{
	if (isRemote)
	{
		// instanciate a new remote filechooser
		remoteStreamFileChooser * RFC = new remoteStreamFileChooser
			(
				*this,
				"Remote media filechooser...",
				"Type the path to the remote stream :",
				false
			);
	
		if (RFC->getResponse() == Gtk::RESPONSE_OK) 
		{
			std::cout<<"I'm opening the EBUCore file :"<<std::endl<<RFC->getURL()<<std::endl;
		}
		delete RFC;
	} 
	else
	{
		// instanciate a new local filechooser
		genericFilechooserWindow * FC = new genericFilechooserWindow
		(
			*this,
			"Select an EBUCore file",
			Gtk::FILE_CHOOSER_ACTION_OPEN,
			Glib::get_home_dir(),
			false,
			((EBUCoreKind) ? MXFMETADATAMIMETYPE : XMLMETADATAMIMETYPE)
		);
	
		if (FC->getResponse() == Gtk::RESPONSE_OK) 
		{
			// construct the treeview
			if (genericFeatures::isExtension(FC->getPathToFile(),"xml"))
			{
				engine->constructTreeViewFromXML(FC->getPathToFile());
			} else {
				engine->constructTreeViewFromMXF(FC->getPathToFile());
			}
			// set the information about the file into the right side panel
			engine->setRightSide(FC->getPathToFile());
			// set the new size
			set_size_request(engine->getViewportSize()+410,-1);
			// force to reshow window with the new viewport size
			reshow_with_initial_size();
		}
		delete FC;
	}
}
void metadataWindow::on_removeEBUCore_clicked
(
	void
)
{
	// instanciate a new local filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select an MXF file",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		MXFMETADATAMIMETYPE
	);

	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the information about the file into the right side panel
		engine->removeEBUCore(FC->getPathToFile());
	}
	delete FC;
}

void metadataWindow::on_saveAndRemoveEBUCore_clicked
(
	void
)
{
	// instanciate a new local filechooser
	genericFilechooserInputOutput * FCIO = new genericFilechooserInputOutput
	(
		*this,
		"Convert MXF EBUCore into XML EBUCore",
		"Select the input MXF file :",
		"Save the output XML EBUCore file into :",
		"Extract from ...",
		"Save as ...",
		"Set the input path to the MXF EBUCore file...",
		"Set the output path to the XML EBUCore file...",
		true
	);
	
	if (FCIO->getResponse() == Gtk::RESPONSE_OK) 
	{
		engine->removeEBUCore(FCIO->getInputPath(),FCIO->getOutputPath());
	}
	
	delete FCIO;
}

void metadataWindow::on_embedXML2MXFEBUCore_clicked
(
	void
)
{
	metadataEncapsulationInputOutputDialog * MEIOD = new metadataEncapsulationInputOutputDialog (*this,"Encapsulate EBUCore metadata as ...");
	
	if (MEIOD->getResponse() == Gtk::RESPONSE_OK)  {
		std::cout<<"Output MXF : "<<MEIOD->getOutputPath()<<std::endl;
		std::cout<<"Input XML  : "<<MEIOD->getInputPath()<<std::endl;
		std::cout<<"Force to set XML into MXF header partition : "<<MEIOD->getHeader()<<std::endl;
		std::cout<<"Set metadata as dark partition : "<<MEIOD->getDark()<<std::endl;
		std::cout<<"Set metadata as sidecar reference : "<<MEIOD->getSidecar()<<std::endl;
		engine->embedEBUCore
		(
			MEIOD->getOutputPath(),
			MEIOD->getInputPath(),
			MEIOD->getHeader(),
			MEIOD->getDark(),
			MEIOD->getSidecar()
		);
	}
	// free memory
	delete MEIOD;
}

void metadataWindow::on_embedEBUCore_clicked
(
	void
)
{
	metadataEncapsulationDialog * MetadataExportDialog = new metadataEncapsulationDialog 
	(
		*this, 
		"Embed current EBUCore metadatas into current MXF file",
		"",
		"",
		"",
		"",
		"",
		"",
		false,
		false,
		true,
		true
	);
	
	if (MetadataExportDialog->getResponse() == Gtk::RESPONSE_OK)  {
		//engine->embedEBUCore();
	}
	// free memory
	delete MetadataExportDialog;
}

void metadataWindow::on_exportMXF2XMLEBUCore_clicked
(
	void
)
{
	// instanciate a new local filechooser
	genericFilechooserInputOutput * FCIO = new genericFilechooserInputOutput
	(
		*this,
		"Convert MXF EBUCore into XML EBUCore",
		"Select the input MXF file :",
		"Save the output XML EBUCore file into :",
		"Extract from ...",
		"Save as ...",
		"Set the input path to the MXF EBUCore file...",
		"Set the output path to the XML EBUCore file...",
		true
	);
	
	if (FCIO->getResponse() == Gtk::RESPONSE_OK) 
	{
		engine->exportMXF2XMLEBUCore(FCIO->getInputPath(),FCIO->getOutputPath());
	}
	
	delete FCIO;
}

void metadataWindow::on_exportEBUCore_clicked
(
	void
)
{
	metadataEncapsulationDialog * MetadataExportDialog = new metadataEncapsulationDialog 
	(
		*this, 
		"Embed current EBUCore metadatas into an XML file",
		"",
		"Set the output XML file :",
		"",
		"Save as",
		"",
		"Set the output XML filename...",
		false,
		true,
		false,
		true
	);
	if (MetadataExportDialog->getResponse() == Gtk::RESPONSE_OK)  {
		engine->exportXMLEBUCore(MetadataExportDialog->getOutputPath());
	}
	// free memory
	delete MetadataExportDialog;
}

void metadataWindow::initMetadataMenuPopup
(
	void
)
{
	// Create action groups for the popup menus which has to 
	// be included into the metadata action buttons 
	importEBUCoreActionGroup = Gtk::ActionGroup::create();
	exportEBUCoreActionGroup = Gtk::ActionGroup::create();
	removeEBUCoreActionGroup = Gtk::ActionGroup::create();
	//Add a new item action menu to the action group
	importEBUCoreActionGroup->add(Gtk::Action::create("importEBUCoreMenu", "Import an EBUCore File ..."));
	exportEBUCoreActionGroup->add(Gtk::Action::create("exportEBUCoreMenu", "Save / Export ..."));
	exportEBUCoreActionGroup->add(Gtk::Action::create("removeEBUCoreMenu", "emove EBUCore metadatas from MXF file..."));
	// add new menu items with their own action menu, a keyboard shortcut 
	// and a listener ; open a new playlist or append a playlist to the 
	// current playlist
	////////////////////////////////////////////
	
	//import menu xml and mxf ebucore
	importEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"importEBUCoreXMLMenu", 
			Gtk::Stock::NEW, 
			"XML EBUCore", 
			"Open an XML EBUCore instance file..."
		)
	);
	importEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"importEBUCoreMXFMenu", 
			Gtk::Stock::NEW, 
			"MXF EBUCore", 
			"Open an MXF EBUCore instance file"
		)
	);
	// import submenu for xml
	importEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"importEBUCoreXMLLocal", 
			Gtk::Stock::NEW, 
			"local file", 
			"Open a local XML EBUCore instance file..."
		), 
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataWindow::on_importEBUCore_clicked
			), 
			false, 
			false
		)
	);
	importEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"importEBUCoreXMLRemote", 
			Gtk::Stock::NEW, 
			"remote file", 
			"Open a remote XML EBUCore instance file..."
		), 
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataWindow::on_importEBUCore_clicked
			), 
			false, 
			true
		)
	);
	// import submenu for mxf
	importEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"importEBUCoreMXFLocal", 
			Gtk::Stock::NEW, 
			"local file", 
			"Open a local MXF EBUCore instance file..."
		), 
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataWindow::on_importEBUCore_clicked
			), 
			true, 
			false
		)
	);
	importEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"importEBUCoreMXFRemote", 
			Gtk::Stock::NEW, 
			"remote file", 
			"Open a remote MXF EBUCore instance file..."
		), 
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&metadataWindow::on_importEBUCore_clicked
			), 
			true, 
			true
		)
	);
	
	
	////////////////////////////////////////
	exportEBUCoreActionGroup->add
	(
		
		Gtk::Action::create
		(
			"exportEBUCoreMXF2XMLMenu", 
			Gtk::Stock::ADD, 
			"MXF EBUCore to XML", 
			"Export MXF EBUCore to XML EBUCore..."
		)
	);
	
	// export submenu for local xml from local mxf
	exportEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"exportEBUCoreMXF2XMLLocal", 
			Gtk::Stock::NEW, 
			"Local file", 
			"Save the MXF EBUCore into an XML local file..."
		), 
		sigc::mem_fun
		(
			*this, 
			&metadataWindow::on_exportMXF2XMLEBUCore_clicked
		)
	);
	exportEBUCoreActionGroup->add
	(
		
		Gtk::Action::create
		(
			"exportEBUCoreXML2MXFMenu", 
			Gtk::Stock::ADD, 
			"XML EBUCore to MXF", 
			"Export XML EBUCore to MXF EBUCore..."
		)
	);
	
	// export submenu for local mxf from local yml
	exportEBUCoreActionGroup->add
	(
		Gtk::Action::create
		(
			"exportEBUCoreXML2MXFLocal", 
			Gtk::Stock::NEW, 
			"Local file", 
			"Embed an XML EBUCore into an MXF local file..."
		), 
		sigc::mem_fun
		(
			*this, 
			&metadataWindow::on_embedXML2MXFEBUCore_clicked
		)
	);
	exportEBUCoreActionGroup->add
	(
		
		Gtk::Action::create
		(
			"exportEBUCoreMXFFile", 
			Gtk::Stock::ADD, 
			"Save into MXF", 
			"Save current EBUCore metadata into EBUCore MXF file..."
		),
		sigc::mem_fun
		(
			*this, 
			&metadataWindow::on_embedEBUCore_clicked
		)
	);
	exportEBUCoreActionGroup->add
	(
		
		Gtk::Action::create
		(
			"exportEBUCoreXMLFile", 
			Gtk::Stock::ADD, 
			"Save into XML", 
			"Save current EBUCore metadata into EBUCore XML file..."
		),
		sigc::mem_fun
		(
			*this, 
			&metadataWindow::on_exportEBUCore_clicked
		)
	);
	
	/////////////////////////

	removeEBUCoreActionGroup->add
	(
		
		Gtk::Action::create
		(
			"removeEBUCoreFromMXF", 
			Gtk::Stock::ADD, 
			"Remove EBUCore metadatas...", 
			"Remove EBUCore metadatas from MXF"
		),
		sigc::mem_fun
		(
			*this, 
			&metadataWindow::on_removeEBUCore_clicked
		)
	);

	removeEBUCoreActionGroup->add
	(
		
		Gtk::Action::create
		(
			"saveAndRemoveEBUCoreFromMXF", 
			Gtk::Stock::ADD, 
			"Save and then remove...", 
			"Save and then remove EBUCore metadatas from MXF"
		),
		sigc::mem_fun
		(
			*this, 
			&metadataWindow::on_saveAndRemoveEBUCore_clicked
		)
	);

	importEBUCoreUIManager = Gtk::UIManager::create();
	exportEBUCoreUIManager = Gtk::UIManager::create();
	removeEBUCoreUIManager = Gtk::UIManager::create();
	importEBUCoreUIManager->insert_action_group(importEBUCoreActionGroup);
	exportEBUCoreUIManager->insert_action_group(exportEBUCoreActionGroup);
	removeEBUCoreUIManager->insert_action_group(removeEBUCoreActionGroup);

	add_accel_group(importEBUCoreUIManager->get_accel_group());
	add_accel_group(exportEBUCoreUIManager->get_accel_group());
	add_accel_group(removeEBUCoreUIManager->get_accel_group());

	//Layout the actions in a menubar and toolbar:
	Glib::ustring ui_info =
		"<ui>"
		"  <popup name='importEBUCorePopupMenu'>"
		"    <menu action='importEBUCoreXMLMenu'>"
		"      <menuitem action='importEBUCoreXMLLocal'/>"
		"      <menuitem action='importEBUCoreXMLRemote'/>"
		"    </menu>"
		"    <menu action='importEBUCoreMXFMenu'>"
		"      <menuitem action='importEBUCoreMXFLocal'/>"
		"      <menuitem action='importEBUCoreMXFRemote'/>"
		"    </menu>"
		"  </popup>"
		"</ui>";

	try
	{
		importEBUCoreUIManager->add_ui_from_string(ui_info);
	} catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}
	importEBUCoreMenuPopup = dynamic_cast<Gtk::Menu*>
	(
		importEBUCoreUIManager->get_widget("/importEBUCorePopupMenu")
	); 
	if(!importEBUCoreMenuPopup)
		g_warning("menu not found");

	//Layout the actions in a menubar and toolbar:
	ui_info =
		"<ui>"
		"  <popup name='exportEBUCorePopupMenu'>"
		"    <menuitem action='exportEBUCoreMXFFile'/>"
		"    <menuitem action='exportEBUCoreXMLFile'/>"
        "      <separator/>"
		"    <menu action='exportEBUCoreMXF2XMLMenu'>"
		"      <menuitem action='exportEBUCoreMXF2XMLLocal'/>"
		"    </menu>"
		"    <menu action='exportEBUCoreXML2MXFMenu'>"
		"      <menuitem action='exportEBUCoreXML2MXFLocal'/>"
		"    </menu>"
		"  </popup>"
		"</ui>";

	try
	{
		exportEBUCoreUIManager->add_ui_from_string(ui_info);
	} catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}
	exportEBUCoreMenuPopup = dynamic_cast<Gtk::Menu*>
	(
		exportEBUCoreUIManager->get_widget("/exportEBUCorePopupMenu")
	); 
	if(!exportEBUCoreMenuPopup)
		g_warning("menu not found");
		

	//Layout the actions in a menubar and toolbar:
	ui_info =
		"<ui>"
		"  <popup name='removeEBUCorePopupMenu'>"
		"    <menuitem action='removeEBUCoreFromMXF'/>"
		"    <menuitem action='saveAndRemoveEBUCoreFromMXF'/>"
		"  </popup>"
		"</ui>";

	try
	{
		removeEBUCoreUIManager->add_ui_from_string(ui_info);
	} catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}
	removeEBUCoreMenuPopup = dynamic_cast<Gtk::Menu*>
	(
		removeEBUCoreUIManager->get_widget("/removeEBUCorePopupMenu")
	); 
	if(!removeEBUCoreMenuPopup)
		g_warning("menu not found");
		
	// set the popup
	importEBUCore->set_popup(*importEBUCoreMenuPopup);
	exportEBUCore->set_popup(*exportEBUCoreMenuPopup);
	removeEBUCore->set_popup(*removeEBUCoreMenuPopup);

}

void metadataWindow::initMetadataWindow
(
	void
)
{
	// menubutton to open an EBUCore file
	importEBUCore->set_visible(true);
	importEBUCore->set_sensitive(true);
	// menubutton to save/export an EBUCore file
	exportEBUCore->set_visible(true);
	exportEBUCore->set_sensitive(true);
	// menubutton to save/export an EBUCore file
	removeEBUCore->set_visible(true);
	removeEBUCore->set_sensitive(true);
	// button to switch into edition mode
	switchEBUCoreEdition->set_visible(true);
	switchEBUCoreEdition->set_sensitive(false);
	switchEBUCoreEdition->set_active(false);
	// Initial window state
	set_visible(false);
}

void metadataWindow::initRightSide
(
	void
)
{
	//boxStatus = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
 	//boxStatus->show();
	status = manage(new Gtk::Label("No EBUCore metadata detected"));
 	lineOne = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
 	source = manage(new Gtk::Label("source : "));
 	sourceLabel = manage(new Gtk::Label("no source"));
	lineTwo = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
 	size = manage(new Gtk::Label("size : "));
 	sizeValue = manage(new Gtk::Label("0 byte"));
	status->show();
	source->show();
	sourceLabel->show();
	size->show();
	sizeValue->show();
	lineOne->show();
	lineTwo->show();
	lineOne->add(*source);
	lineOne->add(*sourceLabel);
	lineTwo->add(*size);
	lineTwo->add(*sizeValue);
	boxStatus->add(*status);
	boxStatus->add(*lineOne);
	boxStatus->add(*lineTwo);
	boxStatus->show();
	FirstScrolledWindowBox->hide();
	boxEntries->hide();
}
