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
	version.set_mode(2);
	version.set_version("15159");
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
	return 0;
}
