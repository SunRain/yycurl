#ifndef SUBJECT_H_
#define SUBJECT_H_

#include <pthread.h>
#include <string>
#include <fstream>
#include "DownloadNode.h"

class Subject{
public:
	int WORK_NUM;
	bool buffer_is_new[5];
	Subject();
	void set_work_num(int new_num);
	void notify(void *ptr);
	DownloadNode get_info(int id);
protected:
	pthread_t fetch_thread[5];
	DownloadNode shared_node[5];
	int get_local_file_length(std::string &path);
private:
	pthread_mutex_t mutex;
};


#endif /* SUBJECT_H_ */
