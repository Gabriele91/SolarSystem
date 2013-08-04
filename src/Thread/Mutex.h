#ifndef MUTEX_H
#define MUTEX_H

#include "MutexConf.h"

namespace SolarSystem{
    
class Mutex{

public:

	Mutex();
	virtual ~Mutex();
	bool lock(bool block=true);
	void unlock();

private:

	mxrd_t mutex;

};
    
}

#endif