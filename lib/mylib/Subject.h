#ifndef SUBJECT_H_
#define SUBJECT_H_

#include "DownloadNode.h"
#include <pthread.h>
#include <string>
#include <fstream>
#include <vector>

class Subject{
public:
	//set default WORK_NUM(3), and initialize shared_mutex
	Subject();
	//set number of worker threads
	void set_work_num(int work_num);
	//set total number of tasks
	void set_task_num(int task_num);
	//get number of worker threads
	int get_work_num();
	//get total number of tasks
	int get_task_num();
	//notify there is new data
	void notify(void *ptr);
	//get the copy of id'th shared_node
	DownloadNode get_info(int id);
protected:
	//number of worker threads, set by Subject::set_work_num(int)
	int WORK_NUM;
	//number of tasks, set by Subject::set_task_num(int)
	int TASK_NUM;
	//index of worker threads
	std::vector<pthread_t> worker_thread;
	//shared memory stored downloading information
	std::vector<DownloadNode> shared_node;
	//get local file length of path
	int get_local_file_length(std::string path);
private:
	//mutex lock of shared_node
	pthread_mutex_t share_mutex;
};


#endif /* SUBJECT_H_ */
