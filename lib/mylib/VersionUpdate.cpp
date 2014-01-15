#include "VersionUpdate.h"
#include "FetchData.h"
#include <fstream>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

VersionUpdate::VersionUpdate(){
	intranet = "http://172.16.0.85/v";
	outernet = "http://retest.imobile-ent.com/v";
	file_num = 0;
}

void VersionUpdate::set_mode(int md){
	this->mode = md;
}

void VersionUpdate::set_version(std::string vr){
	this->version = vr;
}

void VersionUpdate::set_content_path(std::string content){
	this->content_path = content;
}

void VersionUpdate::get_res(){
	std::vector<std::string> res_url;
	std::vector<std::string> res_path;
	std::string tmp = "";
	if(mode == 1){
		tmp = intranet + version + "/res.md5";
		res_url.push_back(tmp);
	}
	else{
		tmp = outernet + version + "/res.md5";
		res_url.push_back(tmp);
	}
	res_path.push_back("/tmp/res.md5");
	FetchData fetch_res(1, res_url, res_path);
	fetch_res.start();
	fetch_res.join();
}

bool VersionUpdate::create_dir(std::string path){
	char str[path.size()+1];
	strcpy(str, path.c_str());
	for(int i=0;i<(int)path.size();i++){
		if(path[i] == '/' && i > 0){
			str[i] = '\0';
			if(access(str,F_OK)<0){
				if(mkdir(str,0755)<0){
					return false;
				}
			}
			str[i] = '/';
		}
	}
	return true;
}

void VersionUpdate::read_res(){
	std::ifstream fin;
	fin.open("/tmp/res.md5", std::ios::in);
	while(1){
		std::string path_input, md5_input, length_input, url_input;
		getline(fin, path_input);
		getline(fin, md5_input);
		getline(fin, length_input);
		if(mode == 1){
			url_input = intranet + version + "/" + path_input;
		}
		else{
			url_input = intranet + version + "/" + path_input;
		}
		if(fin.eof())
			break;
		path_input = content_path + path_input;
		create_dir(path_input);
		url.push_back(url_input);
		path.push_back(path_input);
		md5.push_back(md5_input);
		total_length.push_back(length_input);
		file_num += 1;
	}
	fin.close();
}

int VersionUpdate::get_num(){
	return file_num;
}

std::vector<std::string> VersionUpdate::get_url(){
	return url;
}

std::vector<std::string> VersionUpdate::get_path(){
	return path;
}
