#ifndef CPP_CUSTOMENTRY_CLASS_H
#define CPP_CUSTOMENTRY_CLASS_H

/*!
 * \project_name EBU Player
 * \file customEntry.hpp
 * \brief Custom entry
 * \details This class specification defines a custom entry
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 3 may 2013
 * \copyright This software is published under MPL v2.0
 *
 */


#include <iostream>
#include <iomanip>
#include <string>
#include <glibmm.h>
#include <gtkmm/entry.h> 
#include <gdkmm/screen.h> 
#include <gtkmm/cssprovider.h> 
#include <gtkmm/stylecontext.h> 

class customEntry : public Gtk::Entry
{
	public:
	
		customEntry
		(
			std::string expression,
	 		std::string placeholder
		);
		
		~customEntry
		(
			void
		);
		
		bool error
		(
			void
		) const;
		
		Glib::ustring get_value
		(
			void
		);
		
		void set_value
		(
			Glib::ustring value
		);
	protected:
	
		bool m_error;
		sigc::connection insert_text_connection;
		Glib::RefPtr<Glib::Regex> regex;
		
		
		Glib::ustring data;
		Glib::RefPtr<Gtk::CssProvider> css;
		Glib::RefPtr<Gtk::StyleContext> style_context;
		
		std::string getRegEx
		(
			std::string type
		);
		
		void insert_text_handler
		(
			void
		);
};

#endif
