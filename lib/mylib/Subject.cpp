#include "Subject.h"
<<<<<<< HEAD
#include <iostream>
#include <string.h>

Subject::Subject(){
	WORK_NUM = 3;
	memset(buffer_is_new, false, sizeof(buffer_is_new));
	pthread_mutex_init(&mutex, NULL);
}

void Subject::notify(void *ptr){
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	pthread_mutex_lock(&mutex);
	buffer_is_new[node->id] = true;
	node->local_file_length = get_local_file_length(node->path);
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

int Subject::get_local_file_length(std::string &path){
=======
#include "DownloadNode.h"

extern std::vector<DownloadNode> shared_node;
extern pthread_mutex_t count_lock;
extern pthread_cond_t count_nonzero;
extern unsigned count;
extern struct timeval time_start;

void Subject::notify(){
	pthread_mutex_lock(&count_lock);
	if(count == 0){
		pthread_cond_signal(&count_nonzero);
	}
	struct timeval time_end;
	gettimeofday(&time_end, NULL);
	double timeuse = 1e6 * (time_end.tv_sec - time_start.tv_sec) + time_end.tv_usec - time_start.tv_usec;
	time_start = time_end;
	for(int i = 0; i < shared_node.size(); i++){
		shared_node[i].local_file_length = get_local_file_length(shared_node[i].path);
		shared_node[i].download_speed = 1e6 * (shared_node[i].local_file_length - shared_node[i].old_local_file_length);
		shared_node[i].old_local_file_length = shared_node[i].local_file_length;
	}
	count = count + 1;
	pthread_mutex_unlock(&count_lock);
}


int Subject::get_local_file_length(std::string path){
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
    std::ifstream fin;
    fin.open(path.c_str(), std::ios::app);
    fin.seekg(0, std::ios::end);
    std::streampos ps = fin.tellg();
    return (int)ps;
}
