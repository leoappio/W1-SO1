#include "cpu.h"
#include <iostream>

__BEGIN_API

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
    if (_stack);
        delete this->_stack;
}

int CPU::switch_context(Context *from, Context *to)
{
    swapcontext(&from->_context, &to->_context);
    return 1;
}

int CPU::finc(volatile int & value)
{
    register int res = 1;
    asm("lock xadd %0, %2" : "=a"(res) : "a"(res), "m"(value));
    return res;
}

int CPU::fdec(volatile int & value)
{
    register int res = -1;
    asm("lock xadd %0, %2" : "=a"(res) : "a"(res), "m"(value));
    return res;
}

__END_API
