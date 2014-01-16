//during the downloding, the file will be named as *.yytmp
//under /tmp/ , there are yycurl.error and yycurl.log
//yycurl.error shows errors during downloading
//yycurl.log shows libcurl downloading log
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
	//mode 1 means intranet download, others mean outernet download
	version.set_mode(1);
	//set download version
	version.set_version("15166");
	//set download directory,
	version.set_content_path("/tmp/");
	//set the download path of res.md5
	version.download_res("/tmp/res.md5");
	//set path of update log
	version.update("/tmp/update.log");
	//initialize fetching data
	FetchData fetch(version.get_download_num(),
					version.get_download_url(),
					version.get_download_path());
	//set number of worker thread, default is 3
	fetch.set_work_num(5);
	ConsoleDisplay console(&fetch);
	//start downloading
	fetch.start();
	//start showing of downloading process
	console.start();
	fetch.join();
	console.join();
	//length and md5 checking, according to res.md5
	version.check("/tmp/check.log");
	return 0;
}
