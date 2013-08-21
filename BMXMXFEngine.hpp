#ifndef CPP_BMXMXFENGINE_CLASS_H
#define CPP_BMXMXFENGINE_CLASS_H

/*!
 * \project_name EBU Player
 * \file BMXMXFEngine.hpp
 * \brief This class implements the BMX features to wrap and unwrap RAW streams into a MXF container. 
 * \details This class implements the BMX features to wrap and unwrap RAW streams into a MXF container without instantiating a class. We don't need all the BMX features. Basically, all public functions are declared as static to avoid the class instanciation. Currently, only the wrapping features are implemented.
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 * \copyright GNU GPLv3
 *
 */

#include <iostream>
#include <string>
#include <vector>

#include <bmx/clip_writer/ClipWriter.h>
#include <bmx/as02/AS02PictureTrack.h>
#include <bmx/as02/AS02PCMTrack.h>
#include <bmx/avid_mxf/AvidPCMTrack.h>
#include <bmx/mxf_op1a/OP1APCMTrack.h>
#include <bmx/essence_parser/DVEssenceParser.h>
#include <bmx/essence_parser/MPEG2EssenceParser.h>
#include <bmx/essence_parser/AVCIRawEssenceReader.h>
#include <bmx/essence_parser/MJPEGEssenceParser.h>
#include <bmx/essence_parser/VC3EssenceParser.h>
#include <bmx/essence_parser/RawEssenceReader.h>
#include <bmx/wave/WaveFileIO.h>
#include <bmx/wave/WaveReader.h>
#include <bmx/URI.h>
#include <bmx/MXFUtils.h>
#include <bmx/Utils.h>
#include <bmx/Version.h>
#include <bmx/AppUtils.h>
#include <bmx/AS11Helper.h>
#include <bmx/BMXException.h>
#include <bmx/Logging.h>

#include <mxf/mxf_avid.h>

#ifdef _WIN32
	#include <mxf/mxf_win32_file.h>
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
#endif


#define DEFAULT_BEXT_ORIGINATOR "bmx"

using namespace std;
using namespace bmx;
using namespace mxfpp;


/*! \class BMXMXFEngine
 * \brief this class is used to define the BMX features useful to the EBU Mediaplayer and its components (wrapper, unwrapper,etc...)
 *
 * This class implements the BMX features to wrap RAW streams into a MXF container in one step and without the class instantiating. We don't need all the BMX features. Basically, this is only one public function which is declared as static to avoid the class instanciation. Currently, only the wrapping features are implemented and all are defined as privates.
 */

class BMXMXFEngine {
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

	/**
	 * @fn static bool generateMXF(
	 		std::string mxf_filename,
			std::string mxf_type,
			std::string raw_video_filename,
			std::string raw_audio_directory,
			std::string raw_ebucore_filename,
			bool forceFramerate, 
			int framerate_type,
			std::string timecode_value,
			std::string clipname_value,
			int64_t clipduration_value,
			bool set_avci_sequence_header,
			std::string avci_format_str,
			std::string avci_file_str,
			std::string avci_offset_str
		)
	 * @brief This function tries to build the MXF file with the input arguments
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string mxf_filename   : path to mxf file
	 * @param[in] std::string mxf_type : the MXF type wished
	 * @param[in] std::string raw_video_filename : path to the raw video stream
	 * @param[in] std::string raw_audio_directory   : path to the directory of raw audio streams
	 * @param[in] std::string raw_ebucore_filename : path to the raw ebucore stream
	 * @param[in] bool forceFramerate  : set the video frame rate
	 * @param[in] framerate_type : frame rate value
	 * @param[in] std::string timecode_value : timecode in BMX format
	 * @param[in] std::string clipname_value   : name of the mxf clip
	 * @param[in] int64_t clipduration_value : duration of the mxf clip
	 * @param[in] bool set_avci_sequence_header : set the AVC-Intra header sequence
	 * @param[in] std::string avci_format_str : set the flavor of the the AVC-Intra file
	 * @param[in] std::string avci_file_str   : set the path to the AVC-Intra file
	 * @param[in] std::string avci_offset_str : set the start offset of the AVC-Intra file
	 * @return bool if all is right or an error at compilation time.
	 */
	static bool generateMXF
	(
		std::string mxf_filename,
		std::string mxf_type,
		std::string raw_video_filename,
		std::string raw_audio_directory,
		std::string raw_ebucore_filename,
		bool forceFramerate, 
		int framerate_type,
		std::string timecode_value,
		std::string clipname_value,
		int64_t clipduration_value,
		bool set_avci_sequence_header,
		std::string avci_format_str,
		std::string avci_file_str,
		std::string avci_offset_str
	);

	/**
	 * @fn static ClipWriterType parse_clip_type(const char * clip_type_str)
	 * @brief This function will return the BMX ClipWriterType equivalent of the string
	 * @brief
	 * @note needs more documentation
	 * @param[in] const char * clip_type_str : string 
	 * @return ClipWriterType if all is right or an error at compilation time.
	 */
	static ClipWriterType parse_clip_type
	(
		const char * clip_type_str
	);

protected:

/////////////////////////////////
//////                     ////// 
////// protected variables //////
//////                     //////
/////////////////////////////////

enum EssenceTypeGroup
{
    NO_ESSENCE_GROUP = 0,
    DV_ESSENCE_GROUP,
    VC3_ESSENCE_GROUP,
    MPEG2LG_ESSENCE_GROUP,
    D10_ESSENCE_GROUP,
};

struct LocatorOption
{
    const char *position_str;
    AvidLocator locator;
};

struct RawInput
{
    ClipWriterType clip_type;
    EssenceTypeGroup essence_type_group;
    EssenceType essence_type;
    bool is_wave;

    ClipWriterTrack *track;

    const char *filename;
    int64_t file_start_offset;
    int64_t file_max_length;

    int64_t output_start_offset;
    int64_t output_end_offset;

    RawEssenceReader *raw_reader;
    WaveReader *wave_reader;
    uint32_t wave_track_index;

    uint32_t sample_sequence[32];
    size_t sample_sequence_size;
    size_t sample_sequence_offset;

    uint32_t track_number;
    bool track_number_set;

    // picture
    Rational aspect_ratio;
    bool aspect_ratio_set;
    uint8_t afd;
    uint32_t component_depth;
    uint32_t input_height;
    bool have_avci_header;
    bool no_d10_depad;

    // sound
    Rational sampling_rate;
    uint32_t audio_quant_bits;
    bool locked;
    bool locked_set;
    int8_t audio_ref_level;
    bool audio_ref_level_set;
    int8_t dial_norm;
    bool dial_norm_set;
};


//const char DEFAULT_SHIM_NAME[]       = "Sample File";
//const char DEFAULT_SHIM_ID[]         = "http://bbc.co.uk/rd/as02/default-shim.txt";
//const char DEFAULT_SHIM_ANNOTATION[] = "Default AS-02 shim";
//const Rational DEFAULT_SAMPLING_RATE = SAMPLING_RATE_48K;

//namespace bmx
//{
//extern bool BMX_REGRESSION_TEST;
//};
/////////////////////////////////
//////                     ////// 
////// protected functions //////
//////                     //////
/////////////////////////////////

	uint32_t read_samples
	(
		RawInput * input, 
		uint32_t max_samples_per_read
	);
	
	bool open_raw_reader
	(
		RawInput * input
	);
	
	bool open_wave_reader
	(
		RawInput * input
	);
	
	void init_input
	(
		RawInput * input
	);
	
	void copy_input
	(
		const RawInput * from, 
		RawInput * to
	);
	
	void clear_input
	(
		RawInput * input
	);
	
	bool parse_mic_type
	(
		const char * mic_type_str, 
		MICType *mic_type
	);
	static Rational set_frame_rate
	(
		int frame_rate_value
	);

};
#endif
