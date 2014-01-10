#include "mylib/DownloadNode.h"
#include "mylib/Subject.h"
#include "mylib/Observer.h"
#include "mylib/FetchData.h"
#include "mylib/ConsoleDisplay.h"
#include <iostream>

bool STOP_ALL = false;

int main(){
	std::string path[3] = 	{"/tmp/curl-test0", "/tmp/curl-test1", "/tmp/curl-test2"};
	std::string url[3] = 	{
								"http://download.gamed9.com/package/ttcjh/1.0/ttcjh.apk",
								"http://download.gamed9.com/package/ttcjh/1.0/ttcjh.apk",
								"http://pica.nipic.com/2008-07-25/2008725111428552_2.jpg"
							};
	FetchData fetch(url, path);
	ConsoleDisplay console(&fetch);
	fetch.set_work_num(3);
	fetch.start();
	console.start();
	fetch.join();
	console.join();
	return 0;
}
