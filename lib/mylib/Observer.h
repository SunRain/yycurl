#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <vector>
#include <string>
<<<<<<< HEAD
#include <string.h>
#include "DownloadNode.h"

extern const int URL_NUM;
=======
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9

class Observer{
public:
	Observer();
	virtual void update(){};
<<<<<<< HEAD
protected:
	std::string url[5];
	std::string path[5];
	int local_length[5];
	int total_length[5];
	double speed[5];
=======
	std::vector<std::string> url;
	std::vector<std::string> path;
	std::vector<int> local_length;
	std::vector<int> total_length;
	std::vector<double> speed;
>>>>>>> 5c5d12f895a7d480c6c48d441f01edb5562b84f9
};

#endif /* OBSERVER_H_ */
