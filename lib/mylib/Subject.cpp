#include "Subject.h"
#include <iostream>
#include <string.h>

Subject::Subject(){
	WORK_NUM = 3;
	memset(buffer_is_new, false, sizeof(buffer_is_new));
	pthread_mutex_init(&mutex, NULL);
}

void Subject::set_work_num(int new_num){
	WORK_NUM = new_num;
}

void Subject::notify(void *ptr){
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	pthread_mutex_lock(&mutex);
	buffer_is_new[node->id] = true;
	std::string tmp_path = node->path + ".yytmp";
	node->local_file_length = get_local_file_length(tmp_path);
	pthread_mutex_unlock(&mutex);
}

DownloadNode Subject::get_info(int id){
	pthread_mutex_lock(&mutex);
	DownloadNode ret = shared_node[id];
	if(buffer_is_new[id])
		buffer_is_new[id] = false;
	pthread_mutex_unlock(&mutex);
	return ret;
}

int Subject::get_local_file_length(std::string path){
    std::ifstream fin;
    fin.open(path.c_str(), std::ios::app);
    fin.seekg(0, std::ios::end);
    std::streampos ps = fin.tellg();
    return (int)ps;
}
