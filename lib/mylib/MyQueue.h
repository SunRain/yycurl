#ifndef MYQUEUE_H_
#define MYQUEUE_H_

#include "DownloadNode.h"
#include <queue>
#include <pthread.h>

struct MyQueue{
	//mutex lock of task queue
	pthread_mutex_t mutex;
	//task queue, used to stored tasks which haven't been started
	std::deque<DownloadNode*> task;
};


#endif /* MYQUEUE_H_ */
