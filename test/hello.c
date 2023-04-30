//=============================================================================
// FILE:
//      hello.c
//
// DESCRIPTION:
//      Sample input file for CallCounter analysis.
//
// License: MIT
//=============================================================================
#include <sys/syscall.h>
#include <unistd.h>

__attribute__((noinline)) int foo(int a) {
    if (a % 2 == 0)
        return a - 1;
    else
        return a / 2;
}

int bar(int a) {
    int b = a * 3;
    return foo(b); 
}

void fez() {bar(2); }

void _start(int argc, char** argv) {
    volatile int a = foo(argc);
    unsigned long long b = bar(argc);
    
    fez();

    __asm__(
        "movq $60, %%rax\n\t"
        //"movq %0, %%rdi\n\t"
        "syscall\n\t"
        :
        ://"r"(b)
        :"%rax","%rdi");

    //syscall(60);

    //return b;
}
