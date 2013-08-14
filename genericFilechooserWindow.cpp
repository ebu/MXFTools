/*!
 * \project_name EBU Player
 * \file genericFilechooserWindow.cpp
 * \brief Generic filechooser window functions
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 4 march 2013
 *
 * This software is published in LGPLv3.0
 *
 */

#include "genericFilechooserWindow.hpp"

genericFilechooserWindow::genericFilechooserWindow
(
	Gtk::Window& parent, 
	const Glib::ustring& title, 
	Gtk::FileChooserAction action, 
	Glib::ustring currentFolder,
	bool multipleSelection,
	int mimeType
) 
: 
	 Gtk::FileChooserDialog (parent, title, action)
{
	// append filechooser buttons and linked events
	add_button
	(
		Gtk::Stock::CANCEL, 
		Gtk::RESPONSE_CANCEL
	);
	add_button
	(
		Gtk::Stock::OK, 
		Gtk::RESPONSE_OK
	);
	// set the path to folder of the application as current folder
	set_current_folder(currentFolder);
	// Allow the selection of several files
	set_select_multiple(multipleSelection);
	// ask confirmation before overwrite an existing file
	if (action == Gtk::FILE_CHOOSER_ACTION_SAVE)
		set_do_overwrite_confirmation(true);
	// mime type file filter
	setFilters(mimeType);
	// run the filechooser and grab the result
	response = run();
	// alert dialog
	if (action != Gtk::FILE_CHOOSER_ACTION_SAVE) {
		if (response == Gtk::RESPONSE_OK && 
		!genericFeatures::fileExists(get_filename()))
		{
			genericAlertWindow * AC = new genericAlertWindow
			(
				*this, 
				"File error" , 
				"The file you are trying to open does not exist"
			);
			delete AC;
		}
	} else {
		if (response == Gtk::RESPONSE_OK) {
			outputM3U = (genericFeatures::isExtension(get_filename(),"m3u"))
					? get_filename()
					: get_filename()+".m3u";
			
		/*	if (genericFeatures::fileExists(get_filename())) 
			{
				genericConfirmationWindow * AC = new genericConfirmationWindow
				(
					*this, 
					"This file already exists" , 
					"This file already exists.\n\nDo you want to overwrite this file ?"
				);
				delete AC;
			} */
		}
	}
}

genericFilechooserWindow::~genericFilechooserWindow
(
	void
)
{
}

int genericFilechooserWindow::getResponse
(
	void
)
{
	return response;
}

Glib::ustring genericFilechooserWindow::getURIToFile
(
	void
)
{
	return get_uri();
}

std::vector<Glib::ustring> genericFilechooserWindow::getURIToFiles
(
	void
)
{
	return get_uris();
}

Glib::ustring genericFilechooserWindow::getURIToFolder
(
	void
)
{
	return get_current_folder_uri();
}
	
std::string genericFilechooserWindow::getPathToFile
(
	void
)
{
	return get_filename();
}
	
std::string genericFilechooserWindow::getPathToM3U
(
	void
)
{
	return outputM3U;
}

	
std::vector<std::string> genericFilechooserWindow::getPathToFiles
(
	void
)
{
	return (get_action() != Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER)
		? get_filenames()
		: genericFeatures::listFiles(get_filenames().at(0));
}

std::string genericFilechooserWindow::getPathToFolder
(
	void
)
{
	return get_current_folder();
}

void genericFilechooserWindow::setVideoFilter
(
	void
) 
{
	// filter for video files
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("Video");
	filter->add_mime_type("video/*");
	// RAW Video types
	filter->add_mime_type("video/x-raw-yuv");//YUV or YCbCr video
	filter->add_mime_type("video/x-raw-rgb");//RGB video
	// Encoded Video types
	filter->add_mime_type("video/x-3ivx"); //3ivx video
	filter->add_mime_type("video/x-divx"); //divx video
	filter->add_mime_type("video/x-dv"); //digital video
	filter->add_mime_type("video/x-ffv");//ffmpeg video
	filter->add_mime_type("video/x-h263");//H.263 video
	filter->add_mime_type("video/x-h264");//H.264 video
	filter->add_mime_type("video/x-huffyuv");//Huffyuv video
	filter->add_mime_type("video/x-indeo");//Indeo video
	filter->add_mime_type("video/x-intel-h263");//H.263 video Intel variant
	filter->add_mime_type("video/x-intel-h263");//Motion JPEG video
	filter->add_mime_type("video/x-jpeg");//H.263 video Intel variant
	filter->add_mime_type("video/mpeg");//MPEG video
	filter->add_mime_type("video/x-msmpeg");//MPEG4 Microsoft variant
	filter->add_mime_type("video/x-msvideocodec");//Microsoft Video 1
	filter->add_mime_type("video/x-pn-realvideo");//Realmedia video
	filter->add_mime_type("video/x-rle");//RLE Animation video
	filter->add_mime_type("video/x-svq");//Sorenson video
	filter->add_mime_type("video/x-tarkin");//Tarkin video
	filter->add_mime_type("video/x-rle");//Theora video
	filter->add_mime_type("video/x-theora");//RLE Animation video
	filter->add_mime_type("video/x-vp3");//VP3 video
	filter->add_mime_type("video/x-wmv");//Windows Media Video
	filter->add_mime_type("video/x-xvid");//Xvid video
	//Multimedia/Video Container types
	filter->add_mime_type("video/x-ms-asf");//Advanced Streaming Format
	filter->add_mime_type("video/x-msvideo");//AVI Container
	filter->add_mime_type("video/x-matroska");//Matroska (MKV)
	filter->add_mime_type("video/ogg");//Ogg container
	filter->add_mime_type("video/quicktime");//Apple container
	filter->add_mime_type("video/vnd.rn-realmedia");//Realmedia container
	filter->add_mime_type("application/mxf");//MXF Container
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setAudioFilter
(
	void
) 
{
	// filter for audio files 
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("Audio");
	filter->add_mime_type("audio/*");
	filter->add_pattern("*.mp3");
	filter->add_pattern("*.ogg");
	filter->add_pattern("*.wav");
	filter->add_pattern("*.wma");
	filter->add_pattern("*.pcm");
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setPlaylistFilter
(
	void
) 
{
	// filter for playlist files
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("Playlist");
	filter->add_pattern("*.m3u");
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setAnyEBUCoreFilter
(
	void
) 
{
	// filter for any ebucore files
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("Any EBUCore source");
	filter->add_mime_type("application/mxf");//MXF Container mime type
	filter->add_mime_type("text/xml");//xml mime type
	filter->add_pattern("*.xml");
	filter->add_pattern("*.mxf");
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setXMLEBUCoreFilter
(
	void
) 
{
	// filter for XML ebucore files
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("XML EBUCore");
	filter->add_pattern("*.xml");
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setMXFFilter
(
	void
) 
{
	// filter for MXF files
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("MXF Container");
	filter->add_mime_type("application/mxf");//MXF Container
	filter->add_pattern("*.mxf");
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setAVCITextFilter
(
	void
) 
{
	// filter for any raw AVC-Intra text files
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("AVC-Intra Text");
	filter->add_pattern("*.txt");
	filter->add_pattern("*.dat");
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setAllFilesFilter
(
	void
) 
{
	// no filter -> all files
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("All files");
	filter->add_pattern("*");
	// add the filters to the filechooser
	add_filter(filter);
}

void genericFilechooserWindow::setFilters
(
	int mimeType
)
{	
	switch (mimeType)
	{
		case 1 : // player --> audios/videos/pictures/containers/...
		{
			setVideoFilter();
			setAudioFilter();
		} break;
		case 2 : // playlist --> m3u and folder
		{
			setPlaylistFilter();
		} break;
		case 3 : // ebucore --> xml and mxf
		{
			setAnyEBUCoreFilter();
		} break;
		case 4 : // mxf generator --> MXF container only
		{
			setMXFFilter();
		} break;
		case 5 : // mxf generator --> any raw video files
		{
			setVideoFilter();
		} break;
		case 6 : // mxf generator --> any raw audio files
		{
			setAudioFilter();
		} break;
		case 7 : // mxf generator --> any raw ebucore files
		{
			setXMLEBUCoreFilter();
		} break;
		case 8 : // mxf generator -->any raw AVC-Intra video files
		{
			setVideoFilter();
		} break;
		case 9 : // mxf generator --> any raw AVC-Intra text files
		{
			setAVCITextFilter();
		} break;
		default : {} break;
	}
	setAllFilesFilter();
}
