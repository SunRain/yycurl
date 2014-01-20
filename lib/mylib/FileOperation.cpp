#include "FileOperation.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <string.h>

FileOperation::FileOperation(std::string path){
	set_path(path);
}

void FileOperation::set_path(std::string path){
	this->path = path;
}

std::string FileOperation::get_path(){
	return path;
}

std::string FileOperation::get_file_md5(){
	MD5_CTX md5;
	unsigned char md[16];
	char tmp[33]={};
	int length, i;
	char buffer[1024];
	std::string hash = "";
	MD5_Init(&md5);

	//open file in binary
	std::ifstream fin(path.c_str(), std::ios::in | std::ios::binary);
	while(!fin.eof()){
		//read 1024 byte
		fin.read(buffer, 1024);
		length = fin.gcount();
		if(length > 0){
			//if read s.th., update md5
			MD5_Update(&md5, buffer, length);
		}
	}
	MD5_Final(md, &md5);

	//change md5 to HEX
	for(int i = 0; i < 16; i++){
		sprintf(tmp, "%02x", md[i]);
		hash += (std::string)tmp;
	}
	return hash;
}

int FileOperation::get_file_length(){
    std::ifstream fin;
    fin.open(path.c_str(), std::ios::app);
    fin.seekg(0, std::ios::end);
    std::streampos ps = fin.tellg();
    return (int)ps;
}

bool FileOperation::make_dir(int authority){
	char str[path.size() + 1];
	strcpy(str, path.c_str());
	for(int i = 0; i < (int)path.size(); i++){
		if(path[i] == '/' && i > 0){
			str[i] = '\0';
			//flag F_OK meaning test for existence of the file.
			if(access(str, F_OK)<0){
				//if not exists, mkdir
				if(mkdir(str, authority)<0){
					return false;
				}
			}
			str[i] = '/';
		}
	}
	return true;
}
