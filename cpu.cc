#include "cpu.h"
#include <iostream>

__BEGIN_API

//nao sei se isso tá certo, provavelmente nao
//CPU::Context::Context(void (* func)(Tn ...), Tn ... an)
//{
    //aqui tem que passar os parametros pro makecontext, mas não sei quais e nem como passar
//    makecontext();
//}

void CPU::Context::save()
{
    getcontext(&this->_context);
}

void CPU::Context::load()
{
    setcontext(&this->_context);
}

CPU::Context::~Context()
{
    //mudar o fre
    delete this->_stack;
}

void CPU::switch_context(Context *from, Context *to)
{
     from->save();
     to->load();
}

__END_API
