/*!
 * \project_name EBU Player
 * \file metadataEncapsulationDialog.cpp
 * \brief EBUCore Metadata encapsulation dialog
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 28 march 2013
 * \dateOfUpdate 28 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */

// include files and libraries
#include "metadataEncapsulationDialog.hpp"

// class constructor
metadataEncapsulationDialog::metadataEncapsulationDialog
(
	Gtk::Window& parent,  
	const Glib::ustring& title,
	const Glib::ustring& inputLabel,
	const Glib::ustring& outputLabel,
	const Glib::ustring& inputButtonLabel,
	const Glib::ustring& outputButtonLabel,
	const Glib::ustring& inputPlaceHolderLabel,
	const Glib::ustring& outputPlaceHolderLabel,
	bool hasInputFile,
	bool hasOutputFile,
	bool hasEBUCoreOptions,
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
	Gtk::Label * inputPathLabel = Gtk::manage(new Gtk::Label(inputLabel));
	Gtk::Label * outputPathLabel = Gtk::manage(new Gtk::Label(outputLabel));
	// define box
	Gtk::Box * pathInputBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2));
	Gtk::Box * pathOutputBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2));
	// define buttons
	pathInputButton = Gtk::manage(new Gtk::Button(inputButtonLabel));
	pathOutputButton = Gtk::manage(new Gtk::Button(outputButtonLabel));
		// define entry
	pathInputEntry = Gtk::manage(new Gtk::Entry());
	pathInputEntry->set_editable(true);
	pathInputEntry->set_placeholder_text(inputPlaceHolderLabel);
	pathOutputEntry = Gtk::manage(new Gtk::Entry());
	pathOutputEntry->set_editable(true);
	pathOutputEntry->set_placeholder_text(outputPlaceHolderLabel);
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
    if (hasInputFile) 
    {
		dialogOptionBox->pack_start(*inputPathLabel);
		dialogOptionBox->pack_start(*pathInputBox);
    }
    if (hasEBUCoreOptions) 
    {
        dialogOptionBox->pack_start(*partitionLabel);
		dialogOptionBox->pack_start(*partitionButtonBox);
		dialogOptionBox->pack_start(*encodingLabel);
		dialogOptionBox->pack_start(*encodingButtonBox);
    }
    if (hasOutputFile) 
    {
		dialogOptionBox->pack_start(*outputPathLabel);
		dialogOptionBox->pack_start(*pathOutputBox);
    }
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
metadataEncapsulationDialog::~metadataEncapsulationDialog
(
	void
)
{
}

void metadataEncapsulationDialog::connectSignalClicked
(
	void
)
{
	pathInputButton->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationDialog::on_pathInputButton_clicked
		)
	);
	pathOutputButton->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationDialog::on_pathOutputButton_clicked
		)
	);
	// connect radio groups
	partitionHeader->signal_group_changed().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationDialog::on_partition_group_changed
		)
	);
	encodingKLV->signal_group_changed().connect
	(
		sigc::mem_fun
		(
			*this, 
			&metadataEncapsulationDialog::on_encoding_group_changed
		)
	);
}

void metadataEncapsulationDialog::on_close_dialog
(
	void
)
{
	hide();
}

void metadataEncapsulationDialog::on_terminate_clicked
(
	void
)
{
	hide();
}

void metadataEncapsulationDialog::on_pathInputButton_clicked
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
		4
	);

	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the information about the path
		pathInputEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}

void metadataEncapsulationDialog::on_pathOutputButton_clicked
(
	void
)
{
	// instanciate a new local filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Save into an XML file...",
		Gtk::FILE_CHOOSER_ACTION_SAVE,
		Glib::get_home_dir(),
		false,
		3
	);

	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the information about the path
		pathOutputEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}

void metadataEncapsulationDialog::on_partition_group_changed
(
	void
)
{
}

void metadataEncapsulationDialog::on_encoding_group_changed
(
	void
)
{
}

int metadataEncapsulationDialog::getResponse
(
	void
)
{
	return response;
}

std::string metadataEncapsulationDialog::getInputPath
(
	void
)
{
	return pathInputEntry->get_text();
}

std::string metadataEncapsulationDialog::getOutputPath
(
	void
)
{
	return pathOutputEntry->get_text();
}

bool metadataEncapsulationDialog::getKLV
(
	void
)
{
	return encodingKLV->get_active();
}

bool metadataEncapsulationDialog::getDark
(
	void
)
{
	return encodingDark->get_active();
}

bool metadataEncapsulationDialog::getSidecar
(
	void
)
{
	return encodingSide->get_active();
}

bool metadataEncapsulationDialog::getHeader
(
	void
)
{
	return partitionHeader->get_active();
}



bool metadataEncapsulationDialog::getFooter
(
	void
)
{
	return partitionFooter->get_active();
}

