#ifndef semaphore_h
#define semaphore_h

#include "traits.h"
#include "cpu.h"
#include "thread.h"
#include "debug.h"
#include <queue>

__BEGIN_API

class Semaphore
{
public:
    typedef std::queue<Thread*> Sleep_queue;
    Semaphore(int v = 1) : value(v){}
    ~Semaphore();

    void p();
    void v();
private:
    // Atomic operations
    int finc(volatile int & value);
    int fdec(volatile int & value);

    // Thread operations
    void sleep();
    void wakeup();
    void wakeup_all();

private:
    volatile int value;
    Sleep_queue _sleeping;
};

__END_API

#endif