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
								"http://b.hiphotos.baidu.com/image/w%3D2048/sign=ee13aab2a2ec08fa260014a76dd63d6d/2934349b033b5bb5d0fa2ef334d3d539b600bc89.jpg",
								"http://zhangmenshiting.baidu.com/data2/music/106273846/1009000231389160861128.mp3?xcode=379afd47291f45b237cedfb783639284c0279ecfce43641e",
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
