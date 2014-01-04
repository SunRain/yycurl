#ifndef DOWNLOADNODE_H_
#define DOWNLOADNODE_H_

#include <string>

struct DownloadNode{
	std::string url;
	std::string path;
	int old_local_file_length;
	int local_file_length;
	int download_file_length;
	double download_speed;
};

#endif /* DOWNLOADNODE_H_ */
