#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <vector>
#include <string>
#include <string.h>
#include "DownloadNode.h"

extern const int URL_NUM;

class Observer{
public:
	Observer();
	virtual void update(){};
protected:
	std::string url[5];
	std::string path[5];
	int local_length[5];
	int total_length[5];
	double speed[5];
};

#endif /* OBSERVER_H_ */
