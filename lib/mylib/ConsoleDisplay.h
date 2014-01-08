#ifndef CONSOLEDISPLAY_H_
#define CONSOLEDISPLAY_H_

#include "Observer.h"
#include "FetchData.h"
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

class ConsoleDisplay : public Observer{
public:
	ConsoleDisplay(FetchData *s);
	void display(int id);
	virtual void update();
	void start();
	void join();
	bool fetch_over();
private:
	int WORK_NUM;
	FetchData *subject;
	pthread_t show_thread;
	struct timeval time_start;
	static void *yyshow(void *ptr);
};

#endif /* CONSOLEDISPLAY_H_ */
