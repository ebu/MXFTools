#ifndef CPP_PLAYLISTENGINE_CLASS_H
#define CPP_PLAYLISTENGINE_CLASS_H

/*!
 * \project_name EBU Player
 * \file playlistEngine.hpp
 * \brief playlist engine specifications
 * \details This class is used to implement the functions required by the playlist
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 5 march 2013
 * \copyright GNU GPLv3
 *
 */


#include <gtkmm/treeview.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeselection.h>
#include <gtkmm/stock.h>
#include <gtkmm/togglebutton.h>
#include <iostream>
#include <fstream>
#include <glibmm/convert.h>

#include "genericFeatures.hpp"

#ifdef _WIN32
    #define NEWLINE "\r\n"
#elif defined macintosh // OS 9
    #define NEWLINE "\r"
#else
    #define NEWLINE "\n" // Mac OS X uses \n
#endif

#define COLORSELECTED "#FF4422"
#define COLORPLAY "#11FF11"
#define COLOREVEN "#F9F9F9"
#define COLORODD "#EEEEEE"

class playlistEngine : public Gtk::TreeView
{
	public:

		/*!*
		* @brief Class constructor
		* @brief The playlistEngine class constructor instantiates the features and behavior of the playlist.\n\n
		* @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
		* @param[in] refGlade : This is the reference to your glade design
		*/
		playlistEngine
		(
			void
		);
		
		virtual ~playlistEngine
		(
			void
		);
	
		/**
		* @fn void setPlaylist(std::string pathToPlaylist, bool reset = false)
		* @brief This function will set the path to the playlist file/directory.
		* @brief 
		* @note needs more documentation
		* @param[in] std::string pathToPlaylist : This string is the path to the playlist file/directory
		* @param[in] bool reset :By the default, the reset switch of the playlist is set to false; it means that if we open a playlist, it will be appended to the current playlist. If the reset value is set to true, then the current playlist is closed and the new one is opened.
		* @return nothing if all is right or an error at compilation time.
		*/
		void setPlaylist
		(
			std::string pathToPlaylist,
			bool reset = false
		);
	
		/**
		* @fn void setFile(std::vector<std::string> file)
		* @brief This function will add files into the current playlist.
		* @brief 
		* @note needs more documentation
		* @param[in] std::vector<std::string> file : a vector with the path to all added files. Each path is a string.
		* @return nothing if all is right or an error at compilation time.
		*/
		void setFile
		(
			std::vector<std::string> file,
			bool reset = false
		);
		
		/**
		* @fn void writeM3UFile(std::string filename)
		* @brief This function will write the current playlist as a M3U playlist
		* @brief 
		* @note needs more documentation
		* @param[in] std::string filename : path to the output m3u file
		* @return std::string if all is right or an error at compilation time.
		*/
		void writeM3UFile
		(
			std::string filename
		);
		
		/**
		* @fn void removeFile(void)
		* @brief This function will remove a file into the playlist.
		* @brief The deletion occurs only if a file is currently selected.
		* @note needs more documentation
		* @param[in] void : no param.
		* @return nothing if all is right or an error at compilation time.
		*/
		void removeFile
		(
			void
		);
		
		/**
		* @fn bool isEmpty(void)
		* @brief This function will say if the playlist is empty or not.
		* @brief 
		* @note needs more documentation
		* @param[in] void : no param.
		* @return bool if all is right or an error at compilation time.
		*/
		bool isEmpty
		(
			void
		);
		
		/**
		* @fn void setRefRemoveButton(Gtk::Button & btn)
		* @brief This function will say if there are more than one track selected.
		* @brief 
		* @note needs more documentation
		* @param[in] void : no param.
		* @return bool if all is right or an error at compilation time.
		*/
		void setRefButton
		(
			Gtk::Button * del
		);
	
		/**
		* @fn void setState(const Gtk::StockID& stock_id)
		* @brief This callback function will forward the track state to the playlist engine
		* @brief 
		* @note needs more documentation
		* @param[in] const Gtk::StockID& stock_id : gtk stock id
		* @return nothing if all is right or an error at compilation time.
		*/
		void setState
		(
			Gtk::StockID stock_id
		);
	
		/**
		* @fn Glib::ustring getURItoFile(void)
		* @brief This callback function will forward the URI of the current track
		* @brief 
		* @note needs more documentation
		* @param[in] void : no paramas
		* @return Glib::ustring if all is right or an error at compilation time.
		*/
		Glib::ustring getURItoFile
		(
			void
		);
		
		/**
		* @fn Glib::ustring getNameOfFile(void)
		* @brief This callback function will forward the name of the current track
		* @brief 
		* @note needs more documentation
		* @param[in] void : no paramas
		* @return Glib::ustring if all is right or an error at compilation time.
		*/
		std::string getNameOfFile
		(
			void
		);
		
		void nextTrack
		(
			void
		);

		void previousTrack
		(
			void
		);
		
		void randomTrack
		(
			void
		);

		bool isLastTrack
		(
			void
		);
	
		/**
		* @fn void setNewTrackState(Gtk::ToggleButton * state)
		* @brief This function will indicates when a double-clicked track must be played from the playlist
		* @brief 
		* @note needs more documentation
		* @param[in] Gtk::ToggleButton * state : A toggle button supports signal, a simple bool doesn't support it
		* @return nothing if all is right or an error at compilation time.
		*/
		void setNewTrackState
		(
			Gtk::ToggleButton * state
		);
	protected:
	
		//Tree model columns
		class ModelColumns : public Gtk::TreeModel::ColumnRecord 
		{
			public:
		
				ModelColumns()
				{ 
					add(playlistidcol);
					add(playlistposcol);
					add(playlistnamecol); 
					add(playlistlengthcol);
					add(playlistformatcol); 
					add(playliststatecol);
					add(playlistdateofcreationcol);
					add(playlisturlcol);
					add(playlistcellbgcolorcol);
				}

			Gtk::TreeModelColumn<std::size_t> playlistidcol;
			Gtk::TreeModelColumn<unsigned int> playlistposcol;
			Gtk::TreeModelColumn<std::string> playlistnamecol;
			Gtk::TreeModelColumn<Glib::ustring> playlistlengthcol;
			Gtk::TreeModelColumn<Glib::ustring> playlistformatcol;
			Gtk::TreeModelColumn<Glib::RefPtr< Gdk::Pixbuf >> playliststatecol;
			Gtk::TreeModelColumn<Glib::ustring> playlistdateofcreationcol;
			Gtk::TreeModelColumn<Glib::ustring> playlisturlcol;
			Gtk::TreeModelColumn<Glib::ustring> playlistcellbgcolorcol;
	  	};
	  	
	  	ModelColumns playlistColumns;

		Glib::RefPtr<Gtk::ListStore> playlistStore;
		Glib::RefPtr<Gtk::TreeSelection> playlistTreeviewSelection;
		std::vector<Gtk::TreeModel::Path> playlistPathList;
	
	
		std::vector<std::string> pathfilestoplay;
		unsigned int selectedTrack;
		int position;
		std::size_t trackToPlay;
		Gtk::StockID trackToPlayState;
		Gtk::Button * removeBtn;
		Gtk::TreeModel::Row previousRow;
		unsigned int previousID;
		Glib::RefPtr< Gdk::Pixbuf > stateNull;
		Glib::RefPtr< Gdk::Pixbuf > previousState;	
		Gtk::ToggleButton * playlistNewTrack;  /*!< playlistNewTrack boolean to know if a new track is set */
		
		/**
		* @fn void configurePlaylistTreeview(void)
		* @brief To configure the Gtk treeview which will receives the playlist data structure
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void configurePlaylistTreeview
		(
			void
		);
		
		/**
		* @fn std::string writeRelativePath(const std::string& filename, const std::string& path)
		* @brief This function will write the path to the file like a relative path
		* @brief 
		* @note needs more documentation
		* @param[in] const std::string& filename : the name of the file
		* @param[in] const std::string& path : the absolute path to the directory where is the file set by the filename argument
		* @return std::string if all is right or an error at compilation time.
		*/
		std::string writeRelativePath
		(
			const std::string& filename, 
			const std::string& path
		);
		
		/**
		* @fn std::string convertIntoRelativePath(const std::string& filename, const std::string& path)
		* @brief This function will convert the absolute file path into relative path.
		* @brief 
		* @note needs more documentation
		* @param[in] const std::string& filename : the name of the file
		* @param[in] const std::string& path : the absolute path to the directory where is the file set by the filename argument
		* @return std::string if all is right or an error at compilation time.
		*/
		std::string convertIntoRelativePath 
		(
			const std::string& filename, 
			const std::string& path
		);
		
		/**
		* @fn unsigned int getSize(void)
		* @brief This function will return the size of the current playlist
		* @brief
		* @param[in] void : no params
		* @return int if all is right or an error at compilation time.
		*/
		unsigned int getSize
		(
			void
		);
		
		 void on_treeview_row_activated
		 (
		 	const Gtk::TreeModel::Path& path, 
		 	Gtk::TreeViewColumn* column
		 );
		/**
		* @fn bool on_playlist_selection_changed(GdkEventButton* event) 
		* @brief This callback function will detect the file selection changes into the playlist
		* @brief 
		* @note needs more documentation
		* @param[in] event : GdkEventButton
		* @return bool if all is right or an error at compilation time.
		*/
		bool on_playlist_selection_changed
		(
			GdkEventButton* event
		);
		
		/**
		* @fn bool keyPressed(GdkEventKey *event) 
		* @brief This callback function will detect when a keyboard button is pressed.
		* @brief 
		* @note needs more documentation
		* @param[in] event : GdkEventKey
		* @return bool if all is right or an error at compilation time.
		*/
		bool keyPressed
		(
			GdkEventKey* event
		);
		
		/**
		* @fn bool keyReleased(GdkEventKey *event) 
		* @brief This callback function will detect when a keyboard button is released.
		* @brief 
		* @note needs more documentation
		* @param[in] event : GdkEventKey
		* @return bool if all is right or an error at compilation time.
		*/
		bool keyReleased
		(
			GdkEventKey* event
		);
		
		/**
		* @fn void on_row_deleted(const Gtk::TreeModel::Path& path)
		* @brief This callback function will detect when a file is removed from the playlist
		* @brief 
		* @note needs more documentation
		* @param[in] path : const Gtk::TreeModel::Path& 
		* @return bool if all is right or an error at compilation time.
		*/
		void on_row_deleted
		(
			const Gtk::TreeModel::Path& path
		);
		
		/**
		* @fn void on_row_added(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter)
		* @brief This callback function will detect when a file is added to the playlist
		* @brief 
		* @note needs more documentation
		* @param[in] path : const Gtk::TreeModel::Path&
		* @param[in] iter : const Gtk::TreeModel::iterator&
		* @return nothing if all is right or an error at compilation time.
		*/
		/*void on_row_added
		(
			const Gtk::TreeModel::Path& path, 
			const Gtk::TreeModel::iterator& iter
		);*/

		/**
		* @fn void on_row_reordored(const Glib::RefPtr<Gdk::DragContext>& context)
		* @brief This callback function will detect when the playlist is reordered.
		* @brief 
		* @note needs more documentation
		* @param[in] context : const Glib::RefPtr<Gdk::DragContext>&
		* @return nothing if all is right or an error at compilation time.
		*/
		void on_row_reordered
		(
			const Glib::RefPtr<Gdk::DragContext>& context
		);

		
		/**
		* @fn void colorizePlaylist(bool reordered)
		* @brief This function will color the playlist rows and update the row id
		* @brief 
		* @note needs more documentation
		* @param[in] bool reordered : this boolean will determine if the playlist is being reordered or not
		* @return nothing if all is right or an error at compilation time.
		*/
		void colorizePlaylist
		(
			bool reordered = false
		);
		
		/**
		* @fn bool isSelected(void)
		* @brief This function will say if a track is selected into the playlist or not.
		* @brief 
		* @note needs more documentation
		* @param[in] void : no param.
		* @return bool if all is right or an error at compilation time.
		*/
		bool isSelected
		(
			void
		);
		
		/**
		* @fn bool hasMultipleSelection(void)
		* @brief This function will say if there are more than one track selected.
		* @brief 
		* @note needs more documentation
		* @param[in] void : no param.
		* @return bool if all is right or an error at compilation time.
		*/
		bool hasMultipleSelection
		(
			void
		);
};

#endif /*CPP_PLAYLISTENGINE_CLASS_H*/
