#include "ConsoleDisplay.h"
<<<<<<< HEAD

ConsoleDisplay::ConsoleDisplay(FetchData *s){
	this->subject = s;
}

void ConsoleDisplay::start(){
	/*console clear*/
	std::string clr = "\033[2J";
	std::cout << clr;

	/*get start time*/
	gettimeofday(&time_start, NULL);

	/*data_init*/
	WORK_NUM = subject->WORK_NUM;

	for(int i = 0; i < WORK_NUM; i++){
		DownloadNode info = subject->get_info(i);
		url[i] = info.url;
		path[i] = info.path;
		total_length[i] = info.download_file_length;
		local_length[i] = info.download_file_length;
		speed[i] = 0;
		display(i);
	}

	/*start display thread*/
	pthread_create(&show_thread, NULL, yyshow, this);
}

void ConsoleDisplay::join(){
	pthread_join(show_thread, NULL);
}

void *ConsoleDisplay::yyshow(void *ptr){
	ConsoleDisplay *console = static_cast<ConsoleDisplay*>(ptr);
	while(1){
		if(console->fetch_over())
			break;
		if(console->subject->buffer_is_new)
			console->update();
		sleep(1);
	}
}

bool ConsoleDisplay::fetch_over(){
	return false;
}

void ConsoleDisplay::update(){
	/*get work number*/
	WORK_NUM = subject->WORK_NUM;

	/*console clear*/
	std::string clr = "\033[2J";
	std::cout << clr;

	/*get the time now*/
	struct timeval time_end;
	gettimeofday(&time_end, NULL);
	double timeuse = 1e6 * (time_end.tv_sec - time_start.tv_sec) + time_end.tv_usec - time_start.tv_usec;
	time_start = time_end;

	/*update info*/
	for(int i = 0; i < WORK_NUM; i++){
		DownloadNode info = subject->get_info(i);
		int download_length = info.local_file_length - local_length[i];
		if(download_length < 0)
			download_length = 0;
		speed[i] = 1e6 * download_length / timeuse;
		local_length[i] = info.local_file_length;
		display(i);
	}
=======
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
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
}


void ConsoleDisplay::display(int id){
	double ans = local_length[id] * 100.0 / total_length[id];
	std::cout << "\033[" << id * 2 + 1 << ";1H";
<<<<<<< HEAD
	std::cout << "-From: [" << (url[id].size() > 50 ? "a long url" : url[id]) << "] To: [" << path[id] << "]\n";
=======
	std::cout << "From: [" << url[id] << "] To: [" << path[id] << "]\n";
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
	if(total_length[id] == -1)
		printf("Downloading: %d / ?? (?? %%) %.2lf KB/s\n", local_length[id], speed[id] / 1000);
	else
		printf("Downloading: %d / %d (%.2lf %%) %.2lf KB/s\n", local_length[id], total_length[id], ans, speed[id] / 1000);
}
