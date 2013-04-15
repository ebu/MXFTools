/*!
 * \project_name EBU Player
 * \file playlistWindow.cpp
 * \brief playlist window function
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 *
 * This software is published in LGPLv3.0
 *
 */

#include "playlistWindow.hpp"

playlistWindow::playlistWindow
(
	BaseObjectType* cobject,
	const Glib::RefPtr<Gtk::Builder>& refGlade
)
: 
	Gtk::Window(cobject),
	m_refGlade(refGlade)
{
	refGladeButton(); // instantiates the buttons
	refGladeWidget();//  widgets
	connectSignalClicked(); // signal clicked buttons
	initPlaylistStates(); // initalize the playlistWindows states
}

playlistWindow::~playlistWindow
(
	void
) 
{
}

void playlistWindow::refGladeButton
(
	void
) 
{
	/* metadata control buttons */
	m_refGlade->get_widget
	(
		"openPlaylistFile", 
		openPlaylistFile
	);	
	m_refGlade->get_widget
	(
		"savePlaylistFile", 
		savePlaylistFile
	);	
	m_refGlade->get_widget
	(
		"removeFileFromPlaylist", 
		removeFileFromPlaylist
	);	
	m_refGlade->get_widget
	(
		"addFileToPlaylist", 
		addFileToPlaylist
	);	
}

void playlistWindow::refGladeWidget
(
	void
) 
{
	/* instantiate widgets for metadata window*/
	m_refGlade->get_widget
	(
		"playlistScrolledWindow",
		playlistScrolledWindow
	);
}

void playlistWindow::connectSignalClicked(void) {
/*	openPlaylistFile->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistWindow::on_openPlaylistFile_clicked
		)
	);*/
	savePlaylistFile->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistWindow::on_savePlaylistFile_clicked
		)
	);
	removeFileFromPlaylist->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistWindow::on_removeFileFromPlaylist_clicked
		)
	);
/*	addFileToPlaylist->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistWindow::on_addFileToPlaylist_clicked
		)
	); */
	
}

void playlistWindow::initPlaylistStates
(
	void
)
{
	// button to open a playlist file
	openPlaylistFile->set_visible(true);
	openPlaylistFile->set_sensitive(true);
	openPlaylistFile->show();
	// button to save a playlist into a file
	savePlaylistFile->set_visible(true);
	savePlaylistFile->set_sensitive(false);
	savePlaylistFile->show();
	// button states to remove a file from the playlist
	removeFileFromPlaylist->set_visible(true);
	removeFileFromPlaylist->set_sensitive(false);
	removeFileFromPlaylist->show();
	// button states to add a file to the playlist
	addFileToPlaylist->set_visible(true);
	addFileToPlaylist->set_sensitive(true);
	addFileToPlaylist->show();
	// Initial states
	set_visible(false);
	// init popup menu
	initPlaylistMenuPopup();
	// To instanciate and initialize the player engine
	pls = new playlistEngine();
	pls->setRefButton(removeFileFromPlaylist);
	playlistScrolledWindow->add(*pls);
	// boolean
	isShuffle = false;
	isLoop = false;
}

void playlistWindow::initPlaylistMenuPopup
(
	void
)
{
	// Create an action group for the popup menu which has to 
	// be included into the opening playlist button and the adding 
	openPlaylistActionGroup = Gtk::ActionGroup::create();
	appendPlaylistActionGroup = Gtk::ActionGroup::create();
	//Add a new item action menu to the action group
	openPlaylistActionGroup->add(Gtk::Action::create("openPlaylistMenu", "Open Playlist Menu"));
	appendPlaylistActionGroup->add(Gtk::Action::create("appendPlaylistMenu", "Append Playlist Menu"));
	// add new menu items with their own action menu, a keyboard shortcut 
	// and a listener ; open a new playlist or append a playlist to the 
	// current playlist
	openPlaylistActionGroup->add
	(
		Gtk::Action::create
		(
			"openPlaylistMenuNew", 
			Gtk::Stock::NEW, 
			"Open a playlist file", 
			"Close any playlist opened before to open new one..."
		),
		Gtk::AccelKey("<control>O"),
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			true, // close the current playlist before to load the new one
			false // is playlist file
		)
	);
	openPlaylistActionGroup->add
	(
		Gtk::Action::create
		(
			"openPlaylistMenuNewDirectory", 
			Gtk::Stock::NEW, 
			"Open a directory as playlist", 
			"Close any playlist opened before to open a directory as new playlist..."
		),
		Gtk::AccelKey("<control>d"),
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			true, // close the current playlist before to load the new one
			true // is a directory
		)
	);
	appendPlaylistActionGroup->add
	(
		
		Gtk::Action::create
		(
			"appendPlaylistMenuFiles", 
			Gtk::Stock::ADD, 
			"Append files to the playlist", 
			"Add files to the current playlist..."
		),
		Gtk::AccelKey("<control>F"),
		sigc::mem_fun
		(
			*this, 
			&playlistWindow::on_addFileToPlaylist_clicked
		)
	);
	appendPlaylistActionGroup->add
	(
		
		Gtk::Action::create
		(
			"appendPlaylistMenuAppend", 
			Gtk::Stock::ADD, 
			"Append a playlist file", 
			"Append a M3U playlist to the current one..."
		),
		Gtk::AccelKey("<control>A"),
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			false, // append a playlist to the current playlist
			false // is a playlist file
		)
	);
	appendPlaylistActionGroup->add
	(
		Gtk::Action::create
		(
			"appendPlaylistMenuAppendDirectory", 
			Gtk::Stock::NEW, 
			"Append a directory as playlist", 
			"Append a multimedia directory to the current playlist..."
		),
		Gtk::AccelKey("<control>m"),
		sigc::bind<bool,bool>
		(
			sigc::mem_fun
			(
				*this, 
				&playlistWindow::on_openPlaylistFile_clicked
			),
			false, // close the current playlist before to load the new one
			true // is a directory
		)
	);

	openPlaylistUIManager = Gtk::UIManager::create();
	appendPlaylistUIManager = Gtk::UIManager::create();
	openPlaylistUIManager->insert_action_group(openPlaylistActionGroup);
	appendPlaylistUIManager->insert_action_group(appendPlaylistActionGroup);

	add_accel_group(openPlaylistUIManager->get_accel_group());
	add_accel_group(appendPlaylistUIManager->get_accel_group());

	//Layout the actions in a menubar and toolbar:
	Glib::ustring ui_info =
		"<ui>"
		"  <popup name='openPlaylistPopupMenu'>"
		"    <menuitem action='openPlaylistMenuNew'/>"
		"    <menuitem action='openPlaylistMenuNewDirectory'/>"
		"  </popup>"
		"</ui>";

	try
	{
		openPlaylistUIManager->add_ui_from_string(ui_info);
	} catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}
	openPlaylistMenuPopup = dynamic_cast<Gtk::Menu*>
	(
		openPlaylistUIManager->get_widget("/openPlaylistPopupMenu")
	); 
	if(!openPlaylistMenuPopup)
		g_warning("menu not found");

	//Layout the actions in a menubar and toolbar:
	ui_info =
		"<ui>"
		"  <popup name='appendPlaylistPopupMenu'>"
		"    <menuitem action='appendPlaylistMenuFiles'/>"
		"    <menuitem action='appendPlaylistMenuAppend'/>"
		"    <menuitem action='appendPlaylistMenuAppendDirectory'/>"
		"  </popup>"
		"</ui>";

	try
	{
		appendPlaylistUIManager->add_ui_from_string(ui_info);
	} catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}
	appendPlaylistMenuPopup = dynamic_cast<Gtk::Menu*>
	(
		appendPlaylistUIManager->get_widget("/appendPlaylistPopupMenu")
	); 
	if(!appendPlaylistMenuPopup)
		g_warning("menu not found");
		
	// set the popup
	openPlaylistFile->set_popup(*openPlaylistMenuPopup);
	addFileToPlaylist->set_popup(*appendPlaylistMenuPopup);

}

void playlistWindow::on_openPlaylistFile_clicked
(
	bool appendPlaylist,
	bool isFolder
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Open a playlist file...",
		((isFolder) 
			? Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER
			: Gtk::FILE_CHOOSER_ACTION_OPEN
		),
		Glib::get_home_dir(),
		false,
		((isFolder) ? 0 : PLAYLISTMIMETYPE)
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// open the playlist source file
		if (isFolder) 
		{
			pls->setFile(FC->getPathToFiles(), appendPlaylist);
		} else {
			pls->setPlaylist
			(
				FC->getPathToFile(), // path to the new playlist
				appendPlaylist // close and then open or just append
			);
		}
	}
	delete FC;
	savePlaylistFile->set_sensitive(!pls->isEmpty());
	(*loaded).set_active(!pls->isEmpty());
	
}

void playlistWindow::on_savePlaylistFile_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Save the current playlist into a playlist file...",
		Gtk::FILE_CHOOSER_ACTION_SAVE,
		Glib::get_home_dir(),
		false,
		PLAYLISTMIMETYPE
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		pls->writeM3UFile(FC->getPathToM3U());
	}
	delete FC;
}

void playlistWindow::on_removeFileFromPlaylist_clicked
(
	void
)
{
	pls->removeFile();
	// Disable the save button if playlist is not empty
	savePlaylistFile->set_sensitive(!pls->isEmpty());
	(*loaded).set_active(!pls->isEmpty());
}

void playlistWindow::on_addFileToPlaylist_clicked
(
	void
)
{

	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Add a file to the current playlist...",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(), // current directory
		true, // allow multiple selection
		1 // to select video/audio/allFiles
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		pls->setFile(FC->getPathToFiles(), false);
	}
	delete FC;
	// Enable the save button if playlist is not empty
	savePlaylistFile->set_sensitive(!pls->isEmpty());
	(*loaded).set_active(!pls->isEmpty());
}

void playlistWindow::setTrackState
(
	unsigned short state
)
{
	switch (state) {
		case 0 : { pls->setState(Gtk::Stock::MEDIA_STOP); } break;
		case 1 : { pls->setState(Gtk::Stock::MEDIA_PLAY); } break;
		case 2 : { pls->setState(Gtk::Stock::MEDIA_PAUSE); } break;
		default :{} break;
	}
}

void playlistWindow::setLoadState
(
	Gtk::ToggleButton * state
)
{
	loaded = state;
}

void playlistWindow::setEOSState
(
	Gtk::ToggleButton * state
)
{
	EOSState = state;
}

void playlistWindow::setNewTrackState
(
	Gtk::ToggleButton * state
)
{
	pls->setNewTrackState(state);
}

Glib::ustring playlistWindow::getURIToPlay
(
	void
)
{
	return pls->getURItoFile();
}
	
std::string playlistWindow::getNameToPlay
(
	void
)
{
	return pls->getNameOfFile();
}

void playlistWindow::nextTrack
(
	void
) 
{
	pls->nextTrack();
}

bool playlistWindow::isLastTrack
(	
	void
) 
{
	return pls->isLastTrack();
}

void playlistWindow::previousTrack
(	
	void
) 
{
	pls->previousTrack();
}

void playlistWindow::randomTrack
(	
	void
) 
{
	pls->randomTrack();
}

void playlistWindow::setShuffle
(
	bool state
)
{
	isShuffle = state;
}


bool playlistWindow::isShuffling
(
	void
)
{
	return isShuffle;
}

void playlistWindow::setLoop
(
	bool state
)
{
	isLoop = state;
}

bool playlistWindow::isLooping
(
	void
)
{
	return isLoop;
}
