#include "mylib/DownloadNode.h"
#include "mylib/Subject.h"
#include "mylib/Observer.h"
#include "mylib/FetchData.h"
#include "mylib/ConsoleDisplay.h"
#include <sstream>
#include <vector>
#include <string>

//if true, kill all the threads
bool STOP_ALL = false;

//used to test
void test(int &num, std::vector<std::string> &url, std::vector<std::string> &path){
	num = 100;
	for(int i = 0; i < 100; i++){
		url.push_back("http://b.zol-img.com.cn/desk/bizhi/image/1/1920x1200/1349683516475.jpg");
		std::stringstream ostr;
		ostr << i;
		std::string tmp= "/tmp/" + ostr.str() + ".jpg";
		path.push_back(tmp);
	}
}

int main(){
	//number of URLs
	int num;
	//download URL
	std::vector<std::string> url;
	//local download file path
	std::vector<std::string> path;
	test(num, url, path);
	FetchData fetch(num, url, path);
	ConsoleDisplay console(&fetch);
	//set worker thread number, default is 3
	fetch.set_work_num(5);
	fetch.start();
	console.start();
	fetch.join();
	console.join();
	return 0;
}
