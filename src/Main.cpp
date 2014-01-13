#include "mylib/DownloadNode.h"
#include "mylib/Subject.h"
#include "mylib/Observer.h"
#include "mylib/FetchData.h"
#include "mylib/ConsoleDisplay.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

bool STOP_ALL = false;

//void gao(int &num, std::vector<std::string> &url, std::vector<std::string> &path){
//	std::cout << "number of download: ";
//	std::cin >> num;
//	for(int i = 0; i < num; i++){
//		std::string new_url, new_path;
//		std::cout << "url: ";
//		std::cin >> new_url;
//		std::cout << "download to: ";
//		std::cin >> new_path;
//		url.push_back(new_url);
//		path.push_back(new_path);
//	}
//}

void test(int &num, std::vector<std::string> &url, std::vector<std::string> &path){
	num = 6;
	for(int i = 0; i < 6; i++){
		url.push_back("http://download.gamed9.com/package/ttcjh/1.0/ttcjh.apk");
		std::stringstream ostr;
		ostr << i;
		std::string tmp= "/tmp/" + ostr.str();
		path.push_back(tmp);
	}
}

int main(){
	int num;
	std::vector<std::string> url;
	std::vector<std::string> path;
//	gao(num, url, path);
	test(num, url, path);
	FetchData fetch(num, url, path);
	ConsoleDisplay console(&fetch);
	fetch.set_work_num(3);
	console.start();
	fetch.start();
	fetch.join();
	console.join();
	return 0;
}
