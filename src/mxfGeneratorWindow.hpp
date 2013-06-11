#ifndef CPP_MXFGENERATORWINDOW_CLASS_H
#define CPP_MXFGENERATORWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file mxfGeneratorWindow.hpp
 * \brief MXF Generator window specifications
 * \details This class is used to load the EBU MXF Generator and his features
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 27 february 2013
 * \copyright GNU GPLv3
 *
 */


#include <gtkmm-3.0/gtkmm.h>
#include <gdkmm-3.0/gdkmm.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif
#include <iostream>
#include <string.h>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <time.h>

#include "genericFeatures.hpp"
#include "genericFilechooserWindow.hpp"
#include "BMXMXFEngine.hpp"

#define MXFGENERATORMIMETYPEOUTPUT 4
#define MXFGENERATORVIDEOMIMETYPEINPUT 5
#define MXFGENERATORAUDIOMIMETYPEINPUT 6
#define MXFGENERATOREBUCOREMIMETYPEINPUT 7
#define MXFGENERATORAVCIVIDEOMIMETYPEINPUT 8
#define MXFGENERATORAVCITXTMIMETYPEINPUT 9

/*! \class mxfGeneratorWindow
 * \brief this class represent the MXF Generator
 *
 * This class controls all the MXF Generator features and its links
 */

class mxfGeneratorWindow : public Gtk::Window
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
	 * @brief The mxfGeneratorWindow class constructor instantiates a new MXF Generator window and all  its features.\n\n
	 * @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
	 * @param[in] refGlade : This is the reference to your glade design
	 */
	mxfGeneratorWindow
	(
		BaseObjectType* cobject,
		const Glib::RefPtr<Gtk::Builder>& refGlade
	);
	
	
	//////////////////////
	//    destructor    //
	//////////////////////
	
	/*!*
	 * @brief Class destructor
	 * @brief The mxfGeneratorWindow class destructor destroys all variables and instanciate objects. This is the garbage collector class. It is always called at the end of class life.
	 *
	 */
	virtual ~mxfGeneratorWindow
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
	//   Gtk Entries    //
	//////////////////////
	Gtk::Entry * path2video;/*!< path2video This entry captures the path to the raw video stream. */
	Gtk::Entry * path2audio;/*!< path2audio This entry captures the path to the folder where are the raw audio streams. */
	Gtk::Entry * path2ebucore;/*!< path2ebucore This entry captures the path to the raw ebucore stream. */
		
	
	//////////////////////
	//   Gtk Buttons    //
	//////////////////////
	Gtk::Button* closeMXFGen; /*!< closeMXFGen This button closes the MXF Generator window. */
	Gtk::Button* createMXFGen;/*!< createMXFGen This button tries to create the MXF file with the input raw streams.*/
	Gtk::Button* openVideoMXFGen;/*!< openVideoMXFGen This button opens a filechooser to select the video raw stream. */
	Gtk::Button* openAudioMXFGen;/*!< openAudioMXFGen This button opens a filechooser to select the folder where are the audio raw streams.*/
	Gtk::Button* openEBUCoreMXFGen;/*!< openEBUCoreMXFGen This button opens a filechooser to select the EBUCore raw stream.*/
	Gtk::Button* AVCIHeadFileB;

	//////////////////////
	//   Gtk Widgets    //
	//////////////////////
	Gtk::ComboBoxText * complianceTypeCBT;
	Gtk::ComboBoxText * videoStreamFormatCBT;
	Gtk::ComboBoxText * audioStreamFormatCBT;
	Gtk::ComboBoxText * framerateChoicesCBT; 
	Gtk::ComboBoxText * AVCIHeadFormatCBT;
	Gtk::ToggleButton * framerateEnableTB;
	Gtk::ToggleButton * timecodeEnableTB;
	Gtk::ToggleButton * AVCIHeadTB;
	Gtk::Entry * timecodeHoursEntry;
	Gtk::Entry * timecodeMinutesEntry;
	Gtk::Entry * timecodeSecondsEntry;
	Gtk::Entry * timecodeDroppedFramesEntry;
	Gtk::Entry * clipnameEntry;
	Gtk::Entry * durationInFrameUnitsEntry;
	Gtk::Entry * AVCIHeadOffsetEntry;
	Gtk::Entry * AVCIHeadFilePathEntry;
	
	Gtk::Box * OP1aSpecificConfigurationBox;
	Gtk::CheckButton * op1a_minPartCB;
	Gtk::Box * AS02SpecificConfigurationBox;
	Gtk::CheckButton * as02_micTypeCB;
	Gtk::CheckButton * as02_micFileCB;
	Gtk::Entry * as02_shimNameEntry;
	Gtk::Entry * as02_shimIdEntry;
	Gtk::Entry * as02_shimAnnotEntry;
	Gtk::Box * AS02AS11OP1aOP1aSpecificConfigurationBox;
	Gtk::Entry * as02_as11op1_op1a_partIntervalEntry;
	Gtk::ComboBoxText * as02_as11op1_op1a_partIntervalCBT;
	Gtk::Box * AS11OP1aAS11D10SpecificConfigurationBox;
	Gtk::Entry * as11_op1a_as11_d10_dmFileEntry;
	Gtk::Button * as11_op1a_as11_d10_dmFileB;
	Gtk::ComboBoxText * as11_op1a_as11_d10_dmFrameworkCBT;
	Gtk::Entry * as11_op1a_as11_d10_segNameEntry;
	Gtk::Button * as11_op1a_as11_d10_segNameB;
	Gtk::Box * AS11OP1aOP1aSpecificConfigurationBox;
	Gtk::Entry * as11_op1a_op1a_outStartEntry;
	Gtk::Entry * as11_op1a_op1a_outEndEntry;
	Gtk::Button* as11_op1a_op1a_singlePassB;
	Gtk::CheckButton * as11_op1a_op1a_MD5ChecksumCB;
	Gtk::Box * AS11OP1aAS11D10OP1aD10SpecificConfigurationBox;
	Gtk::Entry * as11op1_as11d10_op1a_d10_seqOffEntry;

	
//	Gtk::Box * AS11D10D10SpecificConfigurationBox;
//	Gtk::Box * AvidSpecificConfigurationBox;
//	Gtk::Box * WaveSpecificConfigurationBox;
	
	// variable d'état
	bool minpart;
	std::string timecodeBMXFormat;
	

/////////////////////////////////
//////                     ////// 
////// protected functions //////
//////                     //////
/////////////////////////////////
	
	/**
	 * @fn void initMXFGenStates(void)
	 * @brief To initialize MXF Generator interface's states
	 * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
	 * @return nothing if all is right or an error at compilation time.
	 */
	void initMXFGenStates(void);
	
	/**
	 * @fn void refGladeButton (void)
	 * @brief To link the glade references to the buttons
	 * @brief This method loads all required glade's references to connect the MXF Generator buttons.\n \n
	 * @brief
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
	 * @brief To link the glade references to the MXF Generator features
	 * @brief This method loads all required glade's references to connect the MXF Generator widgets.\n \n
	 * @brief 
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
	 * @brief 
	 * @param[in] void : no input parameter.
	 * @return  nothing if all is right or an error at compilation time.
	 * @note This function and his documentation must be completed
	 */
	void connectSignalClicked
	(
		void
	);
	/**
	* @fn void on_close_MXFGenerator_clicked(void)
	* @brief This callback closes the MXF Generator window
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_close_MXFGenerator_clicked
	(
		void
	);

	/**
	* @fn void on_create_MXFGenerator_clicked(void)
	* @brief This callback tries to generate a MXF file with the raw streams indicated.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_create_MXFGenerator_clicked
	(
		void
	);

	/**
	* @fn void on_open_MXFGenerator_video_clicked(void)
	* @brief This callback launches a filechooser to select the input raw video stream.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_open_MXFGenerator_video_clicked
	(
		void
	);

	/**
	* @fn void on_open_MXFGenerator_audio_clicked(void)
	* @brief This callback launches a filechooser to select the folder where are the input raw audio streams.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_open_MXFGenerator_audio_clicked
	(
		void
	);

	/**
	* @fn void on_open_MXFGenerator_ebucore_clicked(void)
	* @brief This callback launches a filechooser to select the input raw ebucore stream.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_open_MXFGenerator_ebucore_clicked
	(
		void
	);
	
	/**
	* @fn std::string getBMXMXFType(void)
	* @brief This function returns a string representing the MXF type wished.
	* @brief
	* @note Only the BMX types are available
	* @param[in] void : no params
	* @return std::string if all is right or an error at compilation time.
	*/
	std::string getBMXMXFType
	(
		void
	);

	/**
	* @fn void on_enable_framerate(void)
	* @brief This callback enables the frame rate configuration.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_enable_framerate
	(
		void
	);

	/**
	* @fn void on_enable_timecode(void)
	* @brief This callback enables the timecode configuration.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_enable_timecode
	(
		void
	);

	/**
	* @fn void on_enable_AVCIHead(void)
	* @brief This callback enables to configure the AVC-Intra sequence header data.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_enable_AVCIHead
	(
		void
	);

	/**
	* @fn void on_AVCIHeadFileB_clicked(void)
	* @brief This callback launches a filechooser to select the AVC-Intra file.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_AVCIHeadFileB_clicked
	(
		void
	);

	/**
	* @fn void on_as11_op1a_as11_d10_dmFileB_clicked(void)
	* @brief This callback launches a filechooser to select the descriptive framework properties text file.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_as11_op1a_as11_d10_dmFileB_clicked
	(
		void
	);

	/**
	* @fn voidon_as11_op1a_as11_d10_segNameB_clicked(void)
	* @brief This callback launches a filechooser to select the segmentation data text file.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_as11_op1a_as11_d10_segNameB_clicked
	(
		void
	);

	/**
	* @fn void on_complianceTypeCBT_changed(void)
	* @brief This callback shows or hides features depending on the BMX type.
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void on_complianceTypeCBT_changed
	(
		void
	);

	/**
	* @fn void formatTimecode(void)
	* @brief This function adjusts the timecode format to RAW2BMX
	* @brief
	* @note needs more documentation
	* @param[in] void : no params
	* @return nothing if all is right or an error at compilation time.
	*/
	void formatTimecode
	(
		void
	);
};

#endif
