/*!
 * \project_name EBU Player
 * \file genericFilechooserWindow.cpp
 * \brief EBUCore Metadata encapsulation dialog
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 28 march 2013
 * \dateOfUpdate 28 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */

// include files and libraries
#include "genericFilechooserInputOutput.hpp"

// class constructor
genericFilechooserInputOutput::genericFilechooserInputOutput
(
	Gtk::Window& parent, 
	const Glib::ustring& title,
	const Glib::ustring& inputLabel,
	const Glib::ustring& outputLabel,
	const Glib::ustring& inputButtonLabel,
	const Glib::ustring& outputButtonLabel,
	const Glib::ustring& inputPlaceHolderLabel,
	const Glib::ustring& outputPlaceHolderLabel,
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
    // pack elements into the dialog box
    dialogOptionBox->pack_start(*inputPathLabel);
    dialogOptionBox->pack_start(*pathInputBox);
    dialogOptionBox->pack_start(*outputPathLabel);
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
genericFilechooserInputOutput::~genericFilechooserInputOutput
(
	void
)
{
}

void genericFilechooserInputOutput::connectSignalClicked
(
	void
)
{
	pathInputButton->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&genericFilechooserInputOutput::on_pathInputButton_clicked
		)
	);
	pathOutputButton->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&genericFilechooserInputOutput::on_pathOutputButton_clicked
		)
	);
}

void genericFilechooserInputOutput::on_close_dialog
(
	void
)
{
	hide();
}

void genericFilechooserInputOutput::on_terminate_clicked
(
	void
)
{
	hide();
}

void genericFilechooserInputOutput::on_pathInputButton_clicked
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
		// set the information about the path
		pathInputEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}

void genericFilechooserInputOutput::on_pathOutputButton_clicked
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
		XMLEBUCOREMETADATAMIMETYPE
	);

	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the information about the path
		pathOutputEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}


int genericFilechooserInputOutput::getResponse
(
	void
)
{
	return response;
}

std::string genericFilechooserInputOutput::getInputPath
(
	void
)
{
	return pathInputEntry->get_text();
}


std::string genericFilechooserInputOutput::getOutputPath
(
	void
)
{
	return pathOutputEntry->get_text();
}


