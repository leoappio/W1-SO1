#include "thread.h"
#include "cpu.h"

__BEGIN_API

Thread * Thread::_running;
Thread * Thread::_main;
int Thread::id_counter=0;


//verificar esse try catch
int Thread::switch_context(Thread * prev, Thread * next){
    if(!prev || !next){
        return -1;
    }

    Thread::_running = next;
    return CPU::switch_context(prev->_context, next->_context);
    
}

// verificar esse thread_exit
void Thread::thread_exit(int exit_code){
    if (this->_context){
        id_counter--;
        delete this->_context;
    }
}

//verificar getter
CPU::Context * Thread::get_context(){
    return this->_context;
}

int Thread::id(){
    return this->_id;
}

__END_API
