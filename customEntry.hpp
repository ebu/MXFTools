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
