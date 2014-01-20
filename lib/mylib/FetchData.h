#ifndef FETCHDATA_H_
#define FETCHDATA_H_

#include "Subject.h"
#include "MyQueue.h"
#include <string>
#include <vector>
#include <curl/curl.h>

struct DownloadNode;

class FetchData : public Subject{
public:
	FetchData(){};
	//DownloadNode initialize
	FetchData(int num, std::vector<std::string> url, std::vector<std::string> path, std::vector<std::string> md5);
	//start WORK_NUM downloading threads
	void start();
	//join the downloading threads
	void join();
private:
	int TRY_NUM;
	//task queue
	MyQueue my_queue;
	//curl_perform error output and retry again
	static void error_output(DownloadNode *node, CURLcode res);
	//downloading thread
	static void *yycurl(void *ptr);
	//CURLOPT_WRITEFUNCTION, write data into the file and notify
	static size_t write_function(char *data, size_t size, size_t nmemb, void *ptr);
	//get total length of downloading file
	int get_download_file_length(std::string url);
};


#endif /* FETCHDATA_H_ */
