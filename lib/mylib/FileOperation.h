#ifndef FILEOPERATION_H_
#define FILEOPERATION_H_

#include <string>

class FileOperation{
public:
	FileOperation(std::string path);
	void set_path(std::string path);
	std::string get_path();
	int get_file_length();
	std::string get_file_md5();
	//In value of 'authority' permissions to create the file at 'path'
	bool make_dir(int authority);
private:
	std::string path;
};


#endif
