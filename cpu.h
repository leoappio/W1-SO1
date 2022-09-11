#ifndef cpu_h
#define cpu_h

#include <ucontext.h>
#include <iostream>
#include "traits.h"

__BEGIN_API

class CPU
{
    public:

        class Context
        {
        private:
            static const unsigned int STACK_SIZE = Traits<CPU>::STACK_SIZE;
        public:

            void setContextStackData(){
                this->_stack = new char[STACK_SIZE];
                getcontext(&this->_context);
                this->_context.uc_link=0;
                this->_context.uc_stack.ss_flags=0;
                this->_context.uc_stack.ss_sp = (void *)_stack;
                this->_context.uc_stack.ss_size = STACK_SIZE;
            }

            Context() {};

            template<typename ... Tn>
            Context(void (* func)(Tn ...), Tn ... an){
                setContextStackData();
                makecontext(&this->_context,(void(*)())(func),sizeof...(Tn), an ... );
            };

            ~Context();

            void save();
            void load();

        private:            
            char *_stack;
        public:
            ucontext_t _context;
        };

    public:

        static void switch_context(Context *from, Context *to);

};

__END_API

#endif

