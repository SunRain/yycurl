#include "ConsoleDisplay.h"

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
}


void ConsoleDisplay::display(int id){
	double ans = local_length[id] * 100.0 / total_length[id];
	std::cout << "\033[" << id * 2 + 1 << ";1H";
	std::cout << "-From: [" << (url[id].size() > 50 ? "a long url" : url[id]) << "] To: [" << path[id] << "]\n";
	if(total_length[id] == -1)
		printf("Downloading: %d / ?? (?? %%) %.2lf KB/s\n", local_length[id], speed[id] / 1000);
	else
		printf("Downloading: %d / %d (%.2lf %%) %.2lf KB/s\n", local_length[id], total_length[id], ans, speed[id] / 1000);
}
