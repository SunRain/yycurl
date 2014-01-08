#ifndef CONSOLEDISPLAY_H_
#define CONSOLEDISPLAY_H_

#include "Observer.h"
<<<<<<< HEAD
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
=======
#include <iostream>
#include <stdio.h>
#include <pthread.h>

class ConsoleDisplay : public Observer{
public:
	ConsoleDisplay();
	void display(int id);
	virtual void update();
private:
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
	static void *yyshow(void *ptr);
};

#endif /* CONSOLEDISPLAY_H_ */
