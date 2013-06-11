/*!
 * \project_name EBU Player
 * \file genericAlertWindow.cpp
 * \brief Generic alert window functions
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 4 march 2013
 *
 * This software is published in LGPLv3.0
 *
 */

#include "genericAlertWindow.hpp"

genericAlertWindow::genericAlertWindow
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

genericAlertWindow::~genericAlertWindow
(
	void
)
{
}
