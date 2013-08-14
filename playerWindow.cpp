/*!
 * \project_name EBU Player
 * \file playerWindow.cpp
 * \brief player window
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 *
 * This software is published in LGPLv3.0
 *
 */


#include "playerWindow.hpp"

// class constructor and destructor

playerWindow::playerWindow
(
	BaseObjectType* cobject, 
	const Glib::RefPtr<Gtk::Builder>& refGlade
) 
	: 
		Gtk::Window(cobject),
		m_refGlade(refGlade)
{
	// To instanciate and initialize the player engine
	gst = new playerEngine();
	
	// To link the Glade references
	refGladeMenu(); // for menus
	refGladeButton(); // for buttons
	refGladeWidget(); // for any widgets;
	
	// To connect the signals
	connectSignalClicked(); // clicked 
	connectSignalActivate(); // activate
	connectSignalChangeValue(); // changeValue
	connectSignalRealize(); // realize
	
	// To initialize all Gtk widgets and mediaplayer's variables
	initMPStates();
	
}

playerWindow::~playerWindow
(
	void
)
{
}

// class public functions


// class protected functions

void playerWindow::initMPStates
(
	void
) 
{

	///////////////////////////////////
	//  mediaplayer state variables  //
	///////////////////////////////////
	
	// fullscreen state
	isFullscreen = false;
	// timecode status
	timecodeState = false;
	// playlist load state
	playlistIsLoaded.set_active(false);
	playlistIsLoaded.signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_playlist_state
		)
	);
	playlist->setLoadState(&playlistIsLoaded);
	gst->setLoadState(&playlistIsLoaded);
	// EOS Message for playlist
	EOSState.set_active(false);
	EOSState.signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_next_clicked_eos
		)
	);
	playlist->setEOSState(&EOSState);
	gst->setEOSState(&EOSState);
	
	playlistNewTrack.set_active(false);
	playlistNewTrack.signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_set_new_track_clicked
		)
	);
	playlist->setNewTrackState(&playlistNewTrack);
	///////////////////////////////////
	// mediaplayer classical buttons //
	///////////////////////////////////
	
	// play
	play->set_visible(true);
	play->set_sensitive(false);
	play->show();
	// pause
	pause->set_sensitive(true);
	pause->set_sensitive(true);
	pause->hide();
	// stop
	stop->set_visible(true);
	stop->set_sensitive(false);
	stop->show();
	// forward
	forward->set_visible(true);
	forward->set_sensitive(false);
	forward->show();
	// rewind
	rewind->set_visible(true);
	rewind->set_sensitive(false);
	rewind->show();
	// unfullscreen
	unfullscreenBtn->set_visible(false);
	unfullscreenBtn->set_sensitive(true);
	unfullscreenBtn->hide();
	// fullscreen
	fullscreenBtn->set_visible(true);
	fullscreenBtn->set_sensitive(true);
	fullscreenBtn->show();

	//////////////////////////////////
	// mediaplayer advance buttons  //
	//////////////////////////////////
	
	// buttons to interact with the playlist
	
	// next
	next->set_visible(true);
	next->set_sensitive(false);
	next->show();
	// previous
	previous->set_visible(true);
	previous->set_sensitive(false);
	previous->show();
	
	// trackoption_show
	//trackoption_show->set_visible(true);
	//trackoption_show->set_sensitive(false);
	//trackoption_show->show();
	// metadata_show
	metadata_show->set_visible(true);
	metadata_show->set_sensitive(true);
	metadata_show->set_active(false);
	metadata_show->show();
	// playlist_show
	playlist_show->set_visible(true);
	playlist_show->set_sensitive(true);
	playlist_show->set_active(false);
	playlist_show->show();
	// loop
	loop->set_visible(true);
	loop->set_sensitive(true);
	loop->set_active(false);
	loop->show();
	// shuffle
	shuffle->set_visible(true);
	shuffle->set_sensitive(true);
	shuffle->show();
	// timecode's widget configurations
	Glib::ustring visualMark; // f.....' mark !
	// set some progress_scale properties
	progress_scale->set_slider_size_fixed (true);
	progress_scale->set_min_slider_size (true);
	progress_scale->set_range(0.0, 1.0);
	progress_scale->set_show_fill_level(true);
	progress_scale->set_restrict_to_fill_level(true);
	progress_scale->set_sensitive(false);
	// classical in HHH:MM:SS
	progress_label->set_visible(true);
	progress_total_label->set_visible(true);
	progress_label->set_sensitive(true);
	progress_total_label->set_sensitive(true);
	// by frame number
	frame_label->set_visible(false);
	frame_total_label->set_visible(false);
	frame_label->set_sensitive(true);
	frame_total_label->set_sensitive(true);
	
	// set the volume range and initial value
	volume_scale->set_range(0.0, 2.0);
	volume_scale->set_value(0.8);
	volume_scale->set_fill_level(0.8);
	// set some visual marks
	volume_scale->add_mark(0.0, Gtk::POS_TOP, visualMark);
	volume_scale->add_mark(1.0, Gtk::POS_TOP, visualMark);
	volume_scale->add_mark(2.0, Gtk::POS_TOP, visualMark);

	// initialize the background color of the drawing area
	videoDrawingArea->override_background_color(Gdk::RGBA("black"));
	videoDrawingArea->show();
	
	// mediaplayer window initial position
	move(0,0);
	
}

void playerWindow::refGladeMenu
(
	void
) 
{
	/* menubar */
	m_refGlade->get_widget(
		"menubar",
		menubar
	);
	
	/* menu _File */
	m_refGlade->get_widget
	(
		"openMenuItem", 
		openMenuItem
	);
	m_refGlade->get_widget
	(
		"openRemoteMenuItem", 
		openRemoteMenuItem
	);
	m_refGlade->get_widget
	(
		"openNewPlaylistMenuItem",
		openNewPlaylistMenuItem
	);
	m_refGlade->get_widget
	(
		"openNewDirPlaylistMenuItem",
		openNewDirPlaylistMenuItem
	);
	m_refGlade->get_widget
	(
		"openAppendPlaylistMenuItem",
		openAppendPlaylistMenuItem
	);
	m_refGlade->get_widget
	(
		"openAppendDirPlaylistMenuItem",
		openAppendDirPlaylistMenuItem
	);
	m_refGlade->get_widget
	(
		"quitMenuItem", 
		quitMenuItem
	);
	
	/* menu _View */
	m_refGlade->get_widget
	(
		"metadataMenuItem",
		metadataMenuItem
	);
	m_refGlade->get_widget
	(
		"playlistMenuItem",
		playlistMenuItem
	);
	
	/* menu _Tools */
	/*m_refGlade->get_widget
	(
		"TranscoderMenuItem",
		TranscoderMenuItem
	);*/
	/*m_refGlade->get_widget
	(
		"MXFAnalyserMenuItem",
		MXFAnalyserMenuItem
	);*/
	m_refGlade->get_widget
	(
		"MXFGeneratorMenuItem",
		MXFGeneratorMenuItem
	);
	
	/* menu _Settings */

	
	/* menu _Help */
	//m_refGlade->get_widget("EBUCoreMenuItem",EBUCoreMenuItem);
}

void playerWindow::refGladeButton
(
	void
) 
{
	// button for control player
	m_refGlade->get_widget
	(
		"play", 
		play
	);
	m_refGlade->get_widget(
		"pause", 
		pause
	);
	m_refGlade->get_widget
	(
		"stop", 
		stop
	);
	m_refGlade->get_widget
	(
		"rewind", 
		rewind
	);
	m_refGlade->get_widget
	(
		"forward", 
		forward
	);
	m_refGlade->get_widget
	(
		"previous", 
		previous
	);
	m_refGlade->get_widget
	(
		"next", 
		next
	);
	m_refGlade->get_widget
	(
		"fullscreen", 
		fullscreenBtn
	);
	m_refGlade->get_widget
	(
		"unfullscreen", 
		unfullscreenBtn
	);
	// button to open a window
	m_refGlade->get_widget
	(
		"playlist_show", 
		playlist_show
	);
	m_refGlade->get_widget
	(
		"metadata_show", 
		metadata_show
	);
	// playlist shuffle
	m_refGlade->get_widget
	(
		"shuffle", 
		shuffle
	);
	// playlist loop
	m_refGlade->get_widget
	(
		"loop", 
		loop
	);
}

void playerWindow::refGladeWidget
(
	void
) 
{
	/* instantiate the display interface */
	m_refGlade->get_widget
	(
		"videoDrawingArea", 
		videoDrawingArea
	);
	
	/* instantiate the timecode features*/
	m_refGlade->get_widget
	(
		"progress_label", 
		progress_label
	);
	m_refGlade->get_widget
	(
		"frame_label", 
		frame_label
	);
	m_refGlade->get_widget
	(
		"progress_total_label", 
		progress_total_label
	);
	m_refGlade->get_widget
	(
		"frame_total_label", 
		frame_total_label
	);
	m_refGlade->get_widget
	(
		"progress_scale", 
		progress_scale
	);
	
	/* instantiate the volume features*/
	m_refGlade->get_widget
	(
		"volume_scale", 
		volume_scale
	);
	
	/* player border */
	m_refGlade->get_widget
	(
		"borderleft", 
		borderleft
	);
	m_refGlade->get_widget
	(
		"borderright", 
		borderright
	);
	
	/* gtk box for timecode/timeline */
	m_refGlade->get_widget
	(
		"timeline_box", 
		timeline_box
	);
	
	/* gtk box for player controls */
	m_refGlade->get_widget
	(
		"player_control_box", 
		player_control_box
	);
	
	/* gtk box for status bar */
	m_refGlade->get_widget
	(
		"status_box", 
		status_box
	);
	
	/* gtk windows */
	m_refGlade->get_widget_derived
	(
		"PLAYLIST-EBU-MXF-ENCODER", 
		playlist
	);
	playlist->signal_show().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_playlistWindow_showing
		)
	);
	playlist->signal_hide().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_playlistWindow_hiding
		)
	);//////////////////////////////////////////////////////////////////
	m_refGlade->get_widget_derived
	(
		"METADATAVIEW-EBU-MXF-ENCODER", 
		metadata
	);
	metadata->signal_show().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_metadata_showing
		)
	);
	metadata->signal_hide().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_metadata_hiding
		)
	);
}

void playerWindow::connectSignalClicked
(
	void
) 
{
	play->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_play_clicked
		)
	);
	pause->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_pause_clicked
		)
	);
	stop->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_stop_clicked
		)
	);
	rewind->signal_pressed().connect
	(
		sigc::mem_fun
		(
			gst, 
			&playerEngine::rewind
		)
	);
	rewind->signal_released().connect
	(
		sigc::mem_fun
		(
			gst, 
			&playerEngine::rewind
		)
	);
	forward->signal_pressed().connect
	(
		sigc::mem_fun
		(
			gst, 
			&playerEngine::forward
		)
	);
	forward->signal_released().connect
	(
		sigc::mem_fun
		(
			gst, 
			&playerEngine::forward
		)
	);
	fullscreenBtn->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_fullscreen_clicked
		)
	);
	unfullscreenBtn->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_fullscreen_clicked
		)
	);

	// connect all signals for timecode	
	progress_label->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_timecode_clicked
		)
	);
	frame_label->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_timecode_clicked
		)
	);
	progress_total_label->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_timecode_clicked
		)
	);
	frame_total_label->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_timecode_clicked
		)
	);
	playlist_show->signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_playlist_toggled_button
		)
	);
	metadata_show->signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_metadata_toggled_button
		)
	);

	// connect all signals for the playlist	
	next->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_next_clicked
		)
	);
	previous->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_previous_clicked
		)
	);
	shuffle->signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::setShuffle
		)
	);
	loop->signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::setLoop
		)
	);
	
}

void playerWindow::connectSignalActivate
(
	void
) 
{

	// Menu File
	openMenuItem->signal_activate().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_open_media_activate_from_menubar
		)
	);
	openRemoteMenuItem->signal_activate().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_open_remotemedia_activate_from_menubar
		)
	);
	openNewPlaylistMenuItem->signal_activate().connect
	(
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				playlist, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			true, // close the current playlist before to load the new one
			false // is a playlist file
		)
	);
	openNewDirPlaylistMenuItem->signal_activate().connect
	(
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				playlist, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			true, // close the current playlist before to load the directory
			true // is a directory
		)
	);
	openAppendPlaylistMenuItem->signal_activate().connect
	(
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				playlist, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			false, // append a playlist to the current playlist
			false // is a playlist file
		)
	);
	openAppendDirPlaylistMenuItem->signal_activate().connect
	(
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				playlist, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			false, // append a directory to the current playlist
			true // is a directory
		)
	);
	quitMenuItem->signal_activate().connect
	(
		sigc::mem_fun(
			*this,
			&playerWindow::on_quit_activate_from_menubar
		)
	);
	
	// Menu View
	playlistMenuItem->signal_activate().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_playlist_activate
		)
	);
	metadataMenuItem->signal_activate().connect
	(
		sigc::mem_fun
		(
			*this,
			&playerWindow::on_metadata_activate
		)
	);
	
	// Menu Tools
	MXFGeneratorMenuItem->signal_activate().connect(
		sigc::mem_fun(
			*this,
			&playerWindow::on_open_MXFGenerator_activate_from_menubar
		)
	);
	
	// Menu Settings
	
	// Menu Help
}

void playerWindow::connectSignalChangeValue
(
	void
) 
{
	progress_scale->signal_button_press_event().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_timeline_button_press_event
		),
		false
	);
	progress_scale->signal_button_release_event().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_timeline_button_release_event
		),
		false
	);
	progress_scale->signal_change_value().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playerWindow::on_timeline_change_value
		)
	);	
	volume_scale->signal_change_value().connect
	(
    	sigc::mem_fun
    	(
    		*this, 
    		&playerWindow::on_volume_scale_value_changed
    	) 
    );
}

void playerWindow::connectSignalRealize
(
	void
) 
{
	/* The "realize" signal gonna map the gstreamer playout into 
	the video area (Gtk::DrawingArea)*/
	videoDrawingArea->signal_realize().connect
	(
		sigc::bind<Gtk::DrawingArea*>
		(
			sigc::mem_fun
			(
				gst, 
				&playerEngine::on_videoDrawingArea_realize
			) ,
			videoDrawingArea
		)
	);
}

bool playerWindow::on_volume_scale_value_changed
(
	Gtk::ScrollType /*type_not_used */, 
	double value
) 
{
	if (value>=0.0 && value<=2.0) {
		volume_scale->set_value(value);
		volume_scale->set_fill_level(value);
		gst->setVolume(value);
	}
	return true;
}

void playerWindow::on_open_media_activate_from_menubar
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select a media file",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		PLAYERMIMETYPE
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		//timer is still alive, kill it
		if (timeout_timecode.connected()) {
			// stop previous playing file in any case
			on_stop_clicked();
			gst->setTimecodePosition(0.0);
			refresh_timecode();
			timeout_timecode.disconnect();
		}
		// set the media file
		gst->setFile
		(
			FC->getURIToFile(),
			Glib::filename_display_basename(FC->getPathToFile())
		);
		// activate buttons
		play->set_sensitive(true);
		play->set_visible(true);
		pause->set_sensitive(true);
		pause->set_visible(false);
		progress_scale->set_sensitive(true);
		// connect the timer to refresh timecode if required
		if (!timeout_timecode.connected()) {
			timeout_timecode =  Glib::signal_timeout().connect
			(
				sigc::mem_fun
				(
					*this, 
					&playerWindow::refresh_timecode
				), 
				100
			);
			// set the file title
			set_title(gst->getTitle());
		}
	}
	delete FC;
}

void playerWindow::on_open_remotemedia_activate_from_menubar
(
	void
)
{
	// instanciate a new filechooser
	remoteStreamFileChooser * RFC = new remoteStreamFileChooser
		(
			*this,
			"Remote media filechooser...",
			"Type the path to the remote stream :",
			false
		);
	
	if (RFC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the media file
		gst->setFile
		(
			RFC->getURL(),
			Glib::filename_display_basename(RFC->getURL())
		);
		//timer is still alive 
		if (timeout_timecode.connected()) {
			timeout_timecode.disconnect();
		}
		// activate buttons
		play->set_sensitive(true);
		play->set_visible(true);
		pause->set_sensitive(true);
		pause->set_visible(false);
		progress_scale->set_sensitive(true);
	}
	delete RFC;
}

void playerWindow::on_quit_activate_from_menubar
(
	void
)
{
  hide();
}

void playerWindow::on_open_MXFGenerator_activate_from_menubar
(
	void
)
{
  m_refGlade->get_widget_derived("EBU-MP-MXFGENERATOR", MXFGenWindow);
  MXFGenWindow->show();
}

// button callbacks

void playerWindow::on_play_clicked
(
	void
)
{
	// set the pipeline to play mode
	gst->setState(Gst::STATE_PLAYING);
	gst->setVolume(volume_scale->get_value());
	//Change the UI appropriately if required
	if (gst->isState(Gst::STATE_PLAYING)) {
		progress_scale->set_sensitive();
		stop->set_sensitive();
		rewind->set_sensitive();
		forward->set_sensitive();
		play->hide();
		pause->show();
		// connect the timer to refresh timecode if required
		if (!timeout_timecode.connected()) {
			timeout_timecode =  Glib::signal_timeout().connect
			(
				sigc::mem_fun
				(
					*this, 
					&playerWindow::refresh_timecode
				), 
				100
			);
			// set the file title
			set_title(gst->getTitle());
		}
		// set the track state
		if (playlistIsLoaded.get_active()) {
			playlist->setTrackState(1);
		}
	} else {
		play->set_sensitive(false);
		set_title("EBU Media Player");
	}
}

void playerWindow::on_pause_clicked
(
	void
)
{
	// Set the pipeline to pause mode
	gst->setState(Gst::STATE_PAUSED);
	// change the UI appropriately
	pause->hide();
	play->show();
	// set the track state
	if (playlistIsLoaded.get_active()) {
		playlist->setTrackState(2);
	}
}

void playerWindow::on_stop_clicked
(
	void
)
{
	// Set the pipeline to inactive mode
	gst->setState(Gst::STATE_NULL);
	// reset the timecode
	gst->setTimecodePosition(0.0);
	refresh_timecode();
	// Change the UI appropriately
	progress_scale->set_sensitive(false);
	stop->set_sensitive(false);
	rewind->set_sensitive(false);
	forward->set_sensitive(false);
	pause->hide();
	play->show();
	set_title("EBU Media Player");
	// disconnect the timecode's timer
	timeout_timecode.disconnect();
	// set the track state
	if (playlistIsLoaded.get_active()) {
		playlist->setTrackState(0);
	}
}

void playerWindow::on_fullscreen_clicked
(
	void
)
{
	// hide all useless stuff for fullscreen mode
	menubar->set_visible(!menubar->get_visible());
	borderleft->set_visible(!borderleft->get_visible());
	borderright->set_visible(!borderright->get_visible());
	// show/hide window decoration 
	set_decorated(!get_decorated());
	// switch between the fullscreen and the unfullscreen button
	unfullscreenBtn->set_visible(!unfullscreenBtn->get_visible());
	fullscreenBtn->set_visible(!fullscreenBtn->get_visible());
	// switch between fullscreen and a decorated window 
	(get_decorated()) ? unmaximize () : maximize ();
	(get_decorated()) ? unfullscreen() : fullscreen();
	// set the fullscreen state
	isFullscreen = !isFullscreen;
	/* Call timeout function at a 3000ms interval to hide or show 
	player controls when it is in fullscreen mode. When the window 
	is no more in the fullscreen mode, timeout signals are
	disconnected motion detector */
	if (isFullscreen) { 
		// set the timer to call periodically the timeout function
		timeout_fullscreen =  Glib::signal_timeout().connect
		(
			sigc::mem_fun
			(
				*this, 
				&playerWindow::on_timeout_fullscreen
			), 
			3000
		);
		// set a motion detector into the drawing area
		mouse_motion_signal_drawingArea = 
			videoDrawingArea->signal_motion_notify_event().connect
		(
			sigc::mem_fun
			(
				*this, 
				&playerWindow::on_mousemove
			)
		);
		/* set the focus into the drawing area. Without the focus, 
		the motion detection doesn't occurs when a movie is playing*/
		videoDrawingArea->set_can_focus
		(
			!videoDrawingArea->get_can_focus()
		);
		// next trigger to attempt to hide the mediaplayer controler
		nextmotiondetection = time(NULL)+3;
	} else {
		// disconnect signals
		timeout_fullscreen.disconnect() ;
		mouse_motion_signal_drawingArea.disconnect();
		// remove the focus
		videoDrawingArea->set_can_focus(!videoDrawingArea->get_can_focus());
	}
}

void playerWindow::on_timecode_clicked
(
	void
) 
{
	progress_label->set_visible(!progress_label->get_visible());
	progress_total_label->set_visible(!progress_total_label->get_visible());
	frame_label->set_visible(!frame_label->get_visible());
	frame_total_label->set_visible(!frame_total_label->get_visible());
	timecodeState = !timecodeState;
}

bool playerWindow::refresh_timecode
(
	void
)
{
	// get the timecode duration and position
	gint64 pos = gst->getCurrentTimecode();
	gint64 len = gst->getDuration();
	// set the progress scale with the proper values
	progress_scale->set_value(double(pos)/len);
	progress_scale->set_fill_level(double(pos)/len);
	// convert the timecode into a readable data
	std::ostringstream locationStream (std::ostringstream::out);
	std::ostringstream durationStream (std::ostringstream::out);
	if (!timecodeState) {
		locationStream << std::right << std::setfill('0') << 
		  std::setw(3) << Gst::get_hours(pos) << ":" <<
		  std::setw(2) << Gst::get_minutes(pos) << ":" <<
		  std::setw(2) << Gst::get_seconds(pos);/*<< "." <<
		  std::setw(9) << std::left << Gst::get_fractional_seconds(pos);*/

		durationStream << std::right << std::setfill('0') <<
		  std::setw(3) << Gst::get_hours(len) << ":" <<
		  std::setw(2) << Gst::get_minutes(len) << ":" <<
		  std::setw(2) << Gst::get_seconds(len); /*<< "." <<
		  std::setw(9) << std::left << Gst::get_fractional_seconds(len);
	*/
		progress_label->set_label(locationStream.str());
		progress_total_label->set_label(durationStream.str());
	} else {
		// format the frame duration
		durationStream << std::left << len;
		// format the current frame position 
		locationStream << std::right << std::setfill('0')  <<std::setw(durationStream.str().size())<<pos;
		// remove the last 3 zeros
		frame_label->set_label
		(
			locationStream.str().substr (0,locationStream.str().size()-4)
		);
		frame_total_label->set_label
		(
			durationStream.str().substr (0,durationStream.str().size()-4)
		);
	}
	return true;
}

bool playerWindow::on_timeline_change_value
(
	Gtk::ScrollType scroll, 
	double value
)
{
	// if we tolerate values 0 and bigger or equal to 1, 
	// it can crash the player engine with some multimedia files
	// Warning message : GStreamer-WARNING **: wrong STREAM_LOCK count 0
	if (value>0.0 && value<1.0) {
		// set the progress scale value
		progress_scale->set_value(value);
		progress_scale->set_fill_level(value);
		// set the new timecode position into the player engine
		gst->setTimecodePosition(value);
	}
	return true;
}

bool playerWindow::on_timeline_button_press_event 
(
	GdkEventButton *event
)
{
	if (event->type == GDK_BUTTON_PRESS)
		previousState = ((gst->getState()!= Gst::STATE_NULL) 
			? gst->getState() 
			: Gst::STATE_PAUSED
		); 
		gst->setState(Gst::STATE_PAUSED);
	return false;
}
 
bool playerWindow::on_timeline_button_release_event 
(
	GdkEventButton *event
)
{
	if (event->type == GDK_BUTTON_RELEASE) 
		gst->setState(previousState);
	return false;
}

bool playerWindow::on_timeout_fullscreen
(
	void
)
{
  if (timeline_box->get_visible() && player_control_box->get_visible() && nextmotiondetection <= time(NULL)) {
		timeline_box->set_visible(false);
		player_control_box->set_visible(false);
		status_box->set_visible(false);
	}
	return true;
}

bool playerWindow::on_mousemove
(
	GdkEventMotion* /*event*/
) 
{
	if (!timeline_box->get_visible() and !player_control_box->get_visible()) {
		timeline_box->set_visible(true);
		player_control_box->set_visible(true);
		status_box->set_visible(true);
	}
	// time before hide the player controls
	nextmotiondetection = time(NULL)+3;
	return true; //It has been handled.
}

void playerWindow::on_next_clicked
(
	void
)
{
	// stop the current playback
	on_stop_clicked();
	// set the next track
	playlist->nextTrack();
	// set the new track to play
	gst->setFile
	(
		playlist->getURIToPlay(),
		playlist->getNameToPlay()
	);
	// start playback
	on_play_clicked();
}

void playerWindow::on_next_clicked_eos
(
	void
)
{
	// stop the current playback
	on_stop_clicked();	
	// set the track state
	if (playlistIsLoaded.get_active()) {
		// loop/shuffle enable or disable
		if (playlist->isShuffling()) { // shuffle bypass the loop mode
		 	// set randomly the next track
		 	playlist->randomTrack();
			// set the new track to play
			gst->setFile
			(
				playlist->getURIToPlay(),
				playlist->getNameToPlay()
			);
			// start playback
			on_play_clicked();
		 } else if 
		 (
	 		playlist->isLooping() || 
	 		(!playlist->isShuffling() && !playlist->isLastTrack())
		 ) 
		 {
			// set the next track
			playlist->nextTrack();
			// set the new track to play
			gst->setFile
			(
				playlist->getURIToPlay(),
				playlist->getNameToPlay()
			);
			// start playback
			on_play_clicked();
		 } else {
		 	// set the next track, in this case the first track but
		 	// don't play it
			playlist->nextTrack();
			// set the new track to play
			gst->setFile
			(
				playlist->getURIToPlay(),
				playlist->getNameToPlay()
			);
			// reset the timecode
			refresh_timecode();
			// disconnect the timecode's timer
			timeout_timecode.disconnect();
		 }
	}
}

void playerWindow::on_set_new_track_clicked
(
	void
)
{
	// stop the current playback
	on_stop_clicked();	
	// set the new track to play
	gst->setFile
	(
		playlist->getURIToPlay(),
		playlist->getNameToPlay()
	);
	// start playback
	on_play_clicked();
}

void playerWindow::on_previous_clicked
(
	void
)
{
	// stop the current playback
	on_stop_clicked();
	// set the previous track
	playlist->previousTrack();
	// set the new track to play
	gst->setFile
	(
		playlist->getURIToPlay(),
		playlist->getNameToPlay()
	);
	// start playback
	on_play_clicked();
}

void playerWindow::on_playlistWindow_hiding
(
	void
)
{
	playlist_show->set_active(false);
}

void playerWindow::on_playlistWindow_showing
(
	void
)
{
	playlist_show->set_active(true);
}

void playerWindow::on_playlist_toggled_button
(
	void
)
{
	playlist->set_visible(playlist_show->get_active());
}

void playerWindow::on_playlist_activate
(
	void
)
{
	playlist->set_visible(!playlist_show->get_active());
}

void playerWindow::on_playlist_state
(
	void
)
{
	// activate or disactivate the player controllers 
	play->set_sensitive(playlistIsLoaded.get_active());
	pause->set_sensitive(playlistIsLoaded.get_active());
	stop->set_sensitive(playlistIsLoaded.get_active());
	forward->set_sensitive(playlistIsLoaded.get_active());
	rewind->set_sensitive(playlistIsLoaded.get_active());
	next->set_sensitive(playlistIsLoaded.get_active());	
	previous->set_sensitive(playlistIsLoaded.get_active());
	// set the media file
	gst->setFile
	(
		playlist->getURIToPlay(),
		playlist->getNameToPlay()
	);
	//timer is still alive, disconnect it
	if (timeout_timecode.connected()) {
		timeout_timecode.disconnect();
	}
}



void playerWindow::setShuffle
(
	void
)
{
	playlist->setShuffle(shuffle->get_active());
}

void playerWindow::setLoop
(
	void
)
{

	playlist->setLoop(loop->get_active());
}

void playerWindow::on_metadata_hiding
(
	void
)
{
	metadata_show->set_active(false);
}

void playerWindow::on_metadata_showing
(
	void
)
{
	metadata_show->set_active(true);
}

void playerWindow::on_metadata_toggled_button
(
	void
)
{
	metadata->set_visible(metadata_show->get_active());
}

void playerWindow::on_metadata_activate
(
	void
)
{
	metadata->set_visible(!metadata_show->get_active());
}
