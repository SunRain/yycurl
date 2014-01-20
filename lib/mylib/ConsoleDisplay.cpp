#include "ConsoleDisplay.h"
#include <signal.h>
#include <iostream>
#include <stdio.h>

extern bool STOP_ALL;

ConsoleDisplay::ConsoleDisplay(FetchData *s){
	this->subject = s;
}

void ConsoleDisplay::start(){
	//console clear
	std::string clr = "\033[2J";
	std::cout << clr;

	//initialize start time
	gettimeofday(&time_start, NULL);

	//data_init
	TASK_NUM = subject->get_task_num();
	WORK_NUM = subject->get_work_num();
	url.clear();
	path.clear();
	total_length.clear();
	local_length.clear();
	speed.clear();
	begin.clear();
	check.clear();
	done.clear();

	for(int i = 0; i < TASK_NUM; i++){
		DownloadNode info = subject->get_info(i);
		url.push_back(info.url);
		path.push_back(info.path);
		total_length.push_back(info.download_file_length);
		local_length.push_back(info.download_file_length);
		speed.push_back(0);
		begin.push_back(info.begin);
		check.push_back(info.check);
		done.push_back(info.done);
		display(i);
	}

	//start display thread
	pthread_create(&show_thread, NULL, yyshow, this);
}

void ConsoleDisplay::join(){
	pthread_join(show_thread, NULL);
}

void ConsoleDisplay::ctrl_z(int signal){
	puts("");
	STOP_ALL = true;
}

void *ConsoleDisplay::yyshow(void *ptr){
	ConsoleDisplay *console = static_cast<ConsoleDisplay*>(ptr);
	signal(SIGINT, ctrl_z);
	while(1){
		if(STOP_ALL) break;
		console->update();
		usleep(5e5);
	}
}


void ConsoleDisplay::update(){
	//console clear
	std::string clr = "\033[2J";
	std::cout << clr;

	//get the present time, and difference value between last time and this time
	struct timeval time_end;
	gettimeofday(&time_end, NULL);
	double timeuse = 1e6 * (time_end.tv_sec - time_start.tv_sec) + time_end.tv_usec - time_start.tv_usec;
	time_start = time_end;

	done_num = 0;
	check_num = 0;
	//update information
	for(int i = 0; i < TASK_NUM; i++){
		DownloadNode info = subject->get_info(i);
		//get size of new data
		int download_length = info.local_file_length - local_length[i];
		//calculate downloading speed
		speed[i] = 1e6 * download_length / timeuse;
		total_length[i] = info.download_file_length;
		local_length[i] = info.local_file_length;
		begin[i] = info.begin;
		check[i] = info.check;
		done[i] = info.done;
		if(check[i])
			check_num++;
		if(done[i])
			done_num++;
		display(i);
	}
	if(done_num == TASK_NUM){
		//console clear
		std::string clr = "\033[2J";
		std::cout << clr;
		std::cout << "\033[1;1H";
		std::cout << "all " << done_num << " files done!" << std::endl;
		sleep(1);
		pthread_exit(NULL);
	}
	else if(check_num == TASK_NUM){
		//console clear
		std::string clr = "\033[2J";
		std::cout << clr;
		std::cout << "\033[1;1H";
		std::cout << "all " << check_num << " files checked! ";
		std::cout << done_num << " / " << TASK_NUM << " files done!" << std::endl;
		sleep(1);
		pthread_exit(NULL);
	}
	else{
		std::cout << std::endl;
		std::cout << done_num << " / " << TASK_NUM << " files done!" << std::endl;
	}
}

void ConsoleDisplay::display(int id){
	//display at top of the console
	if(id == 0){
		std::cout << "\033[1;1H";
	}
	//if downloading
	if(begin[id] && !done[id]){
		double ans = local_length[id] * 100.0 / total_length[id];
		//display URL and local path
		std::cout << "- From: [" << url[id] << "]\n  To: [" << path[id] << "]\n";
		if(!check[id])
			std::cout << "  Downloading: ";
		else
			std::cout << "  Checking:    ";
		//if can't get total length of the url, print ??
		if(total_length[id] == -1)
			printf("%d / ?? (?? %%) %.2lf KB/s\n\n", local_length[id], speed[id] / 1000);
		else
			printf("%d / %d (%.2lf %%) %.2lf KB/s\n\n", local_length[id], total_length[id], ans, speed[id] / 1000);
	}
}
