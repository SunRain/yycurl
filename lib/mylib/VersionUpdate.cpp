#include "FileOperation.h"
#include "VersionUpdate.h"
#include "FetchData.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sstream>

extern bool STOP_ALL;

VersionUpdate::VersionUpdate(){
	//some default setting
	intranet = "http://172.16.0.85/v";
	outernet = "http://retest.imobile-ent.com/v";
	loc_file_num = 0;
	res_file_num = 0;
	download_file_num = 0;
	mode = 1;
	content_path = "/tmp/";

	//vectors initialize
	res_url.clear();
	res_path.clear();
	res_md5.clear();
	res_length.clear();
	local_file.clear();
	download_url.clear();
	download_path.clear();
	download_md5.clear();
	md5_check.clear();
}

void VersionUpdate::set_mode(int md){
	this->mode = md;
}

void VersionUpdate::set_version(std::string vr){
	this->version = vr;
}

void VersionUpdate::set_content_path(std::string content){
	int length = content.size();

	//if content doesn't end with '/', add it
	if(content[length-1] != '/'){
		content = content + "/";
	}
	this->content_path = content;
}

int VersionUpdate::get_download_num(){
	return download_file_num;
}

std::vector<std::string> VersionUpdate::get_download_url(){
	return download_url;
}

std::vector<std::string> VersionUpdate::get_download_path(){
	return download_path;
}

std::vector<std::string> VersionUpdate::get_download_md5(){
	return download_md5;
}

void VersionUpdate::download_res(std::string res_loc){
	std::vector<std::string> res_url_tmp;
	std::vector<std::string> res_path_tmp;
	std::vector<std::string> res_md5_tmp;
	res_url_tmp.clear();
	res_path_tmp.clear();
	res_md5_tmp.clear();
	std::string tmp = "";
	if(mode == 1){
		//download mode 1, from intranet
		tmp = intranet + version + "/res.md5";
		res_url_tmp.push_back(tmp);
	}
	else{
		//download mode 2, from outernet
		tmp = outernet + version + "/res.md5";
		res_url_tmp.push_back(tmp);
	}
	res_path_tmp.push_back(res_loc);

	//if '/tmp/res.md5' exits, delete it
	if(access(res_loc.c_str(), 0) == 0){
		remove(res_loc.c_str());
	}

	res_md5_tmp.push_back("");

	//download res.md5
	FetchData fetch_res(1, res_url_tmp, res_path_tmp, res_md5_tmp);
	fetch_res.start();
	fetch_res.join();

	//read from res.md5
	read_res(res_loc);
}

void VersionUpdate::string_replace(std::string& strBig, const std::string &strsrc, const std::string &strdst)
{
     std::string::size_type pos = 0;
     while( (pos = strBig.find(strsrc, pos)) != std::string::npos)
     {
         strBig.replace(pos, strsrc.length(), strdst);
         pos += strdst.length();
     }
}

void VersionUpdate::read_res(std::string res_loc){
	std::ifstream fin;
	fin.open(res_loc.c_str(), std::ios::in);
	while(1){
		std::string path_input, md5_input, length_input, url_input;
		//read three lines
		getline(fin, path_input);
		getline(fin, md5_input);
		getline(fin, length_input);

		//change length_input from string to integer
		std::stringstream ostr;
		int length_int;
		ostr << length_input;
		ostr >> length_int;

		//replace space to %20 in url
		string_replace(path_input, " ", "%20");

		if(mode == 1){
			url_input = intranet + version + "/" + path_input;
		}
		else{
			url_input = outernet + version + "/" + path_input;
		}

		//get to the end of res.md5, jump out
		if(fin.eof())
			break;

		path_input = content_path + path_input;

		//create the directory
		FileOperation file(path_input);
		bool create = file.make_dir(0755);

		//if create directory failed
		if(!create){
			std::ofstream fout;
			fout.open("/tmp/yycurl.error", std::ios::app);
			fout << "create directory [" << path_input << "] error!" << std::endl;
			fout.close();
		}

		//data update
		res_url.push_back(url_input);
		res_path.push_back(path_input);
		res_md5.push_back(md5_input);
		res_length.push_back(length_int);
		res_file_num += 1;
	}
	fin.close();
}

void VersionUpdate::get_all_file(std::string path){
	DIR *dir;
	struct dirent *dir_entity;
	dir = opendir(path.c_str());
	while(dir_entity = readdir(dir)){
		if(dir_entity->d_type == 0x8){
			//file
			std::string full_name = path + "/" + std::string(dir_entity->d_name);
			local_file.push_back(full_name);
		}
		if(dir_entity->d_type == 0x4){
			if(strcmp(dir_entity->d_name, ".") == 0 || strcmp(dir_entity->d_name, "..") == 0){
				continue;
			}
			//directory
			std::string full_path = path + "/" + std::string(dir_entity->d_name);
			get_all_file(full_path);
		}
	}
	loc_file_num = local_file.size();
}

void VersionUpdate::update(std::string update_log){
	//console clear
	std::string clr = "\033[2J";
	std::cout << clr;
	get_all_file(content_path+"data");

	std::ofstream fout(update_log.c_str(), std::ios::out);

	//map check_md5 initialize
	for(int i = 0; i < loc_file_num; i++){
		FileOperation file(local_file[i]);
		md5_check.insert(std::pair<std::string, std::string>(local_file[i], file.get_file_md5()));
		std::cout << "\033[1;1H";
		printf("Local file MD5 calculating: %d / %d (%.2lf %%)\n", i+1, loc_file_num, (double)(i + 1) *100 / loc_file_num);
	}

	//if no local file
	if(loc_file_num == 0){
		std::cout << "\033[1;1H";
		printf("No local files exists.\n");
	}

	//get which url should be downloaded
	std::map<std::string, std::string>::iterator iter;
	for(int i = 0; i < res_file_num; i++){
		iter = md5_check.find(res_path[i]);
		if(iter != md5_check.end()){
			//file exits
			if(md5_check[res_path[i]] == res_md5[i]){
				//needn't to be updated
				md5_check.erase(iter);
			}
			else{
				//file should be updated
				download_url.push_back(res_url[i]);
				download_path.push_back(res_path[i]);
				download_md5.push_back(res_md5[i]);
				download_file_num++;
			}
		}
		else{
			//file not exists, should be downloaded
			download_url.push_back(res_url[i]);
			download_path.push_back(res_path[i]);
			download_md5.push_back(res_md5[i]);
			download_file_num++;
		}
		std::cout << "\033[2;1H";
		printf("The number of file need to be updated: %d\n", download_file_num);
	}

	//wait a second, to let you see the number clearly
	sleep(1);

	//removing resources which are useless
	for(iter = md5_check.begin(); iter != md5_check.end(); ++iter){
		//print log
		fout << "remove:   " << iter->first << std::endl;
		remove(iter->first.c_str());
		std::cout << "\033[2;1H";
		printf("Removing rubbish..");
	}

	//print log
	for(int i = 0; i < (int)download_path.size(); i++){
		fout << "download: " << download_path[i] << std::endl;
	}
	fout.close();
}
