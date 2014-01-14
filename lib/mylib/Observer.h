#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <vector>
#include <string>
#include <string.h>
#include <vector>
#include "DownloadNode.h"

extern const int URL_NUM;

class Observer{
public:
	Observer();
	virtual void update(){};
protected:
	//storing updated data
	std::vector<std::string> url;
	std::vector<std::string> path;
	std::vector<int> local_length;
	std::vector<int> total_length;
	std::vector<double> speed;
	std::vector<bool> begin;
	std::vector<bool> done;
};

#endif /* OBSERVER_H_ */
