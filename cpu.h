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
                this->_context.uc_link=0;
                this->_context.uc_stack.ss_flags=0;
                this->_context.uc_stack.ss_sp = (void *)_stack;
                this->_context.uc_stack.ss_size = STACK_SIZE;
            }

            Context() { _stack=0;};
            //inicializar a _stack para zero no construtor sem parametros


            template<typename ... Tn>
            Context(void (* func)(Tn ...), Tn ... an){
                save();
                this->_stack = new char[STACK_SIZE];
                if (_stack){
                    setContextStackData();
                    makecontext(&this->_context,(void(*)())(func),sizeof...(Tn), an ... );
                } else {
                    std::cout<<"Problemas a alocar a pilha";
                    exit(-1);
                }
                    
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

        static int switch_context(Context *from, Context *to);
        static int finc(volatile int& value);
        static int fdec(volatile int& value);

};

__END_API

#endif

