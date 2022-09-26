#include "thread.h"
#include "cpu.h"
#include "main_class.h"

__BEGIN_API

Thread * Thread::_running;
Thread Thread::_main;
Thread Thread::_dispatcher;

int Thread::id_counter=0;

//meteodo que inicializa dispacher e main
void Thread::init(void (*main)(void *)){
    std::string name = "main";
    Thread::_main = *new Thread(main, (void*) name.data());
    Thread::_dispatcher = *new Thread(&Thread::dispatcher);
    Thread::_main_context = *Thread::_main.get_context();

    //verificar se é assim
    Thread::_main_context.load();

    //ou se é assim
    Thread::switch_context(&_main,&_main);

}


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
