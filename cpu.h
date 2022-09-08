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
            // duplicar o construtor para suportar instanciação com e sem paramentros
            Context() { this->_stack = new char[STACK_SIZE](); }

            template<typename ... Tn>
            Context(void (* func)(Tn ...), Tn ... an){
                getcontext(&this->_context);
                this->_context.uc_flags=0;
                this->_context.uc_link=nullptr;
                this->_context.uc_stack.ss_flags=0;
                this->_context.uc_stack.ss_sp = (void *)_stack;
                this->_context.uc_stack.ss_size = STACK_SIZE;
                //ver como que passa esses argumentos pra essa porra
                makecontext(&this->_context,(* func)(Tn ...), Tn ... an );
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

