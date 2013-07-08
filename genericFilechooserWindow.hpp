#ifndef CPP_GENERICFILECHOOSERWINDOW_CLASS_H
#define CPP_GENERICFILECHOOSERWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file genericFilechooserWindow.hpp
 * \brief Generic filechooser window specifications
 * \details This class is used to load a generic filechooser
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 4 march 2013
 * \copyright GNU GPLv3
 *
 */

#include <gtkmm.h>
#include <gdkmm.h>
#include <iostream>
#include <string>
#include <fstream>

#include "genericAlertWindow.hpp"
#include "genericConfirmationWindow.hpp"
#include "genericFeatures.hpp"

class genericFilechooserWindow : public Gtk::FileChooserDialog
{
public:

	/*!*
	* @brief Class constructor
	* @brief The genericFilechooserWindow class constructor instantiates a new filechooser with alert messages required.\n\n
	* @param[in] Gtk::Window& parent : The parent window which instantiates the filechooser
	* @param[in] const Glib::ustring& title : The title of the filechooser message window
	* @param[in] Gtk::FileChooserAction action : The type of action supported by the FileChooser
	* @param[in] Glib::ustring currentFolder : Set the directory of entry of the filechooser
	* @param[in] bool multipleSelection : Allow the selection of several files
	* @param[in] int mimeType : Set the mime type filter \n\n
		0 : By default -> all files (*.*)\n\n 
		1 : from player -> multimedia files (audio, video, container, etc.)\n\n 
		2 from playlist -> playlist files (folder, m3u)\n\n 
		3 from metadata viewer -> ebucore files (EBUCore MXF, EBUCore XML)\n\n 
		4 from mxf generator output -> MXF container \n\n 
		5 from mxf generator raw video input -> any raw video files\n\n 
		6 from mxf generator raw audio input -> any raw audio files\n\n 
		7 from mxf generator raw ebucore input -> any raw ebucore files\n\n 
		8 from mxf generator raw AVC-Intra video input -> any raw AVC-Intra video files\n\n 
		9 from mxf generator segmentation and data text input -> any raw AVC-Intra text files
	*/	
	genericFilechooserWindow
	(
		Gtk::Window& parent, 
		const Glib::ustring& title, 
		Gtk::FileChooserAction action, 
		Glib::ustring currentFolder,
		bool multipleSelection,
		int mimeType = 0
	);
	
	virtual ~genericFilechooserWindow
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
	* @fn Glib::ustring getURIToFile(void)
	* @brief This function will return the URI to the selected file
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return Glib::ustring if all is right or an error at compilation time.
	*/
	Glib::ustring getURIToFile
	(
		void
	);
	
	/**
	* @fn std::vector<Glib::ustring> getURIToFiles(void)
	* @brief This function will return a vector with the URI to all selected files
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return std::vector<Glib::ustring> if all is right or an error at compilation time.
	*/
	std::vector<Glib::ustring> getURIToFiles
	(
		void
	);

	/**
	* @fn Glib::ustring getURIToFolder(void)
	* @brief This function will return the URI to the selected folder
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return Glib::ustring if all is right or an error at compilation time.
	*/
	Glib::ustring getURIToFolder
	(
		void
	);
	
	/**
	* @fn std::string getPathToFile(void)
	* @brief This function will return the path to the selected file
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return std::string if all is right or an error at compilation time.
	*/
	std::string getPathToFile
	(
		void
	);
	
	/**
	* @fn std::string getPathToM(void)
	* @brief This function will return the path to file in which the playlist must be saved
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return std::string if all is right or an error at compilation time.
	*/
	std::string getPathToM3U
	(
		void
	);
	/**
	* @fn std::vector<std::string> getPathToFiles(void)
	* @brief This function will return a vector with the path to all selected files
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return std::vector<std::string> if all is right or an error at compilation time.
	*/
	std::vector<std::string> getPathToFiles
	(
		void
	);

	/**
	* @fn std::string getPathToFolder(void)
	* @brief This function will return the path to the selected folder
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return std::string if all is right or an error at compilation time.
	*/
	std::string getPathToFolder
	(
		void
	);
	
protected:

	int response; /*!< response the Gtk::ResponType selected by the user into the filechooser */
	std::string outputM3U;/*!< outputM3U this string will return the path to the output file */

	/**
	* @fn void setVideoFilter(void)
	* @brief This function filters files whose mime type corresponds to video files. All others mime types are excluded
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setVideoFilter
	(
		void
	);

	/**
	* @fn void setAudioFilter(void)
	* @brief This function filters files whose mime type corresponds to audio files. All others mime types are excluded
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setAudioFilter
	(
		void
	);

	/**
	* @fn void setPlaylistFilter(void)
	* @brief This function filters files whose mime type corresponds to playlist files. All others mime types are excluded
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setPlaylistFilter
	(
		void
	);

	/**
	* @fn void setAnyEBUCoreFilter(void)
	* @brief This function filters files whose mime type corresponds to any EBUCore files (XML EBUCore or MXF EBUCore). All others mime types are excluded
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setAnyEBUCoreFilter
	(
		void
	);

	/**
	* @fn void setXMLEBUCoreFilter(void)
	* @brief This function filters files whose mime type corresponds to XML EBUCore files. All others mime types are excluded
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setXMLEBUCoreFilter
	(
		void
	);

	/**
	* @fn void setMXFFilter(void)
	* @brief This function filters files whose mime type corresponds to MXF files. All others mime types are excluded
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setMXFFilter
	(
		void
	);

	/**
	* @fn void setAVCITextFilter(void)
	* @brief This function filters files whose mime type corresponds to AVC-Intra text files. All others mime types are excluded
	* @brief
	* @note Currently, only *.txt files and *.dat files are allowed.
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setAVCITextFilter
	(
		void
	);

	/**
	* @fn void setAllFilesFilter(void)
	* @brief This functions defines the filter "All Files" ; this means all files are visible and accessible.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return void if all is right or an error at compilation time.
	*/
	void setAllFilesFilter
	(
		void
	);
	
	/**
	* @fn void setFilters(int mimeType)
	* @brief This function allows you to set the appropriate mime types
	* @brief
	* @note needs more documentation
	* @param[in] int mimeType : this int defines the appropriate mime types
	* @note Values for mime types are : \n
	* \li \c \b 1 : All the mime types readable by the mediaplayer
	* \li \c \b 2 : All the mime types readable by the playlist
	* \li \c \b 3 : All the mime types readable by the EBUCore Metadata Viewer
	* \li \c \b 4 : Only MXF mime types
	* \li \c \b 5 : All the mime types of raw video
	* \li \c \b 6 : All the mime types of raw audio
	* \li \c \b 7 : All the mime types of raw ebucore
	* \li \c \b 8 : All the mime types of raw AVC-Intra video
	* \li \c \b 9 : All the mime types of raw AVC-Intra text
	* \li \c \b 0 : By default, do nothing. 
	* @return nothning if all is right or an error at compilation time.
	*/
	void setFilters
	(
		int mimeType
	);

};

#endif /*CPP_GENERICFILECHOOSERWINDOW_CLASS_H*/
