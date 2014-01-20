#include "Subject.h"
#include "FileOperation.h"
#include <fstream>
#include <string>

Subject::Subject(){
	this->WORK_NUM = 3;
	this->TASK_NUM = 0;
	pthread_mutex_init(&share_mutex, NULL);
}

void Subject::set_work_num(int work_num){
	this->WORK_NUM = work_num;
}

void Subject::set_task_num(int task_num){
	this->TASK_NUM = task_num;
}

void Subject::set_try_num(int try_num){
	this->TRY_NUM = try_num;
}

int Subject::get_work_num(){
	return WORK_NUM;
}

int Subject::get_task_num(){
	return TASK_NUM;
}

int Subject::get_try_num(){
	return TRY_NUM;
}

void Subject::notify(void *ptr){
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	pthread_mutex_lock(&share_mutex);

	node->buffer_is_new = true;
	std::string tmp_path = node->path + ".yytmp";
	FileOperation file(tmp_path);
	node->local_file_length = file.get_file_length();

	pthread_mutex_unlock(&share_mutex);
}

DownloadNode Subject::get_info(int id){
	pthread_mutex_lock(&share_mutex);

	if(shared_node[id].buffer_is_new)
		shared_node[id].buffer_is_new = false;

	pthread_mutex_unlock(&share_mutex);
	//get copy
	return shared_node[id];
}

