#include "thread.h"
#include "cpu.h"

__BEGIN_API

Thread * Thread::_running;
Thread * Thread::_main;
int Thread::id_counter=0;

Thread::~Thread(){
    if (this->_context){
        delete this->_context;
    }
}

//verificar esse try catch
int Thread::switch_context(Thread * prev, Thread * next){
    if(!prev || !next){
        return -1;
    }

    CPU::switch_context(prev->_context, next->_context);
    Thread::_running = next;
    return 1;
}

// verificar esse thread_exit
void Thread::thread_exit(int exit_code){
    switch_context(this,Thread::_main);
}

//verificar getter
CPU::Context * Thread::get_context(){
    return this->_context;
}

int Thread::id(){
    return this->_id;
}

__END_API
