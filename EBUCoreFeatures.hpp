#ifndef CPP_EBUCOREFEATURES_CLASS_H
#define CPP_EBUCOREFEATURES_CLASS_H

/*!
 * \project_name EBU Player
 * \file EBUCoreFeatures.hpp
 * \brief EBUCore features specifications
 * \details This class is used to load the EBUCore features. Basically, it will encapsulate and desencapsulate the EBUCore metadatas into the MXF file
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 28 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */

#include <bmx/mxf_reader/MXFFileReader.h>
#include <bmx/mxf_reader/MXFGroupReader.h>
#include <bmx/mxf_reader/MXFSequenceReader.h>
#include <bmx/mxf_reader/MXFFrameMetadata.h>
#include <bmx/essence_parser/SoundConversion.h>
#include <bmx/MD5.h>
#include <bmx/CRC32.h>
#include <bmx/MXFUtils.h>
#include <bmx/Utils.h>
#include <bmx/Version.h>
#include <bmx/as11/AS11Info.h>
#include <bmx/BMXException.h>
#include <bmx/Logging.h>

#ifdef _WIN64	
	#include <mxf/mxf_win32_file.h>
   //define libraries for Windows (64-bit)
#elif _WIN32
	#include <mxf/mxf_win32_file.h>
//#elif __APPLE__

//    #if TARGET_OS_IPHONE    
         // iOS device
//    #elif TARGET_IPHONE_SIMULATOR
        // iOS Simulator
//    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
//    #else
        // Unsupported platform
//    #endif
#elif __linux
	#include <EBUCoreProcessor.h>
    // linux
//#elif __unix // all unices not caught above
    // Unix
//#elif __posix
    // POSIX
#endif

#include <xercesc/dom/DOM.hpp>
#include <exception>

#include "genericFeatures.hpp"


/*! \class EBUCore Features
 * \brief this class represent the EBUCore Wrapping features
 *
 * This class controls all the EBUCore Wrapping features
 */

class EBUCoreFeatures {
	public:
		/*!*
		* @brief Class constructor
		* @brief The EBUCoreFeatures class constructor instantiates a new EBUCore Wrapper and all its features.
		*/
		EBUCoreFeatures
		(
			void
		);
		/*!*
		* @brief Class destructor
		* @brief The EBUCoreFeatures class destructor destroys all variables and instanciate objects. This is the garbage collector class. It is always called at the end of class life.
		*
		*/
		virtual ~EBUCoreFeatures
		(
			void
		);
	
		void UnwrapEBUCore
		(
			std::string MXFFilename
		);
		
		void UnwrapEBUCore
		(
			std::string MXFFilename,
			std::string XMLEBUCoreOutputFilename
		);
		
		void eraseEBUCore
		(
			std::string MXFFilename
		);
		
		void eraseEBUCore
		(
			std::string MXFFilename,
			std::string XMLEBUCoreOutputFilename
		);
		
		void WrapEBUCore
		(
			std::string MXFFilename,
			std::string XMLEBUCoreFilename,
			bool setHeader,
			bool isDark,
			bool isSidecar
		);
		
		void WrapEBUCore
		(
			std::string MXFFilename,
			bool setHeader,
			bool isDark,
			bool isSidecar,
			std::string XMLEBUCoreFilename = ""
		);
		
		void WrapEBUCore
		(
			xercesc::DOMDocument * EBUCoreMetadata,
			std::string MXFFilename,
			bool setHeader,
			bool isDark,
			bool isSidecar,
			std::string XMLEBUCoreFilename = ""
		);
		
		xercesc::DOMDocument * getEBUCore
		(
			void
		);

		bool hasEBUCore
		(
			void
		);
		
		void setEBUCore
		(
			xercesc::DOMDocument * new_dom_doc
		);

		static void progress_cb
		(
			float progress, 
			EBUSDK::EBUCore::ProgressCallbackLevel level, 
			const char *function, 
			const char *msg_format, 
			...
		);
		
	protected:
	
		xercesc::DOMDocument * dom_doc;
		xercesc::DOMElement*  dom_root;
};

#endif
