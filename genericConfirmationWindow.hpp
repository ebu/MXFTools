#ifndef CPP_GENERICCONFIRMATIONWINDOW_CLASS_H
#define CPP_GENERICCONFIRMATIONWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file genericAlertWindow.hpp
 * \brief Generic alert window specifications
 * \details This class is used to launch a generic dialog box where will be displayed the confirmation messages.
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 12 march 2013
 * \copyright GNU GPLv3
 *
 */

#include <gtkmm.h>
#include <gdkmm.h>
#include <iostream>
#include <string>
#include <fstream>

class genericConfirmationWindow : public Gtk::MessageDialog
{
public:

	/*!*
	* @brief Class constructor
	* @brief The genericConfirmationWindow class constructor instantiates a new confirmation message.\n\n
	* @param[in] Gtk::Window& alertParent : The window which instantiates the dialog message
	* @param[in] const Glib::ustring alertTitle : The title of the dialog message window
	* @param[in] std::string alertMessage : The message to write into the dialog box
	*/
	genericConfirmationWindow
	(
		Gtk::Window& alertParent,
	 	const Glib::ustring & alertTitle,
	 	std::string alertMessage
	);
	
	virtual ~genericConfirmationWindow
	(
		void
	);
	
protected:

};

#endif /*CPP_GENERICCONFIRMATIONWINDOW_CLASS_H*/
