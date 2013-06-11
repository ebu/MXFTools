/*!
 * \project_name EBU Player
 * \file mxfGeneratorWindow.cpp
 * \brief MXF OP1a Generator window
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 27 february 2013
 *
 * This software is published in LGPLv3.0
 *
 */


#include "mxfGeneratorWindow.hpp"

// class constructor and destructor

mxfGeneratorWindow::mxfGeneratorWindow
	(
		BaseObjectType* cobject, 
		const Glib::RefPtr<Gtk::Builder>& refGlade
	) 
	: 
		Gtk::Window(cobject),
		m_refGlade(refGlade)
{
	// To link the Glade references
	refGladeButton(); // for buttons
	refGladeWidget(); // for any widgets;
	// To initialize all Gtk widgets and mediaplayer's variables
	initMXFGenStates();
	// To connect the signals
	connectSignalClicked(); // clicked 
}

mxfGeneratorWindow::~mxfGeneratorWindow
(
	void
)
{
}

// class public functions


// class protected functions

void mxfGeneratorWindow::initMXFGenStates
(
	void
) 
{
	complianceTypeCBT->set_active(0); // By default, a simple op1a
	
	OP1aSpecificConfigurationBox->show();
/*	AS02SpecificConfigurationBox->hide();
	AS02AS11OP1aOP1aSpecificConfigurationBox->hide();
	AS11OP1aAS11D10SpecificConfigurationBox->hide();
	AS11OP1aOP1aSpecificConfigurationBox->hide();
	AS11OP1aAS11D10OP1aD10SpecificConfigurationBox->hide();
*/	

	videoStreamFormatCBT->set_active(0);
	audioStreamFormatCBT->set_active(0);
	/*op1a-minPartCB
	as02-micTypeCB
	as02-micFileCB
	as02-shimNameEntry
	as02-shimIdEntry
	as02-shimAnnotEntry
	as02-as11op1-op1a-partIntervalEntry
	as02-as11op1-op1a-partIntervalCBT
	as11-op1a-as11-d10-dmFileEntry
	as11-op1a-as11-d10-dmFrameworkCBT
	as11-op1a-as11-d10-segNameEntry
	as11-op1a-op1a-outStartEntry
	as11-op1a-op1a-outEndEntry
	as11-op1a-op1a-singlePassB
	as11-op1a-op1a-MD5ChecksumCB
	as11op1-as11d10-op1a-d10-seqOffEntry*/
	
	framerateEnableTB->set_active(false);// By default, the frame rate is 25
	framerateChoicesCBT->set_active(2); // 25 frames selected
	timecodeEnableTB->set_active(false); // Timecode by default 00:00:00:00   
	timecodeHoursEntry->set_sensitive(false); // Hours (HH)   
	timecodeHoursEntry->set_max_length(2);
	timecodeMinutesEntry->set_sensitive(false); // Minutes (MM)
	timecodeMinutesEntry->set_max_length(2);
	timecodeSecondsEntry->set_sensitive(false); // Seconds (SS)
	timecodeSecondsEntry->set_max_length(2);
	timecodeDroppedFramesEntry->set_sensitive(false); // Dropped frames (DD)
	timecodeDroppedFramesEntry->set_max_length(3);
	AVCIHeadTB->set_active(false);
	AVCIHeadFileB->set_sensitive(false);
	AVCIHeadFormatCBT->set_active(16); // set AVC-Intra format to All by default
	AVCIHeadFormatCBT->set_sensitive(false);
	AVCIHeadOffsetEntry->set_sensitive(false);
	AVCIHeadFilePathEntry->set_sensitive(false);
	
	
	// variable
	minpart = true;
}

void mxfGeneratorWindow::refGladeButton
(
	void
) 
{
	m_refGlade->get_widget
	(
		"close_MXFOP1a_from_SDK", 
		closeMXFGen
	);
	m_refGlade->get_widget
	(
		"create_MXFOP1a_from_SDK", 
		createMXFGen
	);
	m_refGlade->get_widget
	(
		"create_mxf_open_video_stream", 
		openVideoMXFGen
	);
	m_refGlade->get_widget
	(
		"create_mxf_open_audio_stream", 
		openAudioMXFGen
	);
	m_refGlade->get_widget
	(
		"create_mxf_open_ebucore_stream", 
		openEBUCoreMXFGen
	);
}

void mxfGeneratorWindow::refGladeWidget
(
	void
) 
{
	m_refGlade->get_widget
	(
		"create_mxf_path_to_video", 
		path2video
	);
	m_refGlade->get_widget
	(
		"create_mxf_path_to_audio", 
		path2audio
	);
	m_refGlade->get_widget
	(
		"create_mxf_path_to_ebucore", 
		path2ebucore
	);
	m_refGlade->get_widget
	(
		"complianceTypeCBT", 
		complianceTypeCBT
	);
	m_refGlade->get_widget
	(
		"framerateChoicesCBT", 
		framerateChoicesCBT
	);
	m_refGlade->get_widget
	(
		"videoStreamFormatCBT",
		videoStreamFormatCBT
	);
	m_refGlade->get_widget
	(
		"audioStreamFormatCBT",
		audioStreamFormatCBT
	);
	m_refGlade->get_widget
	(
		"framerateEnableTB", 
		framerateEnableTB
	);
	m_refGlade->get_widget
	(
		"timecodeEnableTB", 
		timecodeEnableTB
	);
	m_refGlade->get_widget
	(
		"timecodeHoursEntry", 
		timecodeHoursEntry
	);
	m_refGlade->get_widget
	(
		"timecodeMinutesEntry", 
		timecodeMinutesEntry
	);
	m_refGlade->get_widget
	(
		"timecodeSecondsEntry", 
		timecodeSecondsEntry
	);
	m_refGlade->get_widget
	(
		"timecodeDroppedFramesEntry", 
		timecodeDroppedFramesEntry
	);
	m_refGlade->get_widget
	(
		"clipnameEntry", 
		clipnameEntry
	);
	m_refGlade->get_widget
	(
		"durationInFrameUnitsEntry", 
		durationInFrameUnitsEntry
	);
	m_refGlade->get_widget
	(
		"AVCIHeadOffsetEntry", 
		AVCIHeadOffsetEntry
	);
	m_refGlade->get_widget
	(
		"AVCIHeadFilePathEntry", 
		AVCIHeadFilePathEntry
	);
	m_refGlade->get_widget
	(
		"AVCIHeadTB", 
		AVCIHeadTB
	);
	m_refGlade->get_widget
	(
		"AVCIHeadFormatCBT", 
		AVCIHeadFormatCBT
	);
	m_refGlade->get_widget
	(
		"AVCIHeadFileB", 
		AVCIHeadFileB
	);
	m_refGlade->get_widget
	(
		"OP1aSpecificConfigurationBox", 
		OP1aSpecificConfigurationBox
	);
	m_refGlade->get_widget
	(
		"AS02SpecificConfigurationBox", 
		AS02SpecificConfigurationBox
	);
	m_refGlade->get_widget
	(
		"AS02AS11OP1aOP1aSpecificConfigurationBox", 
		AS02AS11OP1aOP1aSpecificConfigurationBox
	);
	m_refGlade->get_widget
	(
		"AS11OP1aAS11D10SpecificConfigurationBox", 
		AS11OP1aAS11D10SpecificConfigurationBox
	);
	m_refGlade->get_widget
	(
		"AS11OP1aOP1aSpecificConfigurationBox", 
		AS11OP1aOP1aSpecificConfigurationBox
	);
	m_refGlade->get_widget
	(
		"AS11OP1aAS11D10OP1aD10SpecificConfigurationBox", 
		AS11OP1aAS11D10OP1aD10SpecificConfigurationBox
	);
	m_refGlade->get_widget
	(
		"op1a_minPartCB",
		op1a_minPartCB
	);
	m_refGlade->get_widget
	(
		"as02_micTypeCB",
		as02_micTypeCB
	);
	m_refGlade->get_widget
	(
		"as02_micFileCB",
		as02_micFileCB
	);
	m_refGlade->get_widget
	(
		"as02_shimNameEntry",
		as02_shimNameEntry
	);
	m_refGlade->get_widget
	(
		"as02_shimIdEntry",
		as02_shimIdEntry
	);
	m_refGlade->get_widget
	(
		"as02_shimAnnotEntry",
		as02_shimAnnotEntry
	);
	m_refGlade->get_widget
	(
		"as02_as11op1_op1a_partIntervalEntry",
		as02_as11op1_op1a_partIntervalEntry
	);
	m_refGlade->get_widget
	(
		"as02_as11op1_op1a_partIntervalCBT",
		as02_as11op1_op1a_partIntervalCBT
	);
	m_refGlade->get_widget
	(
		"as11_op1a_as11_d10_dmFileEntry",
		as11_op1a_as11_d10_dmFileEntry
	);
	m_refGlade->get_widget
	(
		"as11_op1a_as11_d10_dmFrameworkCBT",
		as11_op1a_as11_d10_dmFrameworkCBT
	);
	m_refGlade->get_widget
	(
		"as11_op1a_as11_d10_segNameEntry",
		as11_op1a_as11_d10_segNameEntry
	);
	m_refGlade->get_widget
	(
		"as11_op1a_op1a_outStartEntry",
		as11_op1a_op1a_outStartEntry
	);
	m_refGlade->get_widget
	(
		"as11_op1a_op1a_outEndEntry",
		as11_op1a_op1a_outEndEntry
	);
	m_refGlade->get_widget
	(
		"as11_op1a_op1a_singlePassB",
		as11_op1a_op1a_singlePassB
	);
	m_refGlade->get_widget
	(
		"as11_op1a_op1a_MD5ChecksumCB",
		as11_op1a_op1a_MD5ChecksumCB
	);
	m_refGlade->get_widget
	(
		"as11op1_as11d10_op1a_d10_seqOffEntry",
		as11op1_as11d10_op1a_d10_seqOffEntry
	);
	m_refGlade->get_widget
	(
		"as11_op1a_as11_d10_dmFileB",
		as11_op1a_as11_d10_dmFileB
	);
	m_refGlade->get_widget
	(
		"as11_op1a_as11_d10_segNameB",
		as11_op1a_as11_d10_segNameB
	);
	
/*	*/
}

void mxfGeneratorWindow::connectSignalClicked
(
	void
) 
{
	closeMXFGen->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_close_MXFGenerator_clicked
		)
	);
	createMXFGen->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_create_MXFGenerator_clicked
		)
	);
	openVideoMXFGen->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_open_MXFGenerator_video_clicked
		)
	);
	openAudioMXFGen->signal_pressed().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_open_MXFGenerator_audio_clicked
		)
	);
	openEBUCoreMXFGen->signal_released().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_open_MXFGenerator_ebucore_clicked
		)
	);
	framerateEnableTB->signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_enable_framerate
		)
	);
	timecodeEnableTB->signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_enable_timecode
		)
	);
	AVCIHeadTB->signal_toggled().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_enable_AVCIHead
		)
	);
	AVCIHeadFileB->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_AVCIHeadFileB_clicked
		)
	);
	complianceTypeCBT->signal_changed().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_complianceTypeCBT_changed
		)
	);
	as11_op1a_as11_d10_dmFileB->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_as11_op1a_as11_d10_dmFileB_clicked
		)
	);
	as11_op1a_as11_d10_segNameB->signal_clicked().connect
	(
		sigc::mem_fun
		(
			*this, 
			&mxfGeneratorWindow::on_as11_op1a_as11_d10_segNameB_clicked
		)
	);/**/
}

void mxfGeneratorWindow::on_close_MXFGenerator_clicked
(
	void
)
{
	hide();
	path2video->set_text("");
	path2audio->set_text("");
	path2ebucore->set_text("");
}

void mxfGeneratorWindow::on_create_MXFGenerator_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Save the MXF file",
		Gtk::FILE_CHOOSER_ACTION_SAVE,
		Glib::get_home_dir(),
		false,
		MXFGENERATORMIMETYPEOUTPUT
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// prepare the bmx response
		bool BMXWrapperResponse;
		// prepare timecode
		formatTimecode();
		// try to generate the final MXF file
		BMXWrapperResponse = BMXMXFEngine::generateMXF
		(
			FC->getPathToFile(),
			getBMXMXFType(),
			path2video->get_text(),
			path2audio->get_text(),
			path2ebucore->get_text(),
			framerateEnableTB->get_active(),
			framerateChoicesCBT->get_active_row_number(),
			timecodeBMXFormat,
			clipnameEntry->get_text(),
			(
				(durationInFrameUnitsEntry->get_text_length()>0)
				? atoi((durationInFrameUnitsEntry->get_text()).c_str())
				: -1
			),
			AVCIHeadTB->get_active(),
			genericFeatures::int2str(AVCIHeadFormatCBT->get_active_row_number()),
			AVCIHeadFilePathEntry->get_text(),
			AVCIHeadOffsetEntry->get_text()
		);
		// Prepare the dialog message to inform the user
		Gtk::MessageDialog dialog(*this, "MXF Generation");
		// set the appropriated dialog message
		dialog.set_secondary_text( 
			(
				(BMXWrapperResponse) 
				? "The MXF file generation success" 
				: "The MXF file generation fail"
			)
		);
		// launch the dialog message
		dialog.run();
		
		if (BMXWrapperResponse) {hide();}
	}
	// destroy the filechooser
	delete FC;
	
}

void mxfGeneratorWindow::on_open_MXFGenerator_video_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select a RAW video stream",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		MXFGENERATORVIDEOMIMETYPEINPUT
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the path to the RAW video stream
		path2video->set_text(FC->getPathToFile());
	}
	delete FC;
}

void mxfGeneratorWindow::on_open_MXFGenerator_audio_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select the folder where are the RAW audio streams",
		Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER,
		Glib::get_home_dir(),
		false,
		MXFGENERATORAUDIOMIMETYPEINPUT
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the path to the folder where are the RAW audio streams
		path2audio->set_text(FC->getPathToFolder());
	}
	delete FC;
}

void mxfGeneratorWindow::on_open_MXFGenerator_ebucore_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select a RAW EBUCore stream",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		MXFGENERATOREBUCOREMIMETYPEINPUT
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the path to the RAW ebucore stream
		path2ebucore->set_text(FC->getPathToFile());
	}
	delete FC;
}

std::string mxfGeneratorWindow::getBMXMXFType
(
	void
)
{
	const std::string bmxmxftype[] = {
		"op1a","as02","as11op1a","as11d10","d10","avid","wave"
	};
	return bmxmxftype[complianceTypeCBT->get_active_row_number()];
	
}

void mxfGeneratorWindow::on_enable_framerate
(
	void
)
{
	framerateChoicesCBT->set_sensitive(!framerateChoicesCBT->get_sensitive());
}

void mxfGeneratorWindow::on_enable_timecode
(
	void
)
{
	timecodeHoursEntry->set_sensitive(!timecodeHoursEntry->get_sensitive());
	timecodeMinutesEntry->set_sensitive(!timecodeMinutesEntry->get_sensitive());
	timecodeSecondsEntry->set_sensitive(!timecodeSecondsEntry->get_sensitive());
	timecodeDroppedFramesEntry->set_sensitive(!timecodeDroppedFramesEntry->get_sensitive());
}

void mxfGeneratorWindow::on_enable_AVCIHead
(
	void
)
{

	AVCIHeadFileB->set_sensitive(!AVCIHeadFileB->get_sensitive());
	AVCIHeadFormatCBT->set_sensitive(!AVCIHeadFormatCBT->get_sensitive());
	AVCIHeadOffsetEntry->set_sensitive(!AVCIHeadOffsetEntry->get_sensitive());
	AVCIHeadFilePathEntry->set_sensitive(!AVCIHeadFilePathEntry->get_sensitive());
}

void mxfGeneratorWindow::on_AVCIHeadFileB_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select an AVC-Intra file",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		MXFGENERATORAVCIVIDEOMIMETYPEINPUT
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the path to the RAW video stream
		AVCIHeadFilePathEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}

void mxfGeneratorWindow::on_as11_op1a_as11_d10_dmFileB_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select the descriptive framework properties from a text file",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		MXFGENERATORAVCITXTMIMETYPEINPUT
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the path to the descriptive framework properties file
		as11_op1a_as11_d10_dmFileEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}

void mxfGeneratorWindow::on_as11_op1a_as11_d10_segNameB_clicked
(
	void
)
{
	// instanciate a new filechooser
	genericFilechooserWindow * FC = new genericFilechooserWindow
	(
		*this,
		"Select the segmentation data from a text file",
		Gtk::FILE_CHOOSER_ACTION_OPEN,
		Glib::get_home_dir(),
		false,
		MXFGENERATORAVCITXTMIMETYPEINPUT
	);
	
	if (FC->getResponse() == Gtk::RESPONSE_OK) 
	{
		// set the path to the segmentation data file
		as11_op1a_as11_d10_segNameEntry->set_text(FC->getPathToFile());
	}
	delete FC;
}

void mxfGeneratorWindow::on_complianceTypeCBT_changed
(
	void
)
{

	OP1aSpecificConfigurationBox->hide();
	AS02SpecificConfigurationBox->hide();
	AS02AS11OP1aOP1aSpecificConfigurationBox->hide();
	AS11OP1aAS11D10SpecificConfigurationBox->hide();
	AS11OP1aOP1aSpecificConfigurationBox->hide();
	AS11OP1aAS11D10OP1aD10SpecificConfigurationBox->hide();
	
	switch (complianceTypeCBT->get_active_row_number()) 
	{
		case 0 : 
		{
			OP1aSpecificConfigurationBox->show();
			AS02AS11OP1aOP1aSpecificConfigurationBox->show();
			AS11OP1aOP1aSpecificConfigurationBox->show();	
			AS11OP1aAS11D10OP1aD10SpecificConfigurationBox->show();
		} break;
		case 1 : 
		{
			AS02SpecificConfigurationBox->show();
			AS02AS11OP1aOP1aSpecificConfigurationBox->show();
		} break;
		case 2 : 
		{
			AS02AS11OP1aOP1aSpecificConfigurationBox->show();
			AS11OP1aAS11D10SpecificConfigurationBox->show();
			AS11OP1aOP1aSpecificConfigurationBox->show();
			AS11OP1aAS11D10OP1aD10SpecificConfigurationBox->show();
		} break;
		case 3 : 
		{
			AS11OP1aAS11D10SpecificConfigurationBox->show();
		} break;
		case 4 : 
		{
			AS11OP1aAS11D10SpecificConfigurationBox->show();
			AS11OP1aAS11D10OP1aD10SpecificConfigurationBox->show();
		} break;
		case 5 : 
		{
			// avid
		} break;
		case 6 : 
		{
			// wave
		} break;
	}
}

void mxfGeneratorWindow::formatTimecode
(
	void
)
{
	// prepare timecode to BMX
	// need to be improve
	timecodeBMXFormat =
	(
		(	
			(timecodeHoursEntry->get_text_length()>0)
			? timecodeHoursEntry->get_text()
			: "00"
		)
		+":"+
		(	
			(timecodeMinutesEntry->get_text_length()>0)
			? timecodeMinutesEntry->get_text()
			: "00"
		)
		+":"+
		(	
			(timecodeSecondsEntry->get_text_length()>0)
			? timecodeSecondsEntry->get_text()
			: "00"
		)
		+
		(	
			(timecodeDroppedFramesEntry->get_text_length()>0)
			? timecodeDroppedFramesEntry->get_text()
			: ":00"
		)
	);
}
