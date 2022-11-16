#ifndef semaphore_h
#define semaphore_h

#include "traits.h"
#include "debug.h"
#include "list.h"
#include "cpu.h"
#include "semaphore.h"

__BEGIN_API

void Semaphore::p()
{
db<Semaphore>(TRC) << "Semaphore p";

if(fdec(value) < 1)
    sleep();
}

void Semaphore::v()
{
if(finc(value)< 0)
    wakeup();
}

int Semaphore::finc(volatile int & value)
{
    CPU::finc(value);
}
int Semaphore::fdec(volatile int & value)
{
    CPU::fdec(value);
}
void Semaphore::sleep()
{   
    //coloca na fila de dormindo e chama o theadsleep da Thread
    _sleeping.push(Thread::running())
    Thread::sleep();
}
void Semaphore::wakeup()
{   
    //tira da fila de dormindo e chama o threadwakeup da Thread
    Thread * head =_sleeping.front()
    _sleeping.pop()
    head->wakeup()

}
void Semaphore::wakeup_all()
{
    //so vai ser chamado quando o semafaro for destruido
    Thread::wakeup_all()
}
Semaphore::~Semaphore(){
    wakeup_all()
};


__END_API

#endif