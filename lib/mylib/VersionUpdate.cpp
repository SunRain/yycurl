#include "VersionUpdate.h"

VersionUpdate::VersionUpdate(){
	intranet = "http://172.16.0.85/v";
	outernet = "http://retest.imobile-ent.com/v";
	loc_file_num = 0;
	res_file_num = 0;
	download_file_num = 0;
	mode = 1;
	content_path = "/tmp/";

	res_url.clear();
	res_path.clear();
	res_md5.clear();
	res_length.clear();
	local_file.clear();
	download_url.clear();
	download_path.clear();
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

void VersionUpdate::download_res(std::string res_loc){
	std::vector<std::string> res_url_tmp;
	std::vector<std::string> res_path_tmp;
	res_url_tmp.clear();
	res_path_tmp.clear();
	std::string tmp = "";
	if(mode == 1){
		tmp = intranet + version + "/res.md5";
		res_url_tmp.push_back(tmp);
	}
	else{
		tmp = outernet + version + "/res.md5";
		res_url_tmp.push_back(tmp);
	}
	res_path_tmp.push_back(res_loc);
	if(access(res_loc.c_str(), 0) == 0){
		remove(res_loc.c_str());
	}
	FetchData fetch_res(1, res_url_tmp, res_path_tmp);
	fetch_res.start();
	fetch_res.join();
	read_res(res_loc);
}

bool VersionUpdate::create_dir(std::string path, int authority){
	char str[path.size() + 1];
	strcpy(str, path.c_str());
	for(int i = 0; i < (int)path.size(); i++){
		if(path[i] == '/' && i > 0){
			str[i] = '\0';
			if(access(str, F_OK)<0){
				if(mkdir(str, authority)<0){
					return false;
				}
			}
			str[i] = '/';
		}
	}
	return true;
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
		getline(fin, path_input);
		getline(fin, md5_input);
		getline(fin, length_input);
		std::stringstream ostr;
		int length_int;
		ostr << length_input;
		ostr >> length_int;
		string_replace(path_input, " ", "%20");
		if(mode == 1){
			url_input = intranet + version + "/" + path_input;
		}
		else{
			url_input = intranet + version + "/" + path_input;
		}
		if(fin.eof())
			break;
		path_input = content_path + path_input;
		create_dir(path_input, 0755);
		res_url.push_back(url_input);
		res_path.push_back(path_input);
		res_md5.push_back(md5_input);
		res_length.push_back(length_int);
		res_file_num += 1;
	}
	fin.close();
}

std::string VersionUpdate::file_md5(std::string file_name){
	MD5_CTX md5;
	unsigned char md[16];
	char tmp[33]={};
	int length, i;
	char buffer[1024];
	std::string hash = "";
	MD5_Init(&md5);
	std::ifstream fin(file_name.c_str(), std::ios::in | std::ios::binary);
	while(!fin.eof()){
		fin.read(buffer, 1024);
		length = fin.gcount();
		if(length > 0){
			MD5_Update(&md5, buffer, length);
		}
	}
	MD5_Final(md, &md5);
	for(int i = 0; i < 16; i++){
		sprintf(tmp, "%02x", md[i]);
		hash += (std::string)tmp;
	}
	return hash;
}

int VersionUpdate::get_local_file_length(std::string path){
    std::ifstream fin;
    fin.open(path.c_str(), std::ios::app);
    fin.seekg(0, std::ios::end);
    std::streampos ps = fin.tellg();
    return (int)ps;
}

void VersionUpdate::check(std::string check_log){
	bool all_right = true;
	std::ofstream fout(check_log.c_str(), std::ios::out);
	for(int i = 0; i < res_file_num; i++){
		if(res_md5[i] != file_md5(res_path[i]) || res_length[i] != get_local_file_length(res_path[i])){
			all_right = false;
			fout << "[" << res_path[i] << "] wrong" << std::endl;
		}
		std::cout << "\033[3;1H";
		printf("Checking: %d / %d (%.2lf %%)\n", i+1, res_file_num, (double)(i + 1) *100 / res_file_num);
	}
	fout.close();
	if(all_right)
		std::cout << "All check success." << std::endl;
	else
		std::cout << "Something wrong, please check log at [" << check_log << "]." << std::endl;
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

	//map check_md5 initialize
	for(int i = 0; i < loc_file_num; i++){
		std::string md5 = file_md5(local_file[i]);
		md5_check.insert(std::pair<std::string, std::string>(local_file[i], md5));
		std::cout << "\033[1;1H";
		printf("Local file MD5 calculating: %d / %d (%.2lf %%)\n", i+1, loc_file_num, (double)(i + 1) *100 / loc_file_num);
	}
	if(loc_file_num == 0){
		std::cout << "\033[1;1H";
		printf("No local files exists.\n");
	}
	std::map<std::string, std::string>::iterator iter;
	for(int i = 0; i < res_file_num; i++){
		iter = md5_check.find(res_path[i]);
		if(iter != md5_check.end()){
			if(md5_check[res_path[i]] == res_md5[i]){
				md5_check.erase(iter);
			}
			else{
				download_url.push_back(res_url[i]);
				download_path.push_back(res_path[i]);
				download_file_num++;
			}
		}
		else{
			download_url.push_back(res_url[i]);
			download_path.push_back(res_path[i]);
			download_file_num++;
		}
		std::cout << "\033[2;1H";
		printf("Need download num: %d\n", download_file_num);
	}
	sleep(1);
	for(iter = md5_check.begin(); iter != md5_check.end(); ++iter){
		remove(iter->first.c_str());
		std::cout << "\033[2;1H";
		printf("Removing rubbish..");
	}
}
