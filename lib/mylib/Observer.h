#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <vector>
#include <string>

class Observer{
public:
	Observer();
	virtual void update(){};
	std::vector<std::string> url;
	std::vector<std::string> path;
	std::vector<int> local_length;
	std::vector<int> total_length;
	std::vector<double> speed;
};

#endif /* OBSERVER_H_ */
