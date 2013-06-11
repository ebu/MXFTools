/*!
 * \project_name EBU Player
 * \file genericConfirmationWindow.cpp
 * \brief Generic confirmation window functions
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 12 march 2013
 *
 * This software is published in LGPLv3.0
 *
 */

#include "genericConfirmationWindow.hpp"

genericConfirmationWindow::genericConfirmationWindow
(
		Gtk::Window& alertParent,
	 	const Glib::ustring & alertTitle,
	 	std::string alertMessage
) 
: 
	 Gtk::MessageDialog (alertParent, alertTitle)
{
	set_secondary_text(alertMessage);
	run();
}

genericConfirmationWindow::~genericConfirmationWindow
(
	void
)
{
}
