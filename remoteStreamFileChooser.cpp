/*!
 * \project_name EBU Player
 * \file remoteStreamFileChooser.cpp
 * \brief remote stream window function
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 11 march 2013
 *
 * This software is published in LGPLv3.0
 *
 */

#include "remoteStreamFileChooser.hpp"

remoteStreamFileChooser::remoteStreamFileChooser
(
 	Gtk::Window& windowParent,
	std::string windowTitle,
	std::string messageDialog,
	bool modal
)
: 
	Gtk::Dialog(windowTitle, modal),
	remoteStreamBox(get_content_area()),
	remoteStreamLabel(messageDialog)
{
	remoteStreamBox->pack_start(remoteStreamLabel);
    remoteStreamBox->pack_start(pathToRemoteStreamEntry);
	// transient for this
	set_transient_for(windowParent);
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
	//
	show_all();
	// run the filechooser and grab the result
	response = run();
}

remoteStreamFileChooser::~remoteStreamFileChooser
(
	void
) 
{
}

std::string remoteStreamFileChooser::getURL
(
	void
)
{
	return pathToRemoteStreamEntry.get_text();
}

int remoteStreamFileChooser::getResponse
(
	void
)
{
	return response;
}

