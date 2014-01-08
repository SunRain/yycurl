#ifndef DOWNLOADNODE_H_
#define DOWNLOADNODE_H_

#include <string>

struct DownloadNode{
	int id;
	std::string url;
	std::string path;
	int local_file_length;
	int download_file_length;
};

#endif /* DOWNLOADNODE_H_ */
