#include "semaphore.h"
#include "traits.h"
#include "debug.h"
#include "cpu.h"


__BEGIN_API

void Semaphore::p()
{
db<Semaphore>(TRC) << "Semaphore p";
if(fdec(value) < 1)
    sleep();
}

void Semaphore:: v()
{
db<Semaphore>(TRC) << "Semaphore v";
if(finc(value) < 0)
    wakeup();
}

int Semaphore::finc(volatile int & value)
{
    return CPU::finc(value);
}

int Semaphore::fdec(volatile int & value)
{
    return CPU::fdec(value);
}

void Semaphore::sleep()
{   
    //coloca na fila de dormindo e chama o theadsleep da Thread
    Thread * tds = Thread::running();
    _sleeping.push(tds);
    db<Thread>(TRC) << "Thread que deve tar na fila:"<< tds->id();
    tds->sleep();
}
void Semaphore::wakeup()
{   
    //tira da fila de dormindo e chama o threadwakeup da Thread
    Thread * head =_sleeping.front();
    _sleeping.pop();
    head->wakeup();
}
void Semaphore::wakeup_all()
{
    //so vai ser chamado quando o semafaro for destruido
    while (!_sleeping.empty()){
        Semaphore::wakeup();
    }
}

Semaphore::~Semaphore(){
    wakeup_all();
}

__END_API
