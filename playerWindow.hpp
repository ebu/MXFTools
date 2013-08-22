#ifndef CPP_PLAYERWINDOW_CLASS_H
#define CPP_PLAYERWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file playerWindow.hpp
 * \brief player window specifications
 * \details This class is used to load the EBU Player and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 * \copyright GNU GPLv3
 *
 */


#include <gtkmm/window.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/win32/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif
#include <iostream>
#include <string.h>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <time.h>

#include "playerEngine.hpp"
#include "mxfGeneratorWindow.hpp"
#include "playlistWindow.hpp"
#include "metadataWindow.hpp"
#include "remoteStreamFileChooser.hpp"
#include "genericFilechooserWindow.hpp"

/*! \class playerWindow
 * \brief this class represent the player window
 *
 * This class controls all the player features and its links
 */

#define PLAYERMIMETYPE 1

class playerWindow : public Gtk::Window
{
public:

/////////////////////////////////
//////                     ////// 
//////  public variables   //////
//////                     //////
/////////////////////////////////


/////////////////////////////////
//////                     ////// 
//////  public functions   //////
//////                     //////
/////////////////////////////////

	//////////////////////
	//   constructor    //
	//////////////////////

	/*!*
	 * @brief Class constructor
	 * @brief The playerWindow class constructor instantiates a new player window and all  its features.\n\n
	 * @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
	 * @param[in] refGlade : This is the reference to your glade design
	 */
	playerWindow
	(
		BaseObjectType* cobject,
		const Glib::RefPtr<Gtk::Builder>& refGlade
	);
	
	
	//////////////////////
	//    destructor    //
	//////////////////////
	
	/*!*
	 * @brief Class destructor
	 * @brief The playerWindow class destructor destroys all variables and instanciate objects. This is the garbage collector class. It is always called at the end of class life.
	 *
	 */
	virtual ~playerWindow
	(
		void
	);

protected:

/////////////////////////////////
//////                     ////// 
////// protected variables //////
//////                     //////
/////////////////////////////////

	//////////////////////
	// Glade references //
	//////////////////////
	
	Glib::RefPtr<Gtk::Builder> m_refGlade; /*!< m_refGlade : Glade references */
  

	//////////////////////
	//   player menu    //
	//////////////////////
  
	// _File
	Gtk::MenuItem* openMenuItem;/*!< openMenuItem This menu item opens the media filechooser */
	Gtk::MenuItem* openRemoteMenuItem;/*!< This menu item opens the remote media filechooser */
	Gtk::MenuItem* openNewPlaylistMenuItem; /*!< This menu item opens a playlist filechooser to select a playlist file and then closes the current playlist before to load the new one. */
	Gtk::MenuItem* openNewDirPlaylistMenuItem;  /*!< This menu item opens a filechooser to select a multimedia directory and then closes the current playlist before to load the multimedia directory as a playlist. */
	Gtk::MenuItem* openAppendPlaylistMenuItem; /*!< This menu item opens a playlist filechooser to select a playlist file to append to the current playlist. */
	Gtk::MenuItem* openAppendDirPlaylistMenuItem;/*!< This menu item opens a filechooser to select a multimedia directory to append to the current playlist. */
	Gtk::MenuItem* quitMenuItem;/*!< quitMenuItem This menu item closes the EBU Mediaplayer*/
	
	// _View
	Gtk::MenuItem* metadataMenuItem;/*!< metadataMenuItem This menu item opens the EBUCore Metadata Editor*/
	Gtk::MenuItem* playlistMenuItem;/*!< playlistMenuItem This menu item opens the playlist window */
	
	// _Tool
	//Gtk::MenuItem* TranscoderMenuItem;/*!< TranscoderMenuItem This menu item opens the MXF Transcoder*/
	//Gtk::MenuItem* MXFAnalyserMenuItem;/*!< MXFAnalyserMenuItem This menu item opens the MXF Analyzer*/
	Gtk::MenuItem* MXFGeneratorMenuItem;/*!< MXFGeneratorMenuItem This menu item opens the MXF OP1a Generator*/
	
	
	// _Help
	//Gtk::MenuItem* EBUCoreMenuItem;/*!< open ebucore menu item*/
	

	//////////////////////
	//  player buttons  //
	//////////////////////
	
	/* player classical buttons */
	
	Gtk::Button* play; /*!< play The playback button*/
	Gtk::Button* pause;/*!< pause Button to pause playback*/
	Gtk::Button* stop;/*!< stop Button to stop the playback*/
	Gtk::Button* rewind;/*!< rewind Button to fast rewind the playback*/
	Gtk::Button* forward;/*!< forward Button to fast forward the playback*/
	Gtk::Button* fullscreenBtn;/*!< fullscreen Button to switch in fullscreen mode*/
	Gtk::Button* unfullscreenBtn;/*!< unfullscreen Button to leave the fullscreen mode*/
	
	/* A timecode can easily be displayed as a Gtk::Label, but 
	we need to switch between a timecode in frames and the 
	classical timecode (HHH:MM:SS). An easy way to implement a 
	switching function and handle it is to store the Gtk::Label 
	inside a Gtk::Button which will implement the "clicked" 
	listener. */
	
	Gtk::Button* progress_label;/*!< progress_label Returns the current timecode in hours:minutes:secondes inside a Gtk::Label stored into a Gtk::Button */
	Gtk::Button* progress_total_label;/*!< progress_total_label Returns the total timecode (stream's duration) in hours:minutes:secondes inside a Gtk::Label stored into a Gtk::Button */
	Gtk::Button* frame_label;/*!< frame_label Returns the current timecode as a frame number inside a Gtk::Label stored into a Gtk::Button */
	Gtk::Button* frame_total_label;/*!< frame_total_label Returns the total timecode (stream's duration in frame) as a frame number inside a Gtk::Label stored into a Gtk::Button*/
	
	Gtk::Scale* volume_scale;/*!< volume_scale : volume levels*/
	
	/* player advance buttons */
	
	Gtk::Button* next;/*!< next Button to play the next track of a playlist*/
	Gtk::Button* previous;/*!< previous Button to play the previous track of a playlist*/
	Gtk::ToggleButton* playlist_show;/*!< Playlist This toggle button will open or close the playlist window. */
	Gtk::ToggleButton* metadata_show;/*!< Playlist This toggle button will open or close the playlist window. */

	//////////////////////
	//  player widgets  //
	//////////////////////
	
	Gtk::DrawingArea * videoDrawingArea;/*!< videoDrawingArea Gtk drawing area where will be displayed the Gstreamer playout */
	Gtk::Scale* progress_scale;/*!< progress_scale A progress scale to show visually the timecode and its evolution */
	Gtk::MenuBar* menubar;/*!< menubar : The Gtk menu bar */
	Gtk::AspectFrame* borderleft;/*!< borderleft aspect frame left */
	Gtk::AspectFrame* borderright;/*!< borderright aspect frame right*/
	Gtk::Box* timeline_box;/*!< timeline_box gtk box who contains the timeline/timecode */
	Gtk::Box* player_control_box;/*!< player_control_box gtk box who contains player controls */
	Gtk::Box* status_box;/*!< status_box gtk box who contains the status bar */

	//////////////////////
	//  player states   //
	//////////////////////
	bool isFullscreen; /*!< isFullscren boolean to know if fullscreen is enable */
	bool timecodeState;/*!< timecodeStatus boolean to know if the timecode is displayed as HHH:MM:SS or as a frame number. */
	Gtk::ToggleButton playlistIsLoaded; /*!< playlistIsLoaded boolean to know if we have an opened playlist */
	Gtk::ToggleButton EOSState; /*!< playlistIsLoaded boolean to know if we have an opened playlist */
	Gtk::ToggleButton playlistNewTrack; /*!< playlistNewTrack boolean to know if a new track is set */
	Gtk::ToggleButton * shuffle; /*!< shuffleState boolean to know if we have an opened playlist */
	Gtk::ToggleButton * loop; /*!< loopState boolean to know if we have an opened playlist */

	//////////////////////
	//  player signals  //
	//////////////////////

	sigc::connection timeout_timecode; /*!< timeout_timecode this timer is used to refresh the media duration and the current timeline's position */
	sigc::connection timeout_fullscreen; /*!< timeout_fullscreen This timer is used to hide player controls */
	sigc::connection mouse_motion_signal_drawingArea; /*!< mouse_motion_signal_drawingArea mouse motion detection signal for the drawing area*/
	time_t  nextmotiondetection; /*!< nextmotiondetection time before next motion detection */
	
	//////////////////////
	//  player engine   //
	//////////////////////
	playerEngine * gst; /*!< gst This object represents the Gstreamer engine */
	Gst::State previousState;
	
	//////////////////////
	//  others windows  //
	//////////////////////
	mxfGeneratorWindow * MXFGenWindow; /*!< MXFGenWindow This object implements the MXF Generator window */
	playlistWindow * playlist; /*!< playlistWindow This object implements the playlist window */
	metadataWindow * metadata; /*!< playlistWindow This object implements the playlist window */
	

/////////////////////////////////
//////                     ////// 
////// protected functions //////
//////                     //////
/////////////////////////////////
	
	/**
	 * @fn void initMPStates(void)
	 * @brief To initialize mediaplayer interface's states
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void initMPStates(void);
	
	/**
	 * @fn void refGladeMenu (void)
	 * @brief To link the glade references to the menu items
	 * @brief This method loads all required glade's references to connect the menu items. If you add a new menu item to Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following menu items have been referenced :\n
	 * \li \c \b openMenuItem Open a file (in File menu)
	 * \li \c \b quitMenuItem Quit the application (in File menu)
	 * \li \c \b metadataMenuItem Open the metadata viewer (in View menu)
	 * \li \c \b playlistMenuItem Open the playlist viewer (in View menu)
	 * \li \c \b EBUCoreMenuItem Open the EBUCore documentation (in Help menu)\n \n
	 * @param[in] void : no input parameter.
	 * @return nothing if all is right or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
	 */ 
	void refGladeMenu
	(
		void
	);
	
	/**
	 * @fn void refGladeButton (void)
	 * @brief To link the glade references to the buttons
	 * @brief This method loads all required glade's references to connect the player buttons. If you add a new button to the media player into the Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following menu items have been referenced :\n
	 * \li \c \b play When clicked, this button starts the playback
	 * \li \c \b pause When clicked, this button pauses the playback
	 * \li \c \b stop When clicked, this button stops the playback
	 * \li \c \b rewind When clicked, this button rewinds the playback
	 * \li \c \b forward When clicked, this button forwards the playback
	 * \li \c \b fullscreen When clicked, this button change the screen mode into full screen mode
	 * \li \c \b metadata_show When clicked, this button shows or hide the metadata viewer
	 * \li \c \b playlist_show When clicked, this button shows or hide the playlist viewer\n \n
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
	 * @brief This method loads all required glade's references to connect the player features. If you add a new player feature to Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following features have been referenced :\n
	 * \li \c \b videoDrawingArea Reference to the Gtk.Gdk.DrawingArea
	 * \li \c \b progress_label Reference to the timecode label
	 * \li \c \b progress_scale Reference to the timecode progressbar \n \n
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
	 * \li \c \b play When connected, the play button sends a signal to start playback through a callback function.
	 * \li \c \b pause When connected, the pause button sends a signal to abort playback through a callback function.
	 * \li \c \b stop When connected, the stop button sends a signal to stop playback through a callback function.
	 * \li \c \b rewind When connected, the rewind button sends a signal to rewind the playback through a callback function.
	 * \li \c \b forward When connected, the forward button sends a signal to forward the playback through a callback function.
	 * \li \c \b metadata_show When connected, the metadata_show button sends a signal to show or hide the metadata viewer through a callback function.
	 * \li \c \b playlist_show When connected, the playlist_show button sends a signal to show or hide the playlist viewer through a callback function. \n \n
	 * @param[in] void : no input parameter.
	 * @return  nothing if all is right or an error at compilation time.
	 * @note This function and his documentation must be completed
	 */
	void connectSignalClicked
	(
		void
	);
	
	/**
	 * @fn void connectSignalActivate(void)
	 * @brief To connect the activated signal to events handler
	 * @brief Each menu item activated is connected with its own signal. If you add a new activated event, you must append its signal connection here.\n \n
	 * @brief Currently, the following activated signals have been connected:\n
	 * \li \c \b openMenuItem When connected, the selection of option open in the menu file of the menubar sends a signal to lauch a filechooser dialog through a callback function.
	 * \li \c \b quitMenuItem When connected, the selection of option quit in the menu file of the menubar sends a signal to close the application through a callback function.
	 * \li \c \b EBUCoreMenuItem When connected, the selection of option EBU-Core in the menu documentation of the menu help of the menubar sends a signal to lauch the EBU-Core documentation through a callback function.
	 * \note Need to be completed.
	 * \li \c \b metadataMenuItem When connected, the selection of option Metadta in the menu view of the menubar sends a signal to show or hide the metadata viewer through a callback function.
	 * \li \c \b playlistMenuItem When connected, the selection of option Playlist in the menu view of the menubar sends a signal to show or hide the playlist viewer through a callback function. \n \n
	 * @param[in] void : No params required
	 * @return nothing if all is right or an error at compilation time.
	 */
	void connectSignalActivate
	(
		void
	);
	
	/**
	 * @fn void connectSignalChangeValue(void)
	 * @brief To connect the signal of each widget where the embedded value can change to the event handler
	 * @brief This method connects the signal of each widget who is able to change its own value in time or in function of any events. If you add a new changed value event, you must append its signal connection here.\n \n
	 * @brief Currently, the following changed value signals have been connected:\n
	 * \li \c \b progress_scale When connected, the progress_scale_label will be updated through a callback function. \n \n
	 * @param[in] void : No params required
	 * @return nothing if all is right or an error at compilation time.
	 */
	void connectSignalChangeValue
	(
		void
	);
	
	/**
	 * @fn void connectSignalRealize(void)
	 * @brief To create and connect the resources associated with a widget (signal handlers)
	 * @brief This method will create and connect the resources associated to any widget. Normally the realization signal should be implicit because automatically mapped at widget creation. Unfortunately, in some cases we need to instantiate first an extern object before to map it into the widget and only after we can realiaze the association.
	 * @param[in] void : No params required
	 * @return nothing if all is right or an error at compilation time.
	 */
	void connectSignalRealize
	(
		void
	);
	
	/**
	 * @fn void on_open_media_activate_from_menubar(void)
	 * @brief To launch the multimedia filechooser
	 * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_open_media_activate_from_menubar
	(
		void
	);
	
	/**
	 * @fn void on_open_remotemedia_activate_from_menubar(void)
	 * @brief To launch the multimedia remote filechooser
	 * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_open_remotemedia_activate_from_menubar
	(
		void
	);
	
	/**
	 * @fn void on_quit_activate_from_menubar(void)
	 * @brief Close the mediaplayer main process
	 * @brief When this function is called, the mediaplayer main window is closed then the application process is destroyed
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_quit_activate_from_menubar
	(
		void
	);
	
	/**
	 * @fn void on_open_MXFGenerator_activate_from_menubar(void)
	 * @brief Opens the MXF Generator window
	 * @brief When this function is called, the MXF OP1a Generator is instantiated and then the MXF generator is opened.
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_open_MXFGenerator_activate_from_menubar
	(
		void
	);
	
	/**
	 * @fn void on_play_clicked(void)
	 * @brief
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	 void on_play_clicked
	 (
	 	void
	 );
	 
	/**
	 * @fn void on_pause_clicked(void)
	 * @brief
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_pause_clicked
	(
		void
	);

	/**
	 * @fn void on_stop_clicked(void)
	 * @brief
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */	
	void on_stop_clicked
	(
		void
	);
	
	/**
	 * @fn void on_fullscreen_clicked(void)
	 * @brief To switch the mediaplayer into fullscreen mode
	 * @brief To switch the mediaplayer from windowed mode to fullscreen mode and from the fullscreen mode to the windowed mode
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_fullscreen_clicked
	(
		void
	);
	
	
	/**
	 * @fn void on_volume_scale_value_changed(Gtk::ScrollType // value
	 ,double value)
	 * @brief To set volume level
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return bool if all is right or an error at compilation time.
	 */
	bool on_volume_scale_value_changed
	(
		Gtk::ScrollType /*type_not_used */, 
		double value
	);
	
	/**
	 * @fn void on_timecode_clicked(void)
	 * @brief Switch between the double timecode
	 * @brief Switch between the double timecode; classical timecode in HHH:MM:SS or by frame number
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_timecode_clicked
	(
		void
	);
	
	/**
	 * @fn bool refresh_timecode(void)
	 * @brief This callback function will refresh the timecode position, the timecode duration and the timeline cursor into the progress bar when a file is in playback mode.
	 * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return bool if all is right or an error at compilation time.
	 */
	bool refresh_timecode
	(
		void
	);
	 
	/**
	 * @fn bool on_timeout_fullscreen(void)
	 * @brief This timeout function is called at a 3000ms interval to hide or show player controls when it is in fullscreen mode.
	 * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return bool if all is right or an error at compilation time.
	 */
	bool on_timeout_fullscreen
	(
		void
	);
	 
	/**
	 * @fn bool on_mousemove(GdkEventMotion* event)
	 * @brief This callback function will detect the mouse moves
	 * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return bool if all is right or an error at compilation time.
	 */
	bool on_mousemove
	(
		GdkEventMotion*  /*event*/
	);
	 
	/**
	 * @fn bool on_timeline_change_value(Gtk::ScrollType scroll, double value)
	 * @brief This callback function will detect when the cursor in the timeline progress bar is moved.
	 * @brief 
	 * @note needs more documentation
	 * @param[in] Gtk::ScrollType scroll : 
	 * @param[in] double value : set by the cursor position
	 * @return bool if all is right or an error at compilation time.
	 */
	bool on_timeline_change_value
	(
		Gtk::ScrollType scroll, 
		double value
	);
	 
	/**
	 * @fn bool on_timeline_button_press_event (GdkEventButton *event)
	 * @brief This callback function will detect when the cursor in the timeline progress bar is pressed.
	 * @brief 
	 * @note needs more documentation
	 * @param[in] GdkEventButton *event : button event
	 * @return bool if all is right or an error at compilation time.
	 */
	bool on_timeline_button_press_event 
	(
		GdkEventButton *event
	);
	 
	/**
	 * @fn bool on_timeline_button_release_event (GdkEventButton *event)
	 * @brief This callback function will detect when the cursor in the timeline progress bar is released.
	 * @brief 
	 * @note needs more documentation
	 * @param[in] GdkEventButton *event : button event
	 * @return bool if all is right or an error at compilation time.
	 */
	bool on_timeline_button_release_event 
	(
		GdkEventButton *event
	);
	
	/**
	 * @fn void on_next_clicked(void)
	 * @brief This callback function will set the next track into the playlist
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_next_clicked
	(
		void
	);
	
	/**
	 * @fn void on_timecode_clicked(void)
	 * @brief This callback function will set the next track into the playlist when the next button is clicked
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_next_clicked_eos
	(
		void
	);
	
	/**
	 * @fn void on_timecode_clicked(void)
	 * @brief This callback function will set the next track into the playlist when the EOS signal is sent by the player engine
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_set_new_track_clicked
	(
		void
	);
	
	/**
	 * @fn void on_timecode_clicked(void)
	 * @brief This callback function will set the previous track into the playlist when the previous button is clicked
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void on_previous_clicked
	(
		void
	);
		
	/**
	* @fn void setShuffle(void)
	* @brief This function will enable or disable the shuffle mode
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void setShuffle
	(
		void
	);
	
	/**
	* @fn void setLoop(void)
	* @brief This function will enable or disable the loop mode
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void setLoop
	(
		void
	);
	
	/**
	* @fn void on_playlistWindow_showing(void)
	* @brief This function will be called when the playlist is visible. It used to activate the toggle button of the playlist.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_playlistWindow_showing
	(
		void
	);
	
	/**
	* @fn void on_playlistWindow_hiding(void)
	* @brief This function will be called when the playlist isn't visible. It used to disable the toggle button of the playlist.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_playlistWindow_hiding
	(
		void
	);
	
	/**
	* @fn void on_playlist_toggled_button(void)
	* @brief This function will be called when the toggle button of the playlist is toggled or un. In function of the toggle button state, the playlist window will be shown or hided.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_playlist_toggled_button
	(
		void
	);
	
	/**
	* @fn void on_playlist_activate(void)
	* @brief This function will be called when we want show or hide the playlist window from the menu bar. In function of the toggle button state, the playlist window will be shown or hided and the toggle button state will be updated.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_playlist_activate
	(
		void
	);
	
	/**
	* @fn void on_playlist_state(void)
	* @brief This callback function will detect when the playlist has files loaded. If any file is present into, then we activate the player controller and set the first file of the playlist.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_playlist_state
	(
		void
	);
	
	/**
	* @fn void on_metadataWindow_showing(void)
	* @brief This function will be called when the ebucore metadata editor is visible. It used to activate the toggle button of the metadata editor/viewer.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_metadata_showing
	(
		void
	);
	
	/**
	* @fn void on_metadataWindow_hiding(void)
	* @brief This function will be called when the ebucore metadata editor isn't visible. It used to disable the toggle button of the metadata editor/viewer.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_metadata_hiding
	(
		void
	);
	
	/**
	* @fn void on_metadataWindow_toggled_button(void)
	* @brief This function will be called when the toggle button of the metadata editor is toggled or un. In function of the toggle button state, the metadata editor window will be shown or hided.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_metadata_toggled_button
	(
		void
	);
	
	/**
	* @fn void on_metadataWindow_activate(void)
	* @brief This function will be called when we want show or hide the metadata editor window from the menu bar. In function of the toggle button state, the metadata window will be shown or hided and the toggle button state will be updated.
	* @brief 
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_metadata_activate
	(
		void
	);
};

#endif
