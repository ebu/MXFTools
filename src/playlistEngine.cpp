/*!
 * \project_name EBU Player
 * \file playlistEngine.cpp
 * \brief playlist engine function
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 5 march 2013
 *
 * This software is published in LGPLv3.0
 *
 */

#include "playlistEngine.hpp"

playlistEngine::playlistEngine
(
	void
)
{
	configurePlaylistTreeview();
	show();
}

playlistEngine::~playlistEngine
(
	void
) 
{
}

void playlistEngine::configurePlaylistTreeview
(
	void
)
{
	// create the data model
	playlistStore = Gtk::ListStore::create(playlistColumns);
	// connect signal row added
	/*playlistStore->signal_row_inserted().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistEngine::on_row_added
		)
	);*/
	// connect signal row deleted
	playlistStore->signal_row_deleted().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistEngine::on_row_deleted
		)
	);

	signal_drag_end().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistEngine::on_row_reordered
		)
	);
	// set the data model
	set_model(playlistStore);
	//Add the TreeView's view columns
	append_column
	(
		"ID", 
		playlistColumns.playlistidcol
	);
	append_column
	(
		"", 
		playlistColumns.playlistposcol
	);
	append_column
	(
		"", 
		playlistColumns.playliststatecol
	);
	append_column
	(
		"Name", 
		playlistColumns.playlistnamecol
	);
	append_column
	(
		"Length", 
		playlistColumns.playlistlengthcol
	);
	append_column
	(
		"Format", 
		playlistColumns.playlistformatcol
	);
	append_column
	(
		"Date of creation", 
		playlistColumns.playlistdateofcreationcol
	);
	append_column
	(
		"File URL",
		playlistColumns.playlisturlcol
	);	
	append_column
	(
		"Cell background color", 
		playlistColumns.playlistcellbgcolorcol
	);

	Gtk::TreeView::Column* pColumn;
	for(guint i = 3; i < 7; i++)
	{ 
		pColumn = get_column(i);
		Gtk::CellRenderer* cellRenderer = get_column_cell_renderer(i);
		pColumn->add_attribute
		(
			cellRenderer->property_cell_background(), 
			playlistColumns.playlistcellbgcolorcol
		);
		pColumn->set_reorderable();
		switch(i) 
		{
			case 3 : 
			{
				pColumn->set_sort_column
				(
					playlistColumns.playlistnamecol
				); 
			} break;
			case 4 : 
			{
				pColumn->set_sort_column
				(
					playlistColumns.playlistlengthcol
				);
			} break;
			case 5 :
			{
				pColumn->set_sort_column
				(
					playlistColumns.playlistformatcol
				);
			} break;
			case 6 : 
			{
				pColumn->set_sort_column
				(
					playlistColumns.playlistdateofcreationcol
				);
			} break;
		}
	}
	// hide some columns
	pColumn = get_column(0);
	pColumn->set_visible(false);
	pColumn = get_column(1);
	pColumn->set_visible(false);
	pColumn = get_column(2);
	pColumn->set_visible(true);
	pColumn->set_min_width(32);
	pColumn = get_column(7);
	pColumn->set_visible(false);
	pColumn = get_column(8);
	pColumn->set_visible(false);
	//All the items to be reordered with drag-and-drop
	// Set the visibility state of the headers. 
	set_headers_visible(true);
	// Allow the column title buttons to be clicked
	set_headers_clickable(true);
	// enable the hover effect
	set_hover_selection(true);
	// Resize all columns to their optimal width. 
	columns_autosize();
	// all element are reordorable
	set_reorderable();
	set_rules_hint();
	// grab the treeview selection
	playlistTreeviewSelection = get_selection();
	// allow single selection only
	//playlistTreeviewSelection->set_mode(Gtk::SELECTION_BROWSE);
	// simple clic signal
	signal_button_press_event().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playlistEngine::on_playlist_selection_changed
		), 
		false
	);
	// double clic signal
	signal_row_activated().connect
	(
		sigc::mem_fun
		(
			*this,
			&playlistEngine::on_treeview_row_activated
		) 
	);
	// add the key events mask to the treeview
	//this->add_events(Gdk::KEY_PRESS_MASK || Gdk::KEY_RELEASE_MASK);
	
	signal_key_press_event().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playlistEngine::keyPressed
		)
	);
	signal_key_release_event().connect
	(
		sigc::mem_fun
		(
			*this, 
			&playlistEngine::keyReleased
		)
	);
	
	
    trackToPlay = 0;
	selectedTrack = 0;
}

void playlistEngine::setPlaylist
(
	std::string pathToPlaylist,
	bool reset
) 
{
	// reset the playlist if required
	if (reset)
		playlistStore->clear();
	// open the playlist source file
	std::ifstream playlistFile(pathToPlaylist);
	// read M3Usource
	std::string playlistContent
	(
		std::istreambuf_iterator<char>(playlistFile), //start
		(std::istreambuf_iterator<char>()) //eof
	);

	// locate beginning of the string
	size_t nSearchOffset = 0;
	// locate the first carriage return
	size_t nSubstringOffset = playlistContent.find (NEWLINE, 0);
	// if not EOF, loop
	while (nSubstringOffset != std::string::npos)
	{
		// if next carriage return different of next beginning
		if (nSubstringOffset != nSearchOffset) 
		{
			std::string strline = playlistContent.substr(nSearchOffset,nSubstringOffset-nSearchOffset);
			std::string str = strline.substr
			(
				strline.find_last_of("/")+1,
				strlen(strline.c_str())-strline.find_last_of("/") 
			);
			//Fill the TreeView's model from the end
			Gtk::TreeModel::Row row = *(playlistStore->append());
			// track id
			row[playlistColumns.playlistidcol] = genericFeatures::getTime();
			// track position
			row[playlistColumns.playlistposcol] = getSize();
			if (getSize() == 1) {
				trackToPlay = row[playlistColumns.playlistidcol];
			} 
			// track color
			//row[playlistColumns.playlistcellbgcolorcol] = (row[playlistColumns.playlistposcol]%2==0) ? COLOREVEN : COLORODD;
			// path to the file
			row[playlistColumns.playlisturlcol] = convertIntoRelativePath
				(
					strline, 
					"file://"
				);
			// find last dot in filename "name"
			int lastindex = str.find_last_of(".");
			// copy what comes before last dot	--> file name
			std::string name = str.substr (0,lastindex);
			row[playlistColumns.playlistnamecol] = name;
			row[playlistColumns.playlistlengthcol] = "unknown";
			// find last dot in filename
			lastindex = str.find_last_of(".");
			// copy what comes after last dot	--> file extension
			std::string fileextension = str.substr
			(
				lastindex+1,
				str.size()-(lastindex+1)
			);
			//lowerize the character
			for(int i = 0; fileextension[i] != '\0'; i++)
			{
				fileextension[i] = tolower(fileextension[i]);
			}
			row[playlistColumns.playlistformatcol] = fileextension;
			row[playlistColumns.playlistdateofcreationcol] = "unknown";
		}
		// Make the 'find' function search the next character onwards
		nSearchOffset = nSubstringOffset + 1;
		nSubstringOffset = playlistContent.find
		(
			NEWLINE, 
			nSearchOffset
		);
	}
	colorizePlaylist();
}

void playlistEngine::setFile
(
	std::vector<std::string> file,
	bool reset
) 
{
	// reset the playlist if required
	if (reset)
		playlistStore->clear();
	int countSelected = file.size();
	for (int i=0;i<countSelected;i++) {
		// create path to the file to play
		std::string filename = "file://"+file.at(i);
		//Fill the TreeView's model from the end
		Gtk::TreeModel::Row row = *(playlistStore->append());
		// track id
		row[playlistColumns.playlistidcol] = genericFeatures::getTime();
		// track position
		row[playlistColumns.playlistposcol] = getSize();
		if (getSize() == 1) {
			trackToPlay = row[playlistColumns.playlistidcol];
		} 
		// track color
		//row[playlistColumns.playlistcellbgcolorcol] = (row[playlistColumns.playlistposcol]%2==0) ? COLOREVEN : COLORODD;
		// path to the file
		row[playlistColumns.playlisturlcol] = filename;
		// find last dot in filename "name"
		int lastindex = Glib::filename_display_basename(file.at(i)).find_last_of(".");
		// copy what comes before last dot	--> file name
		std::string name = Glib::filename_display_basename(file.at(i)).substr (0,lastindex);
		row[playlistColumns.playlistnamecol] = name;
		row[playlistColumns.playlistlengthcol] = "unknown";
		// find last dot in filename
		lastindex = filename.find_last_of(".");
		// copy what comes after last dot	--> file extension
		std::string fileextension = filename.substr (lastindex+1,filename.size()-(lastindex+1));
		//lowerize the character
		for(int j = 0; fileextension[j] != '\0'; j++){
			fileextension[j] = tolower(fileextension[j]);
		}
		row[playlistColumns.playlistformatcol] = fileextension;
		row[playlistColumns.playlistdateofcreationcol] = "unknown";
	}
	colorizePlaylist();
}

std::string playlistEngine::writeRelativePath
(
	const std::string& filename, 
	const std::string& path
)
{
	//output relative path
	std::string relativepath;
	// locate beginning of the string
	size_t nSearchOffset = 0;
	// locate the first carriage return
	size_t nSubstringOffset = path.find ("/", 0);
	// locate end of the string
	size_t nFinalOffset = 0;
	bool bFinalOffset = false;
	// if not EOF, loop
	while (nSubstringOffset != std::string::npos)
	{
		// if next carriage return different of next beginning
		if (nSubstringOffset != nSearchOffset)
		{
			std::string strf = filename.substr
			(
				nSearchOffset,
				nSubstringOffset-nSearchOffset
			);
			std::string strp = path.substr
			(
				nSearchOffset,
				nSubstringOffset-nSearchOffset
			);
			if (strf != strp) 
			{
				if (!bFinalOffset) 
				{
					bFinalOffset=true;
					nFinalOffset=nSearchOffset;
				}
				relativepath = relativepath+"../";
			}
		}
        // Make the 'find' function search the next character onwards
        nSearchOffset = nSubstringOffset + 1;
        nSubstringOffset = path.find ("/", nSearchOffset);
	}
	return relativepath+filename.substr
	(
		nFinalOffset,
		strlen(filename.c_str())-nFinalOffset
	);
}

std::string playlistEngine::convertIntoRelativePath
(
	const std::string& filename, 
	const std::string& path
)
{
	std::string relativepath = path+filename;
	size_t previousSearchOffset, nSearchOffset = 0;
	size_t nSubstringOffset = relativepath.find ("../", 0);
	
	while (nSearchOffset < nSubstringOffset-1)
	{
		previousSearchOffset = nSearchOffset+1;
		nSearchOffset = relativepath.find ("/", previousSearchOffset);
	}
	
	while (nSubstringOffset != std::string::npos) 
	{		
		relativepath = relativepath.replace
		(
			previousSearchOffset, 
			nSubstringOffset-previousSearchOffset+3, ""
		);
		// Make the 'find' function search the next character onwards
		nSearchOffset = nSubstringOffset + 1;
		nSubstringOffset = relativepath.find ("../", nSearchOffset);
		nSearchOffset = 0;
		
		while (nSearchOffset < nSubstringOffset-1) 
		{
			previousSearchOffset = nSearchOffset+1;
			nSearchOffset = relativepath.find 
			(
				"/", 
				previousSearchOffset
			);
		}
	}
	return relativepath;
}

void playlistEngine::writeM3UFile
(
	std::string filename
)
{
	std::ofstream myfile;
	myfile.open (filename);
	for (Gtk::TreeNodeChildren::iterator row = 
			playlistStore->children().begin(); 
		row != playlistStore->children().end();
		row++
	)
	{
		Glib::ustring pathtmp = ((*row)[playlistColumns.playlisturlcol]);
		/* Make the replacement. */
		myfile << writeRelativePath
		(
			pathtmp.replace(0, 7, ""), 
			filename
		) << NEWLINE;
	}
	myfile.close();
}

unsigned int playlistEngine::getSize
(
	void
)
{
	return playlistStore->children().size();
}

void playlistEngine::removeFile
(
	void
)
{
	// Browse the playlist to delete the selected files.
	for (Gtk::TreeNodeChildren::iterator row = 
	playlistStore->children().begin(); 
	row != playlistStore->children().end(); row++)
	{
		// if the file is selected
		if ((*row)[playlistColumns.playlistposcol] == selectedTrack) 
		{
			if ((*row)[playlistColumns.playlistidcol] == trackToPlay) {
				trackToPlay = 0;
				// inform that we a change into the played file... 
				(*playlistNewTrack).set_active(!(*playlistNewTrack).get_active());
			}
			playlistStore->erase(*row);
			break;
		}
	}
}


bool playlistEngine::isEmpty
(
	void
)
{
	return (playlistStore->children().size() > 0) ? false : true ;
}


bool playlistEngine::isSelected
(
	void
)
{
	playlistPathList = playlistTreeviewSelection->get_selected_rows();
	return (playlistTreeviewSelection->count_selected_rows() > 0 );
}

bool playlistEngine::hasMultipleSelection
(
	void
)
{
	playlistPathList = playlistTreeviewSelection->get_selected_rows();
	return (playlistTreeviewSelection->count_selected_rows() > 1 );
}

bool playlistEngine::on_playlist_selection_changed
(
	GdkEventButton *event
) 
{
	
	if (event->type == GDK_BUTTON_PRESS && event->button ==1) 
	{
		// grab the list selection
		Gtk::TreeModel::iterator iter = 
			playlistTreeviewSelection->get_selected();
		if (iter) {
			std::cout<<"click....... selection is changing.... selected id : "<<(*iter)[playlistColumns.playlistposcol]<<std::endl;
			//trackToPlay = (*iter)[playlistColumns.playlistidcol];
			selectedTrack = (*iter)[playlistColumns.playlistposcol];
			removeBtn->set_sensitive(true);
			colorizePlaylist();
		} else {
			if (selectedTrack != 0)
			{
				selectedTrack = 0;
				removeBtn->set_sensitive(false);
				colorizePlaylist();
			}
		}
		return false;
	}
	return true;
}
void playlistEngine::on_treeview_row_activated
(
	const Gtk::TreeModel::Path& path,
	Gtk::TreeViewColumn* /* column */
)
{
	Gtk::TreeModel::iterator iter = playlistStore->get_iter(path);
	if(iter)
	{
		if (previousRow && trackToPlay != 0) {
			previousRow[playlistColumns.playliststatecol] = stateNull;
		}
		Gtk::TreeModel::Row row = *iter;
        selectedTrack = row[playlistColumns.playlistposcol];
        trackToPlay = row[playlistColumns.playlistidcol];
		row[playlistColumns.playliststatecol] = render_icon_pixbuf
		(
			Gtk::Stock::MEDIA_PLAY,
			Gtk::ICON_SIZE_LARGE_TOOLBAR
		);
        removeBtn->set_sensitive(true);
        colorizePlaylist();
        // inform that we are at the end of stream
    	(*playlistNewTrack).set_active(!(*playlistNewTrack).get_active());
	}
}

void playlistEngine::on_row_deleted
(
	const Gtk::TreeModel::Path& path
)
{
	selectedTrack = 0;
	removeBtn->set_sensitive(false);
	colorizePlaylist();
}

/*void playlistEngine::on_row_added
(
	const Gtk::TreeModel::Path& path, 
	const Gtk::TreeModel::iterator& iter
)
{ 
	//saveBtn->set_sensitive(!isEmpty());
}*/

void playlistEngine::on_row_reordered
(
	const Glib::RefPtr<Gdk::DragContext>& /*context*/
)
{
	std::cout<<"row reordered"<<std::endl;
	colorizePlaylist(true);
}

bool playlistEngine::keyPressed
(
	GdkEventKey* /* event */ 
)
{
  std::cout << "I'm pushing a keyboard button" << std::endl;

  //Let it propagate:
  return false;
}

bool playlistEngine::keyReleased
(
	GdkEventKey* /* event */ 
)
{
  std::cout << "I'm releasing a keyboard button" << std::endl;

  //Let it propagate:
  return false;
}

void playlistEngine::setRefButton
(
	Gtk::Button * del
) 
{	
	removeBtn = del;
}

void playlistEngine::setState
(
	Gtk::StockID stock_id
)
{
	trackToPlayState = stock_id;
	colorizePlaylist();
}

Glib::ustring playlistEngine::getURItoFile
(
	void
)
{
	Glib::ustring empty;
	for 
	(
		Gtk::TreeNodeChildren::iterator row = 
			playlistStore->children().begin(); 
		row != playlistStore->children().end();
		row++
	)
	{
		if ((*row)[playlistColumns.playlistidcol] == trackToPlay) 
		{
			return (*row)[playlistColumns.playlisturlcol];
		}
	}
	return empty;
}

std::string playlistEngine::getNameOfFile
(
	void
)
{
	Glib::ustring empty;
	for 
	(
		Gtk::TreeNodeChildren::iterator row = 
			playlistStore->children().begin(); 
		row != playlistStore->children().end();
		row++
	)
	{
		if ((*row)[playlistColumns.playlistidcol] == trackToPlay) 
		{
			return (*row)[playlistColumns.playlistnamecol];
		}
	}
	return empty;
}

void playlistEngine::nextTrack
(
	void
)
{
	// compute the next position to play
	position%=getSize();
	// select the row to play and iterate the position
	Gtk::TreeModel::Row row = playlistStore->children()[position++];
	// set the track to play
	trackToPlay = row[playlistColumns.playlistidcol];
	// udpate colors
	colorizePlaylist();
}

void playlistEngine::randomTrack
(
	void
)
{
	/* initialize random seed: */
	srand ( time(NULL) );
	// compute the next position to play
	position=rand()%getSize();
	// select the row to play and iterate the position
	Gtk::TreeModel::Row row = playlistStore->children()[position++];
	// set the track to play
	trackToPlay = row[playlistColumns.playlistidcol];
	// udpate colors
	colorizePlaylist();
}

void playlistEngine::previousTrack
(
	void
)
{
	// compute the next position to play
	position--;
	// select the row to play and iterate the position
	Gtk::TreeModel::Row row = playlistStore->children()[(position-1<0) ? getSize()-1 : position-1];
	// set the track to play
	trackToPlay = row[playlistColumns.playlistidcol];
	// udpate colors
	colorizePlaylist();
}

bool playlistEngine::isLastTrack
(
	void
)
{
	return (position%getSize() == 0) ? true : false;
}

void playlistEngine::setNewTrackState
(
	Gtk::ToggleButton * state
)
{
	playlistNewTrack = state;
}

void playlistEngine::colorizePlaylist
(
	bool reordered
)
{
	unsigned int i = 0;
	for (
			Gtk::TreeNodeChildren::iterator row = 
				playlistStore->children().begin(); 
			row != playlistStore->children().end();
			row++
		) 
	{
		if ((*row)[playlistColumns.playlistposcol] != ++i) {
			(*row)[playlistColumns.playlistposcol] = i;
		}
		// recompute the row color
		if ((*row)[playlistColumns.playlistidcol] == trackToPlay) 
		{
			// update position to play
			position = (*row)[playlistColumns.playlistposcol];
			// update color
			(*row)[playlistColumns.playlistcellbgcolorcol] = COLORPLAY;
			// update icon status
			(*row)[playlistColumns.playliststatecol] = render_icon_pixbuf
			(
				trackToPlayState,
				Gtk::ICON_SIZE_SMALL_TOOLBAR
			);
			//update previous row
			previousRow = (*row);
		}
		else if ((*row)[playlistColumns.playlistposcol] == selectedTrack) 
		{
			(*row)[playlistColumns.playlistcellbgcolorcol] = COLORSELECTED;
			(*row)[playlistColumns.playliststatecol] = previousState;
		}
		else if ((*row)[playlistColumns.playlistposcol]%2==0)  
		{
			(*row)[playlistColumns.playlistcellbgcolorcol] = COLOREVEN;
			(*row)[playlistColumns.playliststatecol] = previousState;
		}
		else
		{
			(*row)[playlistColumns.playlistcellbgcolorcol] = COLORODD;
			(*row)[playlistColumns.playliststatecol] = previousState;
		}
	}
}
