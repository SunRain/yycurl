#include "Subject.h"

Subject::Subject(){
	WORK_NUM = 3;
	pthread_mutex_init(&share_mutex, NULL);
}

void Subject::set_work_num(int new_num){
	WORK_NUM = new_num;
}

void Subject::notify(void *ptr){
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	pthread_mutex_lock(&share_mutex);

	node->buffer_is_new = true;
	std::string tmp_path = node->path + ".yytmp";
	node->local_file_length = get_local_file_length(tmp_path);

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

int Subject::get_local_file_length(std::string path){
    std::ifstream fin;
    fin.open(path.c_str(), std::ios::app);
    fin.seekg(0, std::ios::end);
    std::streampos ps = fin.tellg();
    return (int)ps;
}
