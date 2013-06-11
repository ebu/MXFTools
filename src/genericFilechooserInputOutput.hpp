#ifndef CPP_GENERICFILECHOOSERINPUTOUTPUT_CLASS_H
#define CPP_GENERICFILECHOOSERINPUTOUTPUT_CLASS_H

/*!
 * \project_name EBU Player
 * \file genericFilechooserWindow.hpp
 * \brief EBUCore Metadata encapsulation dialog specifications
 * \details This class is used to load the EBUCore Metadata encapsulation dialog
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 28 march 2013
 * \dateOfUpdate 28 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */


#include <gtkmm/dialog.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/entry.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <iostream>

#include "genericFilechooserWindow.hpp"

#define XMLEBUCOREMETADATAMIMETYPE 3
#define MXFMETADATAMIMETYPE 4

class genericFilechooserInputOutput : public Gtk::Dialog
{
public:
	/*!*
	* @brief Class constructor
	* @brief The metadataEncapsulationDialog class constructor instantiates a new EBUCore Metadata encapsulation dialog and all its features.\n\n
	* @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
	* @param[in] refGlade : This is the reference to your glade design
	*/
	genericFilechooserInputOutput
	( 
		Gtk::Window& parent, 
		const Glib::ustring& title,
		const Glib::ustring& inputLabel,
		const Glib::ustring& outputLabel,
		const Glib::ustring& inputButtonLabel,
		const Glib::ustring& outputButtonLabel,
		const Glib::ustring& inputPlaceHolderLabel,
		const Glib::ustring& outputPlaceHolderLabel,
		bool setModal = true
	);
	
	/*!*
	* @brief Class destructor
	* @brief The metadataEncapsulationDialog class destructor destroys all variables and instanciate objects of metadataEncapsulationDialog class. This is the garbage collector class. It is always called at the end of class life.
	*
	*/
	~genericFilechooserInputOutput
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
	
	/**
	* @fn std::string getInputPath(void)
	* @brief This function will return the input path.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return std::string if all is right or an error at compilation time.
	*/
	std::string getInputPath
	(
		void
	);
	
	/**
	* @fn std::string getOutputPath(void)
	* @brief This function will return the input path.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return std::string if all is right or an error at compilation time.
	*/
	std::string getOutputPath
	(
		void
	);

	
protected:
	Gtk::Box * dialogOptionBox;
	// gtk button
	Gtk::Button* pathInputButton; /*!< SelectPathToOutputMXF */
	Gtk::Button* pathOutputButton; /*!< SelectPathToOutputMXF */
	// gtk entries
	Gtk::Entry * pathInputEntry;
	Gtk::Entry * pathOutputEntry;
	// gtk label
	//Gtk::Label * partitionLabel;
	//Gtk::Label * encodingLabel;
	//Gtk::Label * pathLabel;
	// dialog response
	int response; /*!< response the Gtk::ResponType selected by the user into the filechooser */
	
	/**
	* @fn void connectSignalClicked (void)
	* @brief To connect the clicked signal to events handler 
	* @brief This method connects each widgets clicked with its own signal. If you add a new clicked event, you must append its signal connection here.\n \n
	* @brief Currently, the following clicked signals have been connected:\n
	* \li \c \b importXML When connected, this button sends a signal to import an XML metatada file into the metadata first viewer 
	* \li \c \b exportXML  When connected, this button sends a signal to export an XML metatada from metadata viewer into an XML file.
	* \li \c \b modifyXML When connected, this button sends a signal to open a secondary metadata viewer where the user will modify the metadata of the first viewer.
	* \li \c \b saveXML When connected, this button sends a signal to save the XML metadata into the currently loaded MXF file.
	* \li \c \b XMLconformance When connected, this button sends a signal to run an EBUCore conformance analysis on the metadata.\n \n
	* @param[in] void : No params required
	* @return  nothing if all is right or an error at compilation time.
	* @note This function and his documentation must be completed
	*/
	void connectSignalClicked
	(
		void
	);
	
	void initDialogWidgets
	(
		void
	);
	
	void on_close_dialog
	(
		void
	);

	void on_terminate_clicked
	(
		void
	);

	void on_pathInputButton_clicked
	(
		void
	);

	void on_pathOutputButton_clicked
	(
		void
	);
	
	
};
#endif
