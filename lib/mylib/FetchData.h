#ifndef FETCHDATA_H_
#define FETCHDATA_H_


#include "Subject.h"
#include "DownloadNode.h"
#include <fstream>
#include <vector>
#include <string>
#include <pthread.h>
#include <curl/curl.h>
#include <sys/time.h>

class FetchData : public Subject{
public:
	FetchData(){};
	FetchData(std::vector<std::string> url, std::vector<std::string> path);
	int get_download_file_length(std::string url);
private:
	std::vector<pthread_t> fetch_thread;
	static void error_output(CURLcode res);
	static void *yycurl(void *ptr);
	static size_t write_function(char *data, size_t size, size_t nmemb, void *ptr);
};


#endif /* FETCHDATA_H_ */
