#include "work.h"
#include "gabor.h"

string int2str(int i){
	stringstream ss;
	ss << i;
	return ss.str();
}

vector<string> get_file_names(string name){
	char filename[150];
	vector<string> names;
	string folder = name+"/*.*";
	string folderwithout = name+"/";

	size_t origsize = folder.size() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, folder.c_str(), _TRUNCATE);

    WIN32_FIND_DATA FindFileData;
    HANDLE FileHandle;
	FileHandle = FindFirstFile(wcstring, &FindFileData);
    WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL ); 
	SetLastError(0);
	int count =0;
	while(GetLastError()!=ERROR_NO_MORE_FILES){	
		if(count++>1){
			string dir = folderwithout+filename; 
			names.push_back(dir);
		}
		FindNextFile(FileHandle,&FindFileData);
		WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL );
	}
	return names;

}

vector<string> get_real_file_names(string name){
	char filename[150];
	vector<string> names;
	string folder = name+"/*.*";
	string folderwithout = name+"/";

	size_t origsize = folder.size() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, folder.c_str(), _TRUNCATE);

    WIN32_FIND_DATA FindFileData;
    HANDLE FileHandle;
	FileHandle = FindFirstFile(wcstring, &FindFileData);
    WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL ); 
	SetLastError(0);
	int count = 0;
	while(GetLastError()!=ERROR_NO_MORE_FILES){	
		if(count++>1)
			names.push_back(filename);
		FindNextFile(FileHandle,&FindFileData);
		WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL );
	}
	return names;

}
