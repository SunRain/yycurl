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
#include <openssl/md5.h>

class VersionUpdate{
public:
	VersionUpdate();
	void set_mode(int md);
	void set_version(std::string vr);
	void set_content_path(std::string conent_path);
	void get_res();
	void read_res();
	int get_num();
	std::vector<std::string> get_url();
	std::vector<std::string> get_path();
	bool create_dir(std::string path);
	std::string file_md5(std::string file_name);
	int get_local_file_length(std::string path);
	void check(std::string check_path);
	void update(std::string check_path);
	void string_replace(std::string& strBig, const std::string &strsrc, const std::string &strdst);
private:
	int mode;
	int file_num;
	std::string version;
	std::string content_path;
	std::string intranet;
	std::string outernet;
	std::vector<std::string> url;
	std::vector<std::string> path;
	std::vector<std::string> md5;
	std::vector<std::string> total_length;
};

#endif /* VERSIONUPDATE_H_ */
