#include "VersionUpdate.h"

VersionUpdate::VersionUpdate(){
	intranet = "http://172.16.0.85/v";
	outernet = "http://retest.imobile-ent.com/v";
	file_num = 0;
	mode = 1;
	content_path = "/tmp/";
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
	std::cout << tmp << std::endl;
	res_path.push_back("/tmp/res.md5");
	FetchData fetch_res(1, res_url, res_path);
	fetch_res.start();
	fetch_res.join();
	read_res();
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

void VersionUpdate::string_replace(std::string& strBig, const std::string &strsrc, const std::string &strdst)
{
     std::string::size_type pos = 0;
     while( (pos = strBig.find(strsrc, pos)) != std::string::npos)
     {
         strBig.replace(pos, strsrc.length(), strdst);
         pos += strdst.length();
     }
}

void VersionUpdate::read_res(){
	std::ifstream fin;
	fin.open("/tmp/res.md5", std::ios::in);
	while(1){
		std::string path_input, md5_input, length_input, url_input;
		getline(fin, path_input);
		getline(fin, md5_input);
		getline(fin, length_input);
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

void VersionUpdate::check(std::string check_path){
	bool all_right = true;
	std::ofstream fout(check_path.c_str(), std::ios::out);
	for(int i = 0; i < file_num; i++){
		if(md5[i] != file_md5(path[i])){
			all_right = false;
			fout << "md5 wrong: " << path[i] << std::endl;
		}
		std::stringstream ostr;
		ostr << get_local_file_length(path[i]);
		if(total_length[i] != ostr.str()){
			all_right = false;
			fout << "len wrong: " << path[i] << std::endl;
		}
	}
	fout.close();
	if(all_right){
		std::cout << "all md5 and length check success." << std::endl;
	}
	else{
		std::cout << "there is something wrong. Plese check " << check_path << "." << std::endl;
	}
}
