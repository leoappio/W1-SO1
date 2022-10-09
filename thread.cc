#include "thread.h"
#include "cpu.h"
#include "main_class.h"
#include "list.h"

__BEGIN_API

Thread * Thread::_running;
Thread Thread::_main;
Thread Thread::_dispatcher;
CPU::Context Thread::_main_context;
Thread::Ready_Queue Thread::_ready;

int Thread::id_counter=0;

void Thread::init(void (*main)(void *)){
    new(&_main)Thread(main, (void*) "Main");
    new(&_dispatcher)Thread(&dispatcher);
    new(&_main_context)CPU::Context();

    _running = &_main;
    _main._state = RUNNING;
    
    CPU::switch_context(&_main_context, _main.get_context());
}

void Thread::dispatcher(){
    db<Thread>(TRC) << "Dispatcher called";
    while(_ready.size() > 0){
        Thread *next = (_ready.remove_head())->object();
        _dispatcher._state = READY;
        _ready.insert(&_dispatcher._link);
        _running = next;
        next->_state = RUNNING;
        switch_context(&_dispatcher,next);
        if (_ready.size() > 0 && _ready.head()->object()->_state == FINISHING){
            _ready.remove_head();
        }
    }
    db<Thread>(TRC) << "Dispatcher finishing...";
    _dispatcher._state = FINISHING;
    switch_context(&_dispatcher, &_main);
}

void Thread::yield() {
    db<Thread>(TRC) << "Yield called";
    Thread *next = (_ready.remove_head())->object();
    db<Thread>(TRC) << "Next thread ID:"<< next->id();
    Thread *previous = _running;
    if (_running != &_main && _running->_state != FINISHING){
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        _running->_link.rank(now);
        _running->_state = READY;
        _ready.insert(&_running->_link); 
    }
    _running = next;
    next->_state = RUNNING;
    db<Thread>(TRC) << "switch context from:"<< previous->id()<< "to:"<<next->id();
    switch_context(previous, next);
}

int Thread::switch_context(Thread * prev, Thread * next){
    if(!prev || !next){
        return -1;
    }

    Thread::_running = next;
    return CPU::switch_context(prev->_context, next->_context);
    
}

void Thread::thread_exit(int exit_code){
    id_counter--;
    this->_state = FINISHING;
    yield();
}

Thread::~Thread(){
    _ready.remove(&this->_link);
    if(this->_context){
        delete this->_context;
    }
}

CPU::Context * Thread::get_context(){
    return this->_context;
}

int Thread::id(){
    return this->_id;
}

__END_API
