#include "mylib/DownloadNode.h"
#include "mylib/Subject.h"
#include "mylib/Observer.h"
#include "mylib/FetchData.h"
#include "mylib/ConsoleDisplay.h"
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> url;
std::vector<std::string> auto_path;
std::vector<DownloadNode> shared_node;
pthread_mutex_t count_lock;
pthread_cond_t count_nonzero;
unsigned count;
struct timeval time_start;

void initialize(){
	url.clear();
	auto_path.clear();

	/*url initialize*/
	url.push_back("http://www.sina.com.cn");
	url.push_back("http://www.baidu.com");
	url.push_back("http://pica.nipic.com/2008-07-25/2008725111428552_2.jpg");

	/*local path generation*/
	for(int i = 0; i < (int)url.size(); i++){
		std::stringstream ostr;
		ostr << i;
		std::string path = "/tmp/curl-test" + ostr.str();
		auto_path.push_back(path);
	}

	/* Must initialize libcurl before any threads are started */
	curl_global_init(CURL_GLOBAL_ALL);
}

int main(){
	initialize();
	ConsoleDisplay console;
	FetchData fetch_data(url, auto_path);
	return 0;
}
