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
			std::string expression
		);
		
		~customEntry
		(
			void
		);
		
		bool error
		(
			void
		) const;
		
		std::string get_value
		(
			void
		);
		
		void set_value
		(
			std::string value
		);
	protected:
	
		bool m_error;
		
		std::string getRegEx
		(
			std::string type
		);
};

#endif
