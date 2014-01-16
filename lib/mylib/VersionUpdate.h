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
	void set_mode(int md);
	void set_version(std::string vr);
	void set_content_path(std::string conent_path);
	std::vector<std::string> get_download_url();
	std::vector<std::string> get_download_path();
	int get_download_num();
	void download_res(std::string res_loc);
	void check(std::string check_log);
	void update(std::string update_path);
private:
	int mode;
	int res_file_num;
	int loc_file_num;
	int download_file_num;
	std::string version;
	std::string content_path;
	std::string intranet;
	std::string outernet;
	std::vector<std::string> res_url;
	std::vector<std::string> res_path;
	std::vector<std::string> res_md5;
	std::vector<int> res_length;
	std::vector<std::string> local_file;
	std::vector<std::string> download_url;
	std::vector<std::string> download_path;
	std::map<std::string, std::string> md5_check;
	std::string file_md5(std::string file_name);
	void read_res(std::string res_loc);
	bool create_dir(std::string path, int authority);
	void string_replace(std::string& strBig, const std::string &strsrc, const std::string &strdst);
	int get_local_file_length(std::string path);
	void get_all_file(std::string path);
};

#endif /* VERSIONUPDATE_H_ */
