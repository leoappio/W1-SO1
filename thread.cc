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

/*
inicializa a main, seta o ponteiro runing para ela, e troca seu estado pra running
inicializa thread dispatcher
troca o contexto para a thread main
*/
void Thread::init(void (*main)(void *)){
    new(&_main)Thread(main, (void*) "Main");
    new(&_dispatcher)Thread(&dispatcher);
    new(&_main_context)CPU::Context();

    _running = &_main;
    _main._state = RUNNING;
    
    CPU::switch_context(&_main_context, _main.get_context());
}

/*
Funciona como o escalonador do sistema, verifica se a fila de threads prontas esta vazia
caso esteja, remove do inicio inicio da fila, seja o dispacher para READY e reinsere a thread na fila
seta o estado da thread escolhida para rodar como RUNNING e troca o contexto entre as duas.

depois verifica se a fila de prontos não está vazia após trocar o
contexto é para não dar seg fault no acesso a propriedade _state caso a fila esteja vazia
e caso esteja vazia remove a ultima thread da fila

*/
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

/*
Remove a cabeça da fila pega a referencia do ponteiro running, para reincerir a thread na fila, verificamos
se a thread não é a main (porque a main não pode ter sua prioridade alterada) e o seu state não é finishing.
então alteramos a prioridade da thread setamos o estado para ready e inserimos na fila. Depois setamos o ponteiro
running para a a thread que foi removida da fila, e o seu estado para running e trocamos o contexto
*/

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

    //caso a tenha uma thread suspensa esperando que a thread que vai terminar termine 
    if(this->_joining) {
        this->_joining->resume();
        this->_joining = 0;
    }

    yield();
}

int Thread::join(){
    db<Thread>(TRC) << "join chamado(this=" << this << ",state=" << _state << ")";

    if(_state != FINISHING && running() != this) {
        //need to suspend thread runnig till thread this ends
        Thread * prev = running();

        //atributo joining para guardar que a thread running esta esperando a thread alvo terminar então
        //guardamos que a alvo está sendo esperada pela running 
        this->_joining = prev;
        prev->suspend();

        yield();
    }
    return 0;
}

void Thread::resume(){
    db<Thread>(TRC) << "resume called (this=" << this << ")";

    //checka se a thread está suspensa e se sim coloca ela na fila de prontos denovo
    if(_state == SUSPENDED) {
        _state = READY;
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        _link.rank(now);
        _ready.insert(&_running->_link); 
        
    } else{
        db<Thread>(WRN) << "Resume called for not suspended thread!";
    }

}

void Thread::suspend(){
    db<Thread>(TRC) << "suspend called (this=" << this << ")";

    Thread * prev = running();

    _state = SUSPENDED;

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
