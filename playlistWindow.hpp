#ifndef CPP_PLAYLISTWINDOW_CLASS_H
#define CPP_PLAYLISTWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file playlistWindow.hpp
 * \brief playlist window specifications
 * \details This class is used to load the playlist of EBU Player and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 28 august 2012
 * \copyright GNU GPLv3
 *
 */

#define PLAYLISTMIMETYPE 2

#include <gtkmm.h>
#include <gdkmm.h>
#include <iostream>
#include <string>
#include <fstream>


#include "genericFilechooserWindow.hpp"
#include "genericFeatures.hpp"
#include "playlistEngine.hpp"

class playlistWindow : public Gtk::Window
{
	public:

		/*!*
		* @brief Class constructor
		* @brief The playlistWindow class constructor instantiates the playlist window.\n\n
		* @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
		* @param[in] refGlade : This is the reference to your glade design
		*/
		playlistWindow
		(
			BaseObjectType* cobject,
			const Glib::RefPtr<Gtk::Builder>& refGlade
		);
	
		~playlistWindow();

	
		/**
		* @fn void on_openPlaylistFile_clicked(bool appendPlaylist, bool isFolder)
		* @brief This callback function will launch a filechooser to select and open a playlist file
		* @brief 
		* @note needs more documentation
		* @param[in] bool appendPlaylist : this boolean will determine if we must close the previous playlist before opening the new one or if we must just append a playlist to the current one.
		* @param[in] bool isFolder : this boolean will determine if we must open a directory as playlist or a playlist 
		* @return nothing if all is right or an error at compilation time.
		*/
		void on_openPlaylistFile_clicked
		(
			bool appendPlaylist,
			bool isFolder
		);
	
		/**
		* @fn void on_playlistMenuItem_activate(void)
		* @brief This callback function will show or hide the playlist window
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void on_playlistMenuItem_activate
		(
			void
		);
	
		/**
		* @fn void setTrackState(unsigned short state)
		* @brief This function will set the track state
		* @brief 
		* @note needs more documentation
		* @param[in] unsigned short state : 0 = STOP; 1 = PLAY ; 2 = PAUSE
		* @return nothing if all is right or an error at compilation time.
		*/
		void setTrackState
		(
			unsigned short state
		);
	
		/**
		* @fn void setLoadState(Gtk::ToggleButton * state)
		* @brief This function will indicates if a playlist is loaded or not
		* @brief 
		* @note needs more documentation
		* @param[in] Gtk::ToggleButton * state : A toggle button supports signal, a simple bool doesn't support it
		* @return nothing if all is right or an error at compilation time.
		*/
		void setLoadState
		(
			Gtk::ToggleButton * state
		);
	
		/**
		* @fn Glib::ustring getURIToPlay(void)
		* @brief This function will return the URI to play
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return Glib::ustring if all is right or an error at compilation time.
		*/
		Glib::ustring getURIToPlay
		(
			void
		);
	
		/**
		* @fn std::string getNameToPlay(void)
		* @brief This function will return the name of the file to play
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return std::string if all is right or an error at compilation time.
		*/
		std::string getNameToPlay
		(
			void
		);
		
		void nextTrack
		(
			void
		);
		
		void randomTrack
		(
			void
		);

		void previousTrack
		(
			void
		);
	
		/**
		* @fn void setEOSState(Gtk::ToggleButton * state)
		* @brief This function will indicates when the track played from the playlist is ended
		* @brief 
		* @note needs more documentation
		* @param[in] Gtk::ToggleButton * state : A toggle button supports signal, a simple bool doesn't support it
		* @return nothing if all is right or an error at compilation time.
		*/
		void setEOSState
		(
			Gtk::ToggleButton * state
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
	
		/**
		* @fn bool isLastTrack(void)
		* @brief This function will indicates if it's the last track
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return bool if all is right or an error at compilation time.
		*/
		bool isLastTrack
		(	
			void
		);
		
		/**
		* @fn void setShuffle(bool state)
		* @brief This function will enable or disable the shuffle mode
		* @brief 
		* @note needs more documentation
		* @param[in] bool state : true enable shuffle, false disable shuffle
		* @return nothing if all is right or an error at compilation time.
		*/
		void setShuffle
		(
			bool state
		);
		
		/**
		* @fn bool isShuffling(void)
		* @brief This function will return the variable isShuffle
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return bool if all is right or an error at compilation time.
		*/
		bool isShuffling
		(
			void
		);
	
		/**
		* @fn void setLoop(bool state)
		* @brief This function will enable or disable the loop mode
		* @brief 
		* @note needs more documentation
		* @param[in] bool state : true enable shuffle, false disable shuffle
		* @return nothing if all is right or an error at compilation time.
		*/
		void setLoop
		(
			bool state
		);
		
		/**
		* @fn bool isLooping(void)
		* @brief This function will return the variable isShuffle
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return bool if all is right or an error at compilation time.
		*/
		bool isLooping
		(
			void
		);
	protected:

		Glib::RefPtr<Gtk::Builder> m_refGlade; /*!< Glade references */
	
		Gtk::MenuButton * openPlaylistFile;
		Gtk::Button * savePlaylistFile;
		Gtk::Button * removeFileFromPlaylist;
		Gtk::MenuButton * addFileToPlaylist;
		
		Gtk::Menu* openPlaylistMenuPopup;
		Gtk::Menu* appendPlaylistMenuPopup;
		Glib::RefPtr<Gtk::UIManager> openPlaylistUIManager;
		Glib::RefPtr<Gtk::UIManager> appendPlaylistUIManager;
		Glib::RefPtr<Gtk::ActionGroup> openPlaylistActionGroup;
		Glib::RefPtr<Gtk::ActionGroup> appendPlaylistActionGroup;
	
		Gtk::ScrolledWindow * playlistScrolledWindow;
		
		//////////////////////
		//  playlist engine //
		//////////////////////
		playlistEngine * pls; /*!< pls This object represents the playlist engine */
		Gtk::ToggleButton * loaded; /*!< loaded This switch indicates if a playlist is loaded or not into the playlist engine */
		Gtk::ToggleButton * EOSState; /*!< loaded This switch indicates if a playlist is loaded or not into the playlist engine */
		bool isShuffle;
		bool isLoop;

	
		/**
		 * @fn void refGladeButton (void)
		 * @brief To link the glade references to the buttons
		 * @brief This method loads all required glade's references to connect the player buttons. If you add a new button to the media player into the Glade design, you must append his reference in this function.\n \n
		 * @brief Currently, the following menu items have been referenced :\n
		 * \li \c \b openPlaylistFile When clicked, this button launches a filechooser to select a playlist file
		 * \li \c \b savePlaylistFile When clicked, this button launches a filechooser which saves the current playlist file
		 * \li \c \b addFileToPlaylist When clicked, this button launches a filechooser to select a file and add it to the playlist
		 * \li \c \b removeFileFromPlaylistrewind When clicked, this button removes a selected file from the current playlist
	   * @param[in] void : no input parameter.
	   * @return noting or gtkmm-critical error/segmentation fault
	   * @note This function and his documentation must be completed
	   */
		void refGladeButton
		(
			void
		);
	
		/**
		 * @fn void refGladeWidget (void)
		 * @brief To link the glade references to the player features
		 * @brief This method loads all required glade's references to connect the playlist features. If you add a new playlist feature to Glade design, you must append his reference in this function.\n \n
		 * @brief Currently, the following features have been referenced :\n
		 * @param[in] void : no input parameter.
		 * @return noting or gtkmm-critical error/segmentation fault
		 * @note This function and his documentation must be completed
		 */
		void refGladeWidget
		(
			void
		);
	
		/**
		 * @fn void connectSignalClicked (void)
		 * @brief To connect the clicked signal to events handler
		 * @brief This method connects each widgets clicked with its own signal. If you add a new clicked event, you must append its signal connection here.\n \n
		 * @brief Currently, the following clicked signals have been connected:\n
		 * \li \c \b openPlaylistfile When connected, the openPlaylistfile button sends a signal to launch a filechooser through a callback function.
		 * \li \c \b savePlaylistfile When connected, the savePlaylistfile button sends a signal to launch a filechooser through a callback function.
		 * @param[in] void : no input parameter.
		 * @return  nothing if all is right or an error at compilation time.
		 * @note This function and his documentation must be completed
		 */
		void connectSignalClicked
		(
			void
		);
	
		/**
		* @fn void initPlaylistStates(void)
		* @brief To initiliaze the playlist interface states
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void initPlaylistStates
		(
			void
		);
	
		/**
		* @fn void initPlaylistMenuPopup(void)
		* @brief To initiliaze the popup menu of the opening playlist button.
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void initPlaylistMenuPopup
		(
			void
		);
	
		/**
		* @fn void on_savePlaylistFile_clicked(void)
		* @brief This callback function will launch a filechooser to save the current playlist into a playlist file
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void on_savePlaylistFile_clicked
		(
			void
		);
	
		/**
		* @fn void on_removeFileFromPlaylist_clicked(void)
		* @brief This callback function will remove a file from the current playlist
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void on_removeFileFromPlaylist_clicked
		(
			void
		);
	
		/**
		* @fn void on_addFileToPlaylist_clicked(void)
		* @brief This callback function will open a file and add it to the current playlist
		* @brief 
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void on_addFileToPlaylist_clicked
		(
			void
		);
		
	
		/**
		* @fn void convertFolderIntoPlaylist(std::string directory)
		* @brief This callback function will open a file and add it to the current playlist
		* @brief 
		* @note needs more documentation
		* @param[in] std::string directory : Path to the folder to convert into a playlist
		* @return nothing if all is right or an error at compilation time.
		*/
	/*	void convertFolderIntoPlaylist
		(
			std::string directory
		);*/
};

#endif /*CPP_PLAYLISTWINDOW_CLASS_H*/
