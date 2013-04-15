#ifndef CPP_METADATAENGINE_CLASS_H
#define CPP_METADATAENGINE_CLASS_H

/*!
 * \project_name EBU Player
 * \file metadataEngine.hpp
 * \brief metadata engine specifications
 * \details This class is used to load the EBUCore metadata engine and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 26 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */

// c++
#include <iostream>
// xerces
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// gtk/gtkmm
#include <gtkmm/widget.h>
#include <gtkmm/expander.h>
#include <gtkmm/viewport.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/entry.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/textview.h>
#include <gtkmm/stock.h>
#include <gtkmm/assistant.h>
#include <gtkmm/aspectframe.h>
#include <gtkmm/radiobutton.h>
// glib/glibmm
#include <glibmm/miscutils.h>

// internal classes
#include "EBUCoreFeatures.hpp"
#include "genericFeatures.hpp"


/*! \class metadataEngine
 * \brief this class represent the metadata engine
 *
 * This class controls all the metadata features
 */

class metadataEngine {
	public:
		/*!*
		* @brief Class constructor
		* @brief The metadataEngine class constructor instantiates a new metadata engine and all its features.
		*/
		metadataEngine
		(
			Gtk::Label * statusRef,
			Gtk::Label * sourceRef, 
			Gtk::Label * sourceLabelRef,
			Gtk::Label * sizeRef, 
			Gtk::Label * sizeValueRef,
			Gtk::Box * lineOneRef, 
			Gtk::Box * lineTwoRef,
			Gtk::ScrolledWindow * FirstScrolledWindowBoxRef,
			Gtk::Box * SecondScrolledWindowBoxRef,
			Gtk::Box * boxEntriesRef,
			Gtk::Box * boxStatusRef,
			Gtk::Expander * expanderRootRef,
			Gtk::Viewport * viewportTreeRef
		);
		/*!*
		* @brief Class destructor
		* @brief The metadatarEngine class destructor destroys all variables and instanciate objects. This is the garbage collector class. It is always called at the end of class life.
		*
		*/
		virtual ~metadataEngine
		(
			void
		);
		
		void setRightSide
		(
			std::string filename
		);
		
		void constructTreeViewFromXML
		(
			std::string XMLfile
		);
		
		void constructTreeViewFromMXF
		(
			std::string MXFfile
		);
		
		void embedEBUCore
		(
			std::string MXFFilename,
			bool setHeader,
			bool isDark,
			bool isSidecar
		);
		
		void embedEBUCore
		(
			std::string MXFFilename,
			std::string XMLEBUCoreFilename,
			bool setHeader,
			bool isDark,
			bool isSidecar
		);
		
		void removeEBUCore
		(
			std::string MXFFilename
		);
		
		void removeEBUCore
		(
			std::string MXFFilename,
			std::string XMLEBUCoreFilename
		);
		
		void exportMXF2XMLEBUCore
		(
			std::string MXFFilename,
			std::string XMLEBUCoreOutputFilename
		);
		
		void exportXMLEBUCore
		(
			std::string XMLEBUCoreOutputFilename
		);
		
		unsigned int getViewportSize
		(
			void
		);
	protected:
	
		//Node attributes Tree model columns:
		class NodeAttributesModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:

			NodeAttributesModelColumns() 
			{ 
				add(metadataNodeAttributeIdCol);
				add(metadataNodeAttributeNameCol); 
				add(metadataNodeAttributeValueCol);
				add(metadataNodeAttributeStateCol);
				add(metadataNodeAttributeBgColorCol);
			}

			Gtk::TreeModelColumn<int> metadataNodeAttributeIdCol;
			Gtk::TreeModelColumn<Glib::ustring> metadataNodeAttributeNameCol;
			Gtk::TreeModelColumn<Glib::ustring> metadataNodeAttributeValueCol;
			Gtk::TreeModelColumn<Glib::ustring> metadataNodeAttributeStateCol;
			Gtk::TreeModelColumn<Glib::ustring> metadataNodeAttributeBgColorCol;
		};

		NodeAttributesModelColumns metadataNodeAttributesColumns;
		Glib::RefPtr<Gtk::TreeSelection> metadataNodeAttributesTreeviewSelection;

		//Node attributes Tree model columns:
		class NodeChildrenModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:

			NodeChildrenModelColumns() 
			{ 
				add(metadataNodeChildrenIdCol);
				add(metadataNodeChildrenNameCol);
				add(metadataNodeChildrenBgColorCol);
			}

			Gtk::TreeModelColumn<int> metadataNodeChildrenIdCol;
			Gtk::TreeModelColumn<Glib::ustring> metadataNodeChildrenNameCol;
			Gtk::TreeModelColumn<Glib::ustring> metadataNodeChildrenStateCol;
			Gtk::TreeModelColumn<Glib::ustring> metadataNodeChildrenBgColorCol;
		};

		NodeChildrenModelColumns metadataNodeChildrenColumns;
		Glib::RefPtr<Gtk::TreeSelection> metadataNodeChildrenTreeviewSelection;
	
		unsigned int viewportTreeMinimumWidth;
		std::vector<xercesc::DOMElement *> elReferences;
		Gtk::Assistant * metadataAssistant;
		Glib::RefPtr<Gtk::TextBuffer> metadataTextBuffer;
		Glib::RefPtr<Gtk::TextBuffer> assistantTextBuffer;
		Glib::RefPtr<Gtk::TextBuffer> assistantNodeChildrenBuffer;
		Gtk::Entry * attributeNameEntry;
		Gtk::Entry * attributeValueEntry;
		Gtk::Entry * tagValueEntry;
		bool addNode_type;
		std::string mxffilename; /*!< mxffilename Pointer to store temporarly the current played node */
		
		//left side widgets
		Gtk::ScrolledWindow * FirstScrolledWindowBox; /*!< FirstScrolledWindowBox */
		Gtk::Expander * expanderRoot;
		Gtk::Viewport * viewportTree;
		// right side widgets
		Gtk::Label * status;
	 	Gtk::Label * source, * sourceLabel;
	 	Gtk::Label * size, * sizeValue;
		Gtk::Box * lineOne, * lineTwo;
		Gtk::Box * SecondScrolledWindowBox;
		Gtk::Box * boxEntries, * boxStatus;
		// gdk color
		Gdk::RGBA black;
		Gdk::RGBA white;
		Gdk::RGBA whiteghost;
		Gdk::RGBA red;
		Gdk::RGBA blue;
		// registers
		Gtk::Expander * previousnode; /*!< previousnode Pointer to store temporarly the current node selected */
		Gtk::EventBox * previousnnodeevent; /*!< previousnode Pointer to store temporarly the current node selected */
		Gtk::Label * previousnodelabel; /*!< previousnode Pointer to store temporarly the current node selected */
		bool previouslabel;
		int previousnodepos;
		// boolean
		bool editionMode;
		// EBUCore Wrapper
		EBUCoreFeatures * SDKWrapper;
		xercesc::DOMDocument* xercescdoc;
	
	
		/*void writeMetadataBuffer
		(
			std::string filename
		);*/
		
		void recursiveConstructTreeView
		(
			xercesc::DOMElement * el, 
			Gtk::Expander * seed, 
			int depth
		);
		
		void configureEncapsultedBox
		(
			Gtk::Box * b
		);
		
		void configureExpander
		(
			Gtk::Expander * e
		);
		
		void configureEventLabel
		(
			Gtk::EventBox * evLabel, 
			Gtk::Label * label
		);
		
		void configureLabel
		(
			Gtk::Label * l
		);
		
		void finishEncapsulation
		(
			Gtk::Box * root, 
			Gtk::Expander * node
		);
		
		void finishEncapsulation
		(
			Gtk::Box * root, 
			Gtk::EventBox * evLabel
		);
		
		void constructEditableNode
		(
			xercesc::DOMElement * el
		);
		
		void on_openExpander_changed
		(
			Gtk::Expander * exp
		);
		
		bool on_enter_label
		(
			GdkEventCrossing*, 
			Gtk::EventBox *evLabel
		);
		
		bool on_leave_label
		(
			GdkEventCrossing*, 
			Gtk::EventBox *evLabel
		);
		
		bool on_press_label
		(
			GdkEventButton* event, 
			Gtk::EventBox *evLabel, 
			Gtk::Label * txtLabel
		);
		
		void configureNodeAttributesTreeview
		(
			xercesc::DOMNamedNodeMap *dom_attrs, 
			Gtk::Box * nodebox
		);
		
		void configureNodeChildrenTreeview
		(
			xercesc::DOMElement * children, 
			Gtk::Box * nodebox
		);
		
		void configureNodeText
		(
			std::string nodetext, 
			Gtk::Box * nodebox
		);
		
		void configureNodeAttributeEditionButtonsTreeview
		(
			Gtk::Box * nodebox, 
			Glib::RefPtr<Gtk::ListStore> attributeStore
		);
		
		void configureNodeEditionButtonsTreeview
		(
			Gtk::Box * nodebox, 
			Glib::RefPtr<Gtk::ListStore> nodeStore
		);
		
		void addNodeAttribute // virtual
		(
			Glib::RefPtr<Gtk::ListStore> metadataStore
		);
		
		void removeNodeAttribute // virtual
		(
			Glib::RefPtr<Gtk::ListStore> metadataStore
		);
		
		void addNode // virtual 
		(
			Glib::RefPtr<Gtk::ListStore> metadataStore
		);
		
		void removeNode // virtual
		(
			Glib::RefPtr<Gtk::ListStore> metadataStore
		);
		
		void on_addAttribute_assistant_apply
		(
			Glib::RefPtr<Gtk::ListStore> metadataStore
		);
		
		void on_assistant_cancel
		(
			void
		);
		
		void on_assistant_close
		(
			void
		);
		
		void on_addNodeText_assistant
		(
			Gtk::Box * text, 
			Gtk::Box * children
		);
		
		void on_addNode_assistant_apply
		(
			Glib::RefPtr<Gtk::ListStore> metadataStore
		);
};

#endif
