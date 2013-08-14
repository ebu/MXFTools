#include "genericFeatures.hpp"

std::string genericFeatures::removePrefix
(
	std::string filename, 
	std::string prefix
) 
{
	int firstindex = filename.find_first_of(prefix);
	// returns what comes after first string "prefix"	 
	return filename.substr (firstindex+prefix.size(),filename.size()-(firstindex+prefix.size()));
}

std::string genericFeatures::removeSuffix
(
	std::string filename, 
	std::string suffix
) 
{
	int lastindex = filename.find_last_of(suffix);
	// returns what comes after string "suffix"	 
	return filename.substr (lastindex+suffix.size(),filename.size()-(lastindex+suffix.size()));
}

std::string genericFeatures::int2str
(
	int futurestring
) 
{
	std::ostringstream oss;
	// write the output stream
	oss << futurestring;
	return oss.str();
}

std::size_t genericFeatures::getTime
(
	void
) 
{
	std::chrono::high_resolution_clock::time_point p = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(p.time_since_epoch());
	return us.count();
}

bool genericFeatures::hasExtension
(
	std::string filename
) 
{
	// find last "."
	int lastindex = filename.find_last_of(".");
	// test if empty
	if (lastindex == -1) {
		return false;
	}
	// copy what comes after last dot
	std::string extension = filename.substr 
	(
		lastindex+1,
		filename.size()-(lastindex+1)
	);
	// if the length is superior to 1 character, true else false
	return (extension.size()>0) ? true : false ;
}

bool genericFeatures::isExtension
(
	std::string filename, 
	std::string extension
) 
{
	// find last "."
	int lastindex = filename.find_last_of(".");
	// copy what comes after last dot
	std::string str2 = filename.substr (lastindex+1,filename.size()-(lastindex+1));
	// loop through each character and makes it lower-case. 
	// stop at end of string (\0)
	for(int i = 0; str2[i] != '\0'; i++){
		str2[i] = tolower(str2[i]);
	}
	// return the result
	return (extension.compare(str2) == 0) ? true : false ;
}

bool genericFeatures::fileExists
(
	std::string filename
)
{
	std::ifstream f(filename.c_str());
	if (f.good()) {
		f.close(); return true;
	}
	f.close(); return false;
}

unsigned long int genericFeatures::getFileSize
(
	std::string filename
) 
{
	unsigned long int size = 0;
	FILE * fp = fopen (filename.c_str(),"rb");
	if (fp!=NULL) {
		fseek (fp, 0, SEEK_END);
		size = ftell (fp);
		fclose (fp);
  	}
	return size;
}

std::string genericFeatures::getSizeUnit
(
	unsigned long int bytes
)
{
	std::string Units[] = {"bytes","Kbytes","Mbytes",
							"Gbytes","Tbytes","Pbytes", "Ebytes"};
	short divider = 0;
	for (double i=1.0;((float)(bytes))/i>1000.0;i*=1000.0) {
		divider++;
	}
	//create a stringstream
	std::stringstream converted;
	//add number to the stream
	converted << ((float)(bytes))/pow(1000.0,divider);
	//return a string with the contents of the stream
	return converted.str()+" "+Units[divider];
}

#ifdef _WIN32
	std::string genericFeatures::wchar_t2string
	(
		const wchar_t *wchar
	)
	{
		std::string str = "";
		int index = 0;
		while(wchar[index] != 0)
		{
			str += (char)wchar[index];
			++index;
		}
		return str;
	}

	wchar_t *genericFeatures::string2wchar_t
	(
		const std::string &str
	)
	{
		 wchar_t wchar[260];
		 unsigned int index = 0;
		 while(index < str.size())
		 {
		     wchar[index] = (wchar_t)str[index];
		     ++index;
		 }
		 wchar[index] = 0;
		 return wchar;
	}
	
	std::vector<std::string> genericFeatures::listFiles
	(
		std::string dir
	)
	{
		WIN32_FIND_DATA FindFileData;
		wchar_t * FileName = string2wchar_t(dir);
		HANDLE hFind = FindFirstFile(FileName, &FindFileData);

		std::vector<std::string> listFileNames;
		listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

		while (FindNextFile(hFind, &FindFileData)) {
			listFileNames.push_back(wchar_t2string(FindFileData.cFileName));
		}
		return listFileNames;
	}
#else
	std::vector<std::string> genericFeatures::listFiles
	(
		std::string dir
	)
	{
		std::vector<std::string> PathToFiles;
		struct dirent *entry;
		DIR *dp;
	//	unsigned char isFile =0x8;
		unsigned char isFolder =0x4;

		dp = opendir(dir.c_str());
		if (dp == NULL) { return PathToFiles; }

		while ((entry = readdir(dp))) 
		{ 
			if (entry->d_type != isFolder) 
			{
				PathToFiles.push_back(dir+"/"+entry->d_name);
			}
		}
		closedir(dp);
		return PathToFiles;
	}
#endif
