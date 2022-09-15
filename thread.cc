#include "thread.h"
#include "cpu.h"

__BEGIN_API

int Thread::switch_context(Thread * prev, Thread * next){
    try{
        CPU::switch_context(prev->_context, next->_context);
        Thread::_running = next;
    }
    catch(int error){
        return -1;
    }
    return 1;
}




__END_API