/*!
 * \project_name EBU Player
 * \file EBUCoreFeatures.cpp
 * \brief EBUCore wrapping features
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 28 march 2013
 * \copyright This software is published under MPL v2.0
 *
 */


#include "EBUCoreFeatures.hpp"


EBUCoreFeatures::EBUCoreFeatures
(
	void
) 
{
}

EBUCoreFeatures::~EBUCoreFeatures
(
	void
)
{	
}

void EBUCoreFeatures::UnwrapEBUCore
(
	std::string MXFFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// extract ebucore metadata
	try
	{
		dom_doc = &EBUSDK::EBUCore::ExtractEBUCoreMetadata
		(
			MXFFilename.c_str(), 
			&progress_cb
		);
		 dom_root = dom_doc->getDocumentElement();
	}
	catch (std::exception& e)
	{
		std::cout << "Standard exception: " << e.what() << std::endl;
	}
}

void EBUCoreFeatures::UnwrapEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreOutputFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// extract ebucore metadata
	EBUSDK::EBUCore::ExtractEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		XMLEBUCoreOutputFilename.c_str(), 
		&progress_cb
	);
}

void EBUCoreFeatures::eraseEBUCore
(
	std::string MXFFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// remove ebucore metadatas
	EBUSDK::EBUCore::RemoveEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		&progress_cb
	);
}

void EBUCoreFeatures::eraseEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreOutputFilename
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	// extract ebucore metadatas
	EBUSDK::EBUCore::ExtractEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		XMLEBUCoreOutputFilename.c_str(), 
		&progress_cb
	);
	// remove ebucore metadatas
	EBUSDK::EBUCore::RemoveEBUCoreMetadata
	(
		MXFFilename.c_str(), 
		&progress_cb
	);
}

void EBUCoreFeatures::WrapEBUCore
(
	std::string MXFFilename,
	std::string XMLEBUCoreFilename,
	bool setHeader,
	bool isDark,
	bool isSidecar
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	EBUSDK::EBUCore::EmbedEBUCoreMetadata
	(
		XMLEBUCoreFilename.c_str(), 
		MXFFilename.c_str(), 
		&progress_cb, 
		(EBUSDK::EBUCore::MetadataKind)(isSidecar) ? 
			EBUSDK::EBUCore::SIDECAR 
		: ((isDark) ? 
			EBUSDK::EBUCore::DARK : 
			EBUSDK::EBUCore::KLV_ENCODED
		), 
		false, 
		setHeader
	);
}

void EBUCoreFeatures::WrapEBUCore
(
	std::string MXFFilename,
	bool setHeader,
	bool isDark,
	bool isSidecar,
	std::string XMLEBUCoreFilename 
)
{
	// init bmx
	bmx::connect_libmxf_logging();
	EBUSDK::EBUCore::EmbedEBUCoreMetadata
	(
		*dom_doc,
		XMLEBUCoreFilename.c_str(), 
		MXFFilename.c_str(), 
		&progress_cb, 
		(EBUSDK::EBUCore::MetadataKind)(isSidecar) ? 
			EBUSDK::EBUCore::SIDECAR 
		: ((isDark) ? 
			EBUSDK::EBUCore::DARK : 
			EBUSDK::EBUCore::KLV_ENCODED
		), 
		false, 
		setHeader
	);
}



xercesc::DOMDocument * EBUCoreFeatures::getEBUCore
(
	void
)
{
	return dom_doc;
}

bool EBUCoreFeatures::hasEBUCore
(
	void
)
{
	return (dom_doc != 0);
}

void EBUCoreFeatures::setEBUCore
(
	xercesc::DOMDocument * new_dom_doc
)
{
	dom_doc = new_dom_doc;
}


void EBUCoreFeatures::progress_cb
(
	float progress, 
	EBUSDK::EBUCore::ProgressCallbackLevel level, 
	const char *function, 
	const char *msg_format, 
	...
) 
{
	// append a newline to the msg_format string
	int len = strlen(msg_format);
	char* newline_msg_format =(char*)malloc((len+2) * sizeof(char));
	strncpy(newline_msg_format, msg_format, len); newline_msg_format[len] = 0;
	strncat(newline_msg_format, "\n", 1);

	va_list p_arg;
    va_start(p_arg, msg_format);
	switch (level) {
	case EBUSDK::EBUCore::INFO:
		bmx::log_info(newline_msg_format, p_arg); return;
	case EBUSDK::EBUCore::DEBUG:
	case EBUSDK::EBUCore::TRACE:
		bmx::log_debug(newline_msg_format, p_arg); return;
	case EBUSDK::EBUCore::ERROR:
		bmx::log_error(newline_msg_format, p_arg); return;
	case EBUSDK::EBUCore::WARN:
		bmx::log_warn(newline_msg_format, p_arg); return;
	default :
		bmx::log_warn(newline_msg_format, p_arg);
	};
	va_end(p_arg);

	free(newline_msg_format);
}
