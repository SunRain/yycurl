#ifndef FETCHDATA_H_
#define FETCHDATA_H_


#include "Subject.h"
#include "DownloadNode.h"
#include <fstream>
<<<<<<< HEAD
#include <string>
=======
#include <vector>
#include <string>
#include <pthread.h>
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
#include <curl/curl.h>
#include <sys/time.h>

class FetchData : public Subject{
public:
	FetchData(){};
<<<<<<< HEAD
	FetchData(std::string url[], std::string path[]);
	int get_download_file_length(std::string &url);
	void start();
	void join();
private:
=======
	FetchData(std::vector<std::string> url, std::vector<std::string> path);
	int get_download_file_length(std::string url);
private:
	std::vector<pthread_t> fetch_thread;
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
	static void error_output(CURLcode res);
	static void *yycurl(void *ptr);
	static size_t write_function(char *data, size_t size, size_t nmemb, void *ptr);
};


#endif /* FETCHDATA_H_ */
