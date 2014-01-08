#ifndef DOWNLOADNODE_H_
#define DOWNLOADNODE_H_

#include <string>

struct DownloadNode{
<<<<<<< HEAD
	int id;
	std::string url;
	std::string path;
	int local_file_length;
	int download_file_length;
=======
	std::string url;
	std::string path;
	int old_local_file_length;
	int local_file_length;
	int download_file_length;
	double download_speed;
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
};

#endif /* DOWNLOADNODE_H_ */
