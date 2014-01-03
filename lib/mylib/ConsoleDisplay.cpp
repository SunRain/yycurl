#include "ConsoleDisplay.h"
#include "DownloadNode.h"

extern std::vector<DownloadNode> shared_node;
extern pthread_mutex_t count_lock;
extern pthread_cond_t count_nonzero;
extern unsigned count;

ConsoleDisplay::ConsoleDisplay(){
	/*display data initialize*/
	for(int i = 0; i < shared_node.size(); i++){
		url[i] = shared_node[i].url;
		path[i] = shared_node[i].path;
		local_length[i] = shared_node[i].local_file_length;
		total_length[i] = shared_node[i].download_file_length;
		speed[i] = shared_node[i].download_speed;
		display(i);
	}
	/*start display thread*/
	pthread_t thread;
	pthread_create(&thread, NULL, yyshow, NULL);
}


void *ConsoleDisplay::yyshow(void *ptr){
	while(1){
		ConsoleDisplay *display = new ConsoleDisplay();
		display->update();
	}
}

void ConsoleDisplay::update(){
	pthread_mutex_lock(&count_lock);
	while(count==0){
		pthread_cond_wait(&count_nonzero, &count_lock);
	}
	for(int i = 0; i < shared_node.size(); i++){
		url[i] = shared_node[i].url;
		path[i] = shared_node[i].path;
		local_length[i] = shared_node[i].local_file_length;
		total_length[i] = shared_node[i].download_file_length;
		speed[i] = shared_node[i].download_speed;
		display(i);
	}
	count = count - 1;
	pthread_mutex_unlock(&count_lock);
}


void ConsoleDisplay::display(int id){
	double ans = local_length[id] * 100.0 / total_length[id];
	std::cout << "\033[" << id * 2 + 1 << ";1H";
	std::cout << "From: [" << url[id] << "] To: [" << path[id] << "]\n";
	if(total_length[id] == -1)
		printf("Downloading: %d / ?? (?? %%) %.2lf KB/s\n", local_length[id], speed[id] / 1000);
	else
		printf("Downloading: %d / %d (%.2lf %%) %.2lf KB/s\n", local_length[id], total_length[id], ans, speed[id] / 1000);
}
