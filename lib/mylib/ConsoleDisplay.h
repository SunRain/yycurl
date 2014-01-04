#ifndef CONSOLEDISPLAY_H_
#define CONSOLEDISPLAY_H_

#include "Observer.h"
#include <iostream>
#include <stdio.h>
#include <pthread.h>

class ConsoleDisplay : public Observer{
public:
	ConsoleDisplay();
	void display(int id);
	virtual void update();
private:
	static void *yyshow(void *ptr);
};

#endif /* CONSOLEDISPLAY_H_ */
