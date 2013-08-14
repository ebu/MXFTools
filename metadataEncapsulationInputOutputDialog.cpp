/*!
 * \project_name EBU Player
 * \file metadataEncapsulationInputOutputDialog.cpp
 * \brief EBUCore Metadata encapsulation dialog
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 28 march 2013
 * \dateOfUpdate 28 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */

// include files and libraries
#include "metadataEncapsulationInputOutputDialog.hpp"

// class constructor
metadataEncapsulationInputOutputDialog::metadataEncapsulationInputOutputDialog
(
	Gtk::Window& parent, 
	const Glib::ustring& title,
	bool setModal
)
: 
	Gtk::Dialog(title, parent, setModal),
	dialogOptionBox(get_vbox ())
{
	// dialog configuration
	set_resizable(true);
	set_position(Gtk::WIN_POS_CENTER);
	set_decorated(true);
	// define labels
	Gtk::Label * partitionLabel = Gtk::manage(new Gtk::Label("Set the metadata into :"));
	Gtk::Label * encodingLabel = Gtk::manage(new Gtk::Label("Encoded as :"));
	Gtk::Label * inputLabel = Gtk::manage(new Gtk::Label("In the file :"));
	Gtk::Label * outputLabel = Gtk::manage(new Gtk::Label("Select the input XML EBUCore file :"));;
	// define box
	Gtk::Box * pathInputBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2));
	Gtk::Box * pathOutputBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2));
	// define buttons
	pathInputButton = Gtk::manage(new Gtk::Button("Select"));
	pathOutputButton = Gtk::manage(new Gtk::Button("Select"));
	// define entry
	// define entry
	pathInputEntry = Gtk::manage(new Gtk::Entry());
	pathInputEntry->set_editable(true);
	pathInputEntry->set_placeholder_text("Select the input XML file...");
	pathOutputEntry = Gtk::manage(new Gtk::Entry());
	pathOutputEntry->set_editable(true);
	pathOutputEntry->set_placeholder_text("Select the destination MXF file...");
	// pack the entry and its button
	pathInputBox->pack_start(*pathInputEntry);
	pathInputBox->pack_start(*pathInputButton);
	pathOutputBox->pack_start(*pathOutputEntry);
	pathOutputBox->pack_start(*pathOutputButton);
	// define buttonbox
	Gtk::ButtonBox * partitionButtonBox = Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL));
	Gtk::ButtonBox * encodingButtonBox = Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_VERTICAL));
	// define radio buttons
	Gtk::RadioButton::Group partitionGroup;
    partitionHeader = Gtk::manage(new Gtk::RadioButton(partitionGroup,"header partition"));
    partitionFooter = Gtk::manage(new Gtk::RadioButton(partitionGroup,"footer partition"));
	Gtk::RadioButton::Group encodingGroup;
    encodingKLV = Gtk::manage(new Gtk::RadioButton(encodingGroup,"KLV"));
    encodingDark = Gtk::manage(new Gtk::RadioButton(encodingGroup,"XML"));
    encodingSide = Gtk::manage(new Gtk::RadioButton(encodingGroup,"External link"));
    // add radio buttons into their own radio box
    partitionButtonBox->pack_start(*partitionHeader);
    partitionButtonBox->pack_start(*partitionFooter);
    encodingButtonBox->pack_start(*encodingKLV);
    encodingButtonBox->pack_start(*encodingDark);
    encodingButtonBox->pack_start(*encodingSide);
    // pack elements into the dialog box
    dialogOptionBox->pack_start(*inputLabel);
    dialogOptionBox->pack_start(*pathInputBox);
    dialogOptionBox->pack_start(*partitionLabel);
    dialogOptionBox->pack_start(*partitionButtonBox);
    dialogOptionBox->pack_start(*encodingLabel);
    dialogOptionBox->pack_start(*encodingButtonBox);
    dialogOptionBox->pack_start(*outputLabel);
    dialogOptionBox->pack_start(*pathOutputBox);
	// append filechooser buttons and linked events
	add_button
	(
		Gtk::Stock::CANCEL, 
		Gtk::RESPONSE_CANCEL
	);
	add_button
	(
		Gtk::Stock::OK, 
		Gtk::RESPONSE_OK
	);
	connectSignalClicked();
	show_all_children();
//	
	// run the filechooser and grab the result
	response = run();
}

// class destructor
metadataEncapsulationInputOutputDialog::~metadataEncapsulationInputOutputDialog
(
	void
)
{
	std::cout<<"byebye"<<std::endl;
}

void metadataEncapsulationInputOutputDialog::connectSignalClicked
(
	void
)
{
	pathInputButton->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationInputOutputDialog::on_pathInputButton_clicked
		)
	);
	pathOutputButton->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationInputOutputDialog::on_pathOutputButton_clicked
		)
	);
	// connect radio groups
	partitionHeader->signal_group_changed().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationInputOutputDialog::on_partition_group_changed
		)
	);
	encodingKLV->signal_group_changed().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationInputOutputDialog::on_encoding_group_changed
		)
	);
}

void metadataEncapsulationInputOutputDialog::on_close_dialog
(
	void
)
{
	hide();
}

void metadataEncapsulationInputOutputDialog::on_saveMetadata_clicked
(
	void
)
{
	hide();
}

void metadataEncapsulationInputOutputDialog::on_pathInputButton_clicked
(
	void
)
{
	// instanciate a new local filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select an XML file",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		3
	);

	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the information about the path
		pathInputEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}


void metadataEncapsulationInputOutputDialog::on_pathOutputButton_clicked
(
	void
)
{
	// instanciate a new local filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Save into the MXF file...",
		Gtk::FILE_CHOOSER_ACTION_SAVE,
		Glib::get_home_dir(),
		false,
		MXFMETADATAMIMETYPE
	);

	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the information about the path
		pathOutputEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}

void metadataEncapsulationInputOutputDialog::on_partition_group_changed
(
	void
)
{
}

void metadataEncapsulationInputOutputDialog::on_encoding_group_changed
(
	void
)
{
}

int metadataEncapsulationInputOutputDialog::getResponse
(
	void
)
{
	return response;
}

std::string metadataEncapsulationInputOutputDialog::getInputPath
(
	void
)
{
	return pathInputEntry->get_text();
}

std::string metadataEncapsulationInputOutputDialog::getOutputPath
(
	void
)
{
	return pathOutputEntry->get_text();
}

bool metadataEncapsulationInputOutputDialog::getKLV
(
	void
)
{
	return encodingKLV->get_active();
}

bool metadataEncapsulationInputOutputDialog::getDark
(
	void
)
{
	return encodingDark->get_active();
}

bool metadataEncapsulationInputOutputDialog::getSidecar
(
	void
)
{
	return encodingSide->get_active();
}

bool metadataEncapsulationInputOutputDialog::getHeader
(
	void
)
{
	return partitionHeader->get_active();
}



bool metadataEncapsulationInputOutputDialog::getFooter
(
	void
)
{
	return partitionFooter->get_active();
}
