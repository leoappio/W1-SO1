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
    delete this->_stack;
}

void CPU::switch_context(Context *from, Context *to)
{
     from->save();
     to->load();
}

__END_API
