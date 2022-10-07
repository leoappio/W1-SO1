#include "thread.h"
#include "cpu.h"
#include "main_class.h"
#include "list.h"

__BEGIN_API

Thread * Thread::_running;
Thread Thread::_main;
Thread Thread::_dispatcher;
CPU::Context Thread::_main_context;
Ordered_List<Thread> Thread::_ready;

int Thread::id_counter=0;

//meteodo que inicializa dispacher e main
void Thread::init(void (*main)(void *)){
    std::string name = "main";
    //inicializar como placement new porque ja tem memoria alocada, não precisa de mais
    new(&_main)Thread(main, (void*) name.data());
    new(&_dispatcher)Thread(&Thread::dispatcher);
    new(&_main_context)CPU::Context();
    CPU::switch_context(&_main_context, _main.get_context());
}

void Thread::dispatcher(){
    db<Thread>(TRC) << "Dispatcher";
    while(!_ready.empty()){
        Thread *next = (_ready.remove_head())->object();
        _dispatcher._state = READY;
        _ready.insert(&_dispatcher._link);
        _running = next;
        next->_state = RUNNING;
        switch_context(&_dispatcher,next);
        if (next->_state == FINISHING){
            _ready.remove(&next->_link);
        }
    }
    _dispatcher._state = FINISHING;
    _ready.remove(&_dispatcher._link);
    switch_context(&_dispatcher, &_main);
/**    
imprima informação usando o debug em nível TRC
enquanto existir thread do usuário:
escolha uma próxima thread a ser executada
atualiza o status da própria thread dispatacher para READY e reinsire a mesma em _ready
atualiza o ponteiro _running para apontar para a próxima thread a ser executada
atualiza o estado da próxima thread a ser executada
troca o contexto entre as duas threads
testa se o estado da próxima thread é FINISHNING e caso afirmativo a remova de _ready
muda o estado da thread dispatcher para FINISHING
remova a thread dispatcher da fila de prontos
troque o contexto da thread dispatcher para main
*/
}

void Thread::yield() {
    db<Thread>(TRC) << "Yield";
    Thread *next = (_ready.remove_head())->object();
    Thread *tmp = _running;
    if (_running != &_main){
        db<Thread>(TRC) << "running != main";
    }
    if (_running->_state != FINISHING){
        db<Thread>(TRC) << "running state == FINISHING";
    }
    if (_running != &_main && _running->_state != FINISHING){
        db<Thread>(TRC) << "entered if";
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        db<Thread>(TRC) << "got new priority";
        _running->_link.rank(now);
        db<Thread>(TRC) << "seted rank";
        _ready.insert(&_running->_link);
        db<Thread>(TRC) << "inserted runing";    
    }
    _running = next;
    next->_state = RUNNING;
    switch_context(tmp,next);
/**imprima informação usando o debug em nível TRC
escolha uma próxima thread a ser executada
atualiza a prioridade da tarefa que estava sendo executada (aquela que chamou yield) com o
timestamp atual, a fim de reinserí-la na fila de prontos atualizada (cuide de casos especiais, como
estado ser FINISHING ou Thread main que não devem ter suas prioridades alteradas)
reinsira a thread que estava executando na fila de prontos
atualiza o ponteiro _running
atualiza o estado da próxima thread a ser executada
troque o contexto entre as threads*/


}

int Thread::switch_context(Thread * prev, Thread * next){
    if(!prev || !next){
        return -1;
    }

    Thread::_running = next;
    return CPU::switch_context(prev->_context, next->_context);
    
}

void Thread::thread_exit(int exit_code){
    if (this->_context){
        id_counter--;
        delete this->_context;
    }
}

Thread::~Thread(){
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
