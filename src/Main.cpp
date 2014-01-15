#include "mylib/DownloadNode.h"
#include "mylib/Subject.h"
#include "mylib/Observer.h"
#include "mylib/FetchData.h"
#include "mylib/ConsoleDisplay.h"
#include "mylib/VersionUpdate.h"
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

//if true, kill all the downloading threads and the showing thread
bool STOP_ALL = false;


int main(){
	VersionUpdate version;
	version.set_mode(1);
	version.set_version("15166");
	version.set_content_path("/tmp/");
	version.get_res();
	FetchData fetch(version.get_num(), version.get_url(), version.get_path());
	fetch.set_work_num(5);
	ConsoleDisplay console(&fetch);
	fetch.start();
	console.start();
	fetch.join();
	console.join();
	version.check("/tmp/check.log");
//	std::vector<std::string> url,path;
//	url.push_back("http://172.16.0.85/v15166/data/image/ui/huodongdating/qcbl_wenzi .png");
//	path.push_back("/tmp/qcbl_wenzi .png");
//	FetchData fetch(1, url, path);
//	ConsoleDisplay console(&fetch);
//	fetch.start();
//	console.start();
//	fetch.join();
//	console.join();
	return 0;
}
