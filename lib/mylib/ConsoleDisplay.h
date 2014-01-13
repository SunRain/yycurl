#ifndef CONSOLEDISPLAY_H_
#define CONSOLEDISPLAY_H_

#include "Observer.h"
#include "FetchData.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

class ConsoleDisplay : public Observer{
public:
	ConsoleDisplay(FetchData *s);
	void display(int id);
	virtual void update();
	void start();
	void join();
private:
	bool all_done;
	int WORK_NUM;
	int TOTAL_NUM;
	FetchData *subject;
	pthread_t show_thread;
	struct timeval time_start;
	static void *yyshow(void *ptr);
	static void ctrl_z(int signal);
};

#endif /* CONSOLEDISPLAY_H_ */
