#ifndef DOWNLOADNODE_H_
#define DOWNLOADNODE_H_

#include <string>

struct DownloadNode{
	std::string url;
	std::string path;
	std::string md5;
	//completed size
	int local_file_length;
	//total size
	int download_file_length;
	//if true, the task started
	bool begin;
	//if true, the task checked
	bool check;
	//if true, the task completed
	bool done;
	//called write function, but haven't notified
	bool buffer_is_new;
	//retry time
	int retry_time;
};

#endif /* DOWNLOADNODE_H_ */
