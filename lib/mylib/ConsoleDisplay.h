#ifndef CONSOLEDISPLAY_H_
#define CONSOLEDISPLAY_H_

#include "Observer.h"
#include "FetchData.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

//call ConsoleDisplay(FetchData *s) first, then start() and join() in the end
class ConsoleDisplay : public Observer{
public:
	//accept notify from FetchData s
	ConsoleDisplay(FetchData *s);
	//display download progress of the id'th input
	void display(int id);
	//update download information and call display
	virtual void update();
	//initialize download information and start the thread
	void start();
	//join the thread
	void join();
private:
	//judging if all the task have been done
	bool all_done;
	//number of worker threads, set by Subject::set_work_num(int)
	int WORK_NUM;
	//number of tasks, set by num in FetchData::FetchData(int num, std::vector<std::string> &url, std::vector<std::string> &path)
	int TOTAL_NUM;
	//source of download information, set by ConsoleDisplay(FetchData *s)
	FetchData *subject;
	//show thread identification
	pthread_t show_thread;
	//last time of calling ConsoleDisplay::update(), initialized by ConsoleDisplay::start()
	struct timeval time_start;
	//start showing thread, and updated once per second, stoped by ctrl+z
	static void *yyshow(void *ptr);
	//if pressed ctrl+z, stop the showing thread, and change STOP_ALL to true
	static void ctrl_z(int signal);
};

#endif /* CONSOLEDISPLAY_H_ */
