#ifndef VERSIONUPDATE_H_
#define VERSIONUPDATE_H_

#include "FetchData.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <dirent.h>
#include <openssl/md5.h>
#include <map>

class VersionUpdate{
public:
	VersionUpdate();
	//set download mode, mode 1 means intranet download, others mean outernet download
	//intranet url is like: http://172.16.0.85/v15166/res.md5
	//outernet url is like:
	void set_mode(int md);
	//set download version to be vr
	void set_version(std::string vr);
	//set download content, you should promise all resources is under data/
	void set_content_path(std::string conent_path);
	//get the vector download_url
	std::vector<std::string> get_download_url();
	//get the vector download_path
	std::vector<std::string> get_download_path();
	//get download_file_num
	int get_download_num();
	//download the file res.md5 to res_loc
	void download_res(std::string res_loc);
	//check md5 and length of all files in the res.md5, the location of error log is check_log
	void check(std::string check_log);
	//update
	void update(std::string update_path);
private:
	//mode number, set by VersionUpdate::set_mode(int md)
	int mode;
	//number of files in res.md5, updated in VersionUpdate::read_res(std::string res_loc)
	int res_file_num;
	//local files under content_path/data/, updated in VersionUpdate::get_all_file(std::string path)
	int loc_file_num;
	//number of files need to be downloaded, updated in VersionUpdate::update(std::string update_log)
	int download_file_num;
	//version number, set by VersionUpdate::set_version(std::string vr)
	std::string version;
	//download directory, set by VersionUpdate::set_content_path(std::string conent_path)
	std::string content_path;
	//intranet url, initialized in VersionUpdate::VersionUpdate()
	std::string intranet;
	//outernet url, initialized in VersionUpdate::VersionUpdate()
	std::string outernet;
	//storing urls in res.md5
	std::vector<std::string> res_url;
	//storing paths in res.md5
	std::vector<std::string> res_path;
	//storing md5s in res.md5
	std::vector<std::string> res_md5;
	//storing lengths in res.md5
	std::vector<int> res_length;
	//storing path of local file under content_path/data
	std::vector<std::string> local_file;
	//storing urls need to be downloaded
	std::vector<std::string> download_url;
	//storing paths need to be downloaded
	std::vector<std::string> download_path;
	//used to judging which file need to be updated, used in VersionUpdate::update(std::string update_log)
	std::map<std::string, std::string> md5_check;
	//input file_name, return md5(hex) of it which is lower cases
	std::string file_md5(std::string file_name);
	//read res.md5, and initialize res_url, res_path, res_md5, res_length
	void read_res(std::string res_loc);
	//In value of 'authority' permissions to create the file at 'path'
	bool create_dir(std::string path, int authority);
	//replace strsrc into strdst in the strBig
	void string_replace(std::string& strBig, const std::string &strsrc, const std::string &strdst);
	//get local file length of path
	int get_local_file_length(std::string path);
	//get all files under path, and update the vector local_file
	void get_all_file(std::string path);
};

#endif /* VERSIONUPDATE_H_ */
