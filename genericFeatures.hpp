#ifndef CPP_GENERICFEATURES_CLASS_H
#define CPP_GENERICFEATURES_CLASS_H

/*!
 * \project_name EBU Player
 * \file genericFeatures.hpp
 * \brief the generic features specifications
 * \details This class is used to define several generic functions useful to the media player or any of projet classes without instantiate the class. Basically, all public functions are declared as static to avoid the class instanciation
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 * \copyright GNU GPLv3
 *
 */

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <chrono>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <dirent.h>
	#include <sys/types.h>
#endif

/*! \class genericFeatures
 * \brief this class is used to define generic functions
 *
 * This class implements all generic functions which can be usefull to the media player or any component of the media player
 */

class genericFeatures {
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
	 * @fn static std::string removePrefix(std::string str, std::string prefix)
	 * @brief Returns the portion of the string after the first string prefix
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string filename : The filename to modify
	 * @param[in] std::string prefix   : a prefix to remove
	 * @return std::string if all is right or an error at compilation time.
	 */
	static std::string removePrefix
	(
		std::string filename, 
		std::string prefix
	);

	/**
	 * @fn static std::string removeSuffix(std::string str, std::string suffix)
	 * @brief Returns the portion of the string after the first string suffix
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string filename : The filename to modify
	 * @param[in] std::string suffix   : a suffix to remove
	 * @return std::string if all is right or an error at compilation time.
	 */
	static std::string removeSuffix
	(
		std::string filename, 
		std::string suffix
	);
	
	/**
	 * @fn static std::string int2str(int futurestring)
	 * @brief Convert an int to a std::string
	 * @brief
	 * @note needs more documentation
	 * @param[in] int futurestring   : int to convert
	 * @return std::string if all is right or an error at compilation time.
	 */
	static std::string int2str
	(
		int futurestring
	);
	
	/**
	 * @fn static std::size_t getTime(void)
	 * @brief Return the current timestamp with milliseconds and microseconds
	 * @brief Can be used like a unique id.
	 * @note needs more documentation
	 * @param[in] void : no arg
	 * @return std::size_t if all is right or an error at compilation time.
	 */
	static std::size_t getTime
	(
		void
	);
	
	/**
	 * @fn static bool hasExtension(std::string filename)
	 * @brief This function will test if the input file has an extension.
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string filename : The filename to test
	 * @return std::string if all is right or an error at compilation time.
	 */
	static bool hasExtension
	(
		std::string filename
	);

	/**
	 * @fn static bool isExtension(std::string filename, std::string extension)
	 * @brief This function will test if the input string ends by the extension. The extension should come after a dot.
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string filename  : The filename to test
	 * @param[in] std::string extension : Extension file to test
	 * @return std::string if all is right or an error at compilation time.
	 */
	static bool isExtension
	(
		std::string filename, 
		std::string extension
	);

	/**
	 * @fn static bool fileExists(std::string filename)
	 * @brief This function will test if the input file exists.
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string filename : Path to the file to test
	 * @return std::string if all is right or an error at compilation time.
	 */
	static bool fileExists
	(
		std::string filename
	);

	/**
	 * @fn static unsigned long int getFileSize(std::string filename)
	 * @brief This function will compute the file size in bytes.
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string filename : Path to the file to test
	 * @return unsigned long int if all is right or an error at compilation time.
	 */
	static unsigned long int getFileSize
	(
		std::string filename
	);

	/**
	 * @fn static std::string getSizeUnit(unsigned long int bytes)
	 * @brief This function will identify the most appropriate unit prefix to filesize
	 * @brief
	 * @note needs more documentation
	 * @param[in] unsigned long int bytes : Filesize in bytes
	 * @return std::string if all is right or an error at compilation time.
	 */
	static std::string getSizeUnit
	(
		unsigned long int bytes
	);

	#ifdef _WIN32
		static std::string wchar_t2string
		(
			const wchar_t *wchar
		);
		
		static wchar_t *string2wchar_t
		(
			const std::string &str
		);
	#endif
	/**
	 * @fn static std::vector<std::string> listFiles(std::string dir)
	 * @brief This function will generate a vector with the path to each files into the folder as a standard string.
	 * @brief
	 * @note needs more documentation
	 * @param[in] std::string dir : Path to the folder
	 * @return unsigned long int if all is right or an error at compilation time.
	 */
	static std::vector<std::string> listFiles
	(
		std::string dir
	);
	
protected:

/////////////////////////////////
//////                     ////// 
////// protected variables //////
//////                     //////
/////////////////////////////////

/////////////////////////////////
//////                     ////// 
////// protected functions //////
//////                     //////
/////////////////////////////////
	
};
#endif
