#ifndef SUBJECT_H_
#define SUBJECT_H_

#include <pthread.h>
#include <string>
#include <fstream>
#include <vector>
#include "DownloadNode.h"

class Subject{
public:
	int WORK_NUM;
	int TOTAL_NUM;
	Subject();
	void set_work_num(int new_num);
	void notify(void *ptr);
	DownloadNode get_info(int id);
protected:
	std::vector<pthread_t> fetch_thread;
	std::vector<DownloadNode> shared_node;
	int get_local_file_length(std::string path);
private:
	pthread_mutex_t mutex;
};


#endif /* SUBJECT_H_ */
