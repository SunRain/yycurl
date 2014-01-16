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
	version.set_version("15163");
	version.set_content_path("/tmp/");
	version.download_res("/tmp/res.md5");
	version.update("/tmp/update.log");
	FetchData fetch(version.get_download_num(),
					version.get_download_url(),
					version.get_download_path());
	fetch.set_work_num(5);
	ConsoleDisplay console(&fetch);
	fetch.start();
	console.start();
	fetch.join();
	console.join();
	version.check("/tmp/check.log");
	return 0;
}
