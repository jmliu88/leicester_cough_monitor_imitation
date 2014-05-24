/*
 * ThreadBase.h
 *
 *  Created on: Dec 22, 2013
 *      Author: zwang
 */

#ifndef THREADBASE_H_
#define THREADBASE_H_
extern "C"
{
#include <pthread.h>
#include <unistd.h>
}
class Thread_Base {
public:
	Thread_Base();
	pthread_t start()
	{
		pthread_t tid;
		pthread_create(&tid,NULL,hook,this);
		return tid;
	}
	virtual ~Thread_Base();
private:
	static void* hook(void* args){
		reinterpret_cast<Thread_Base*> (args)->run();
		return NULL;
	}
protected:
	virtual void run()=0;
};

#endif /* THREADBASE_H_ */
