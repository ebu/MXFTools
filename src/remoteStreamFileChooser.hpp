#ifndef CPP_REMOTESTREAMFILECHOOSER_CLASS_H
#define CPP_REMOTESTREAMFILECHOOSER_CLASS_H

/*!
 * \project_name EBU Player
 * \file remoteStreamFileChooser.hpp
 * \brief Remote stream filechooser window specifications
 * \details This class is used to load the remote stream filechooser.
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 11 march 2013
 * \copyright GNU GPLv3
 *
 */


#include <gtkmm.h>
#include <gdkmm.h>
#include <iostream>
#include <string>


class remoteStreamFileChooser : public Gtk::Dialog
{
	public:

		/*!*
		* @brief Class constructor
		* @brief The remoteStreamWindow class constructor instantiates the remote stream filechooser.\n\n
		* @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
		* @param[in] refGlade : This is the reference to your glade design
		*/
		remoteStreamFileChooser
		(
		 	Gtk::Window& windowParent,
			std::string windowTitle,
			std::string messageDialog,
			bool modal
		);
	
		~remoteStreamFileChooser();
	
		/**
		* @fn std::string getURL(void)
		* @brief This function will return the typed URL.
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return std::string if all is right or an error at compilation time.
		*/
		std::string getURL
		(
			void
		);

		/**
		* @fn int getResponse(void)
		* @brief This function will return the Gtk::ResponType of the filechooser
		* @brief
		* @note needs more documentation
		* @param[in] void : no params
		* @return int if all is right or an error at compilation time.
		*/
		int getResponse
		(
			void
		);

	protected:
		Gtk::Box * remoteStreamBox;
		Gtk::Label remoteStreamLabel;
		//Gtk::Button * quitRemoteStream;/*!< quitRemoteStream This button will close the remote filechooser window */
		//Gtk::Button * openRemoteStream;/*!< openRemoteStream This button will open the remote file typed */
		Gtk::Entry pathToRemoteStreamEntry;/*!< pathToRemoteStreamEntry This entry will store the path to the remote stream */
		int response;

};

#endif /*CPP_REMOTESTREAMFILECHOOSER_CLASS_H*/
