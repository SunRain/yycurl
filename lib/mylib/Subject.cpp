#include "Subject.h"
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
    std::ifstream fin;
    fin.open(path.c_str(), std::ios::app);
    fin.seekg(0, std::ios::end);
    std::streampos ps = fin.tellg();
    return (int)ps;
}
