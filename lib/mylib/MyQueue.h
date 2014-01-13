#ifndef MYQUEUE_H_
#define MYQUEUE_H_

#include "DownloadNode.h"
#include <queue>
#include <pthread.h>

struct MyQueue{
	pthread_mutex_t mutex;
	std::queue<DownloadNode*> task;
};


#endif /* MYQUEUE_H_ */
