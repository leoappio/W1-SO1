#include "thread.h"
#include "cpu.h"

__BEGIN_API


//verificar esse try catch
int Thread::switch_context(Thread * prev, Thread * next){
    try{
        CPU::switch_context(prev->_context, next->_context);
        Thread::_running = next;
    }
    catch(int error){
        return -1;
    }
    return 0;
}

// verificar esse thread_exit
void Thread::thread_exit(int exit_code){
    this->_context->~Context();
}

//verificar getter
CPU::Context * Thread::get_context(){
    return this->_context;
}

int Thread::id(){
    return this->_id;
}

__END_API
