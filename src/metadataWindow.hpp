#ifndef CPP_METADATAWINDOW_CLASS_H
#define CPP_METADATAWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file metadataWindow.hpp
 * \brief metadata window specifications
 * \details This class is used to load the EBUCore Metadata viewer and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 20 august 2012
 * \dateOfUpdate 21 march 2012
 * \copyright This software is published under MPL v2.0
 *
 */


#include <gtkmm.h>

#include <gtkmm/cssprovider.h> 
#include <gtkmm/stylecontext.h>

#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <stdexcept>
#include <fstream>

// include files and libraries
#include "metadataEncapsulationDialog.hpp"
#include "metadataEncapsulationInputOutputDialog.hpp"
#include "remoteStreamFileChooser.hpp"
#include "genericFilechooserWindow.hpp"
#include "genericFilechooserInputOutput.hpp"
#include "metadataEngine.hpp"


#define MXFMETADATAMIMETYPE 4
#define XMLMETADATAMIMETYPE 7

class metadataWindow : public Gtk::Window
{
public:
	/*!*
	* @brief Class constructor
	* @brief The metadataWindow class constructor instantiates a new EBUCore Metadata window and all its features.\n\n
	* @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
	* @param[in] refGlade : This is the reference to your glade design
	*/
	metadataWindow
	( 
		BaseObjectType* cobject, 
		const Glib::RefPtr<Gtk::Builder>& refGlade
	);
	
	/*!*
	* @brief Class destructor
	* @brief The metadataWindow class destructor destroys all variables and instanciate objects of metadataWindow class. This is the garbage collector class. It is always called at the end of class life.
	*
	*/
	~metadataWindow
	(
		void
	);
	
protected:
	
	// Glade reference
	Glib::RefPtr<Gtk::Builder> m_refGlade; /*!< Glade references */
	
	// gtk button
	Gtk::Switch* switchEBUCoreEdition; /*!< switchEBUCoreEdition edition mode button */
	
	Gtk::Button* createEBUCore;
	Gtk::MenuButton* removeEBUCore; /*!< disable edition mode button */
	Gtk::MenuButton* importEBUCore; /*!< import an EBUCore file button */
	Gtk::MenuButton* exportEBUCore; /*!< export an EBUCore file button */
	Gtk::MenuButton* generateEBUCoreST434; /*!< export an EBUCore file button */
	
	// Menu, UIManager and ActionGroups
	Gtk::Menu* importEBUCoreMenuPopup;
	Gtk::Menu* importEBUCoreXMLMenuPopup;
	Gtk::Menu* importEBUCoreMXFMenuPopup;
	Gtk::Menu* exportEBUCoreMenuPopup;
	Gtk::Menu* removeEBUCoreMenuPopup;
	Gtk::Menu* generateEBUCoreST434MenuPopup;
	Glib::RefPtr<Gtk::UIManager> importEBUCoreUIManager;
	Glib::RefPtr<Gtk::UIManager> importEBUCoreXMLUIManager;
	Glib::RefPtr<Gtk::UIManager> importEBUCoreMXFUIManager;
	Glib::RefPtr<Gtk::UIManager> exportEBUCoreUIManager;
	Glib::RefPtr<Gtk::UIManager> removeEBUCoreUIManager;
	Glib::RefPtr<Gtk::UIManager> generateEBUCoreST434UIManager;
	Glib::RefPtr<Gtk::ActionGroup> importEBUCoreActionGroup;
	Glib::RefPtr<Gtk::ActionGroup> importEBUCoreXMLActionGroup;
	Glib::RefPtr<Gtk::ActionGroup> importEBUCoreMXFActionGroup;
	Glib::RefPtr<Gtk::ActionGroup> exportEBUCoreActionGroup;
	Glib::RefPtr<Gtk::ActionGroup> removeEBUCoreActionGroup;
	Glib::RefPtr<Gtk::ActionGroup> generateEBUCoreST434ActionGroup;
	
	
	// window widgets
	Gtk::Viewport* viewportTree; /*!< viewportTree the Gtk Viewport where is stored the expander */
	Gtk::Expander * expanderRoot; /*!< expanderRoot the Gtk Expander is the EBUCore Metadata xml tree root */
	Gtk::ScrolledWindow * FirstScrolledWindowBox; /*!< FirstScrolledWindowBox */
	Gtk::Box * SecondScrolledWindowBox; /*!< SecondScrolledWindowBox */
	Gtk::Box * boxEntries;  /*!< boxEntries */
	Gtk::Box * boxStatus; /*!< boxStatus */
	
	// right side widgets
	Gtk::Label * status;
 	Gtk::Label * source, * sourceLabel;
 	Gtk::Label * size, * sizeValue;
	Gtk::Box * lineOne, * lineTwo;

	// left side widgets
	
	// engine
	metadataEngine * engine;
	
	//css
//	Glib::RefPtr<Gtk::CssProvider> css;
//	Glib::RefPtr<Gtk::StyleContext> style_context;
	
	/**
	* @fn void refGladeWidgets(const Glib::RefPtr<Gtk::Builder>& refGlade)
	* @brief To link the glade references to the required player widgets
	* @brief This method loads all required glade's references to connect the player widgets. If you add a new widget to the media player into the Glade design, you must append his reference in this function.\n \n
	* @brief Currently, the following widgets have been referenced :\n
	* \li \c \b expander1 We use this widget to encapsulate the multiple levels of XML DOM
	* \li \c \b viewport1 We use this widget to encapsulate the expander1\n \n
	* @param[in] refGlade : This is the reference to your glade widgets - It's mandatory.
	* @return noting or gtkmm-critical error/segmentation fault
	* @note This function and his documentation must be completed or transfert
	*/
	void refGladeWidgets
	(
		const Glib::RefPtr<Gtk::Builder>& refGlade
	);
	
	/**
	* @fn refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade)
	* @brief To link the glade references to the buttons
	* @brief This method loads all required glade's references to connect the metadata viewer buttons. If you add a new button to the metadata viewer into the Glade design, you must append his reference in this function.\n \n
	* @brief Currently, the following buttons have been referenced :\n
	* \li \c \b importXML When clicked, this button should import an XML metatada file into the metadata viewer 
	* \li \c \b exportXML  When clicked, this button should export an XML metatada from metadata viewer into an XML file.
	* \li \c \b modifyXML When clicked, this button should open a secondary metadata viewer where the user will modify the metadata of the first viewer.
	* \li \c \b saveXML When clicked, this button will save the XML metadata into the MXF file.
	* \li \c \b XMLconformance When clicked, this button starts an EBUCore conformance analysis on the metadata. \n \n
	* @param[in] refGlade : This is the reference to your glade player features - It's mandatory.
	* @return nothing if all is right or gtkmm-critical error/segmentation fault
	* @note This function and his documentation must be completed
	*/
	void refGladeButton
	(
		const Glib::RefPtr<Gtk::Builder>& refGlade
	);
	
	/**
	* @fn void connectSignals (void)
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
	void connectSignals
	(
		void
	);
	/**
	* @fn void on_createEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_createEBUCore_clicked
	(
		void
	);

	/**
	* @fn void on_importEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] bool EBUCoreKind : this boolean will determine if we want open an XML or MXF EBUCore file (0 = XML, 1 = MXF).
	* @param[in] bool isRemote : this boolean will determine if we want open a local file or a remote file (0 = local, 1 = remote)
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_importEBUCore_clicked
	(
		bool EBUCoreKind,
		bool isRemote
	);
	
	/**
	* @fn void on_switchEBUCoreEdition_activate(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_switchEBUCoreEdition_activate
	(
		void
	);
	
	/**
	* @fn void on_embedXML2MXFEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_embedXML2MXFEBUCore_clicked
	(
		void
	);	
	
	/**
	* @fn void on_embedEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_embedEBUCore_clicked
	(
		void
	);
	
	/**
	* @fn void on_exportMXF2XMLEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_exportMXF2XMLEBUCore_clicked
	(
		void
	);	
	
	/**
	* @fn void on_exportEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_exportEBUCore_clicked
	(
		void
	);		
	
	/**
	* @fn void on_removeEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_removeEBUCore_clicked
	(
		void
	);		
	
	/**
	* @fn void on_saveAndRemoveEBUCore_clicked(void)
	* @brief
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_saveAndRemoveEBUCore_clicked
	(
		void
	);	
	
	void on_generateEBUCoreST434Metadata_clicked
	(
		void
	);
	void on_generateEBUCoreST434Mux_clicked
	(
		void
	);
	void on_generateEBUCoreST434Deep_clicked
	(
		void
	);
	
	/**
	* @fn void initMetadataMenuPopup(void)
	* @brief To initiliaze the popup menu of the importing/exporting metadata button.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void initMetadataMenuPopup
	(
		void
	);
	
	/**
	* @fn void initMetadataWindow(void)
	* @brief To initiliaze the metadata window interface elements/states
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void initMetadataWindow
	(
		void
	);
	
	/**
	* @fn void initRightSide(void)
	* @brief To initiliaze the right side of the metadata window interface elements/states
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void initRightSide
	(
		void
	);
};
#endif
