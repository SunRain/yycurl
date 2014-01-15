#ifndef FETCHDATA_H_
#define FETCHDATA_H_


#include "Subject.h"
#include "DownloadNode.h"
#include "MyQueue.h"
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <curl/curl.h>
#include <sys/time.h>

class FetchData : public Subject{
public:
	FetchData(){};
	//DownloadNode initialize
	FetchData(int num, std::vector<std::string> url, std::vector<std::string> path);
	//get total length of downloading file
	int get_download_file_length(std::string url);
	//start WORK_NUM downloading threads
	void start();
	//join the downloading threads
	void join();
private:
	//task queue
	MyQueue my_queue;
	//curl_perform error output and retry again
	static void error_output(DownloadNode *node, CURLcode res);
	//downloading thread
	static void *yycurl(void *ptr);
	//CURLOPT_WRITEFUNCTION, write data into the file and notify
	static size_t write_function(char *data, size_t size, size_t nmemb, void *ptr);
};


#endif /* FETCHDATA_H_ */
