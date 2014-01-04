#ifndef SUBJECT_H_
#define SUBJECT_H_

#include <pthread.h>
#include <string>
#include <vector>
#include <fstream>
#include <sys/time.h>

class Subject{
public:
	void notify();
	int get_local_file_length(std::string path);
};


#endif /* SUBJECT_H_ */
