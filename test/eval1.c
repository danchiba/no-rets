#include <sys/syscall.h>
#include <unistd.h>

__attribute__((noinline)) long long foo(int a) {
    return 5;
}

__attribute__((noinline)) void exit_func(long long retval) {
    __asm__(
        "movq $60, %rax\n\t"
        "syscall"
    );
}

void _start(int argc, char** argv) {
    for (int i = 0; i < 1000000; i++) {
        foo(10);
    }

    exit_func(13);
}

int bar(int a) {
    int b = a * 3;
    foo(10);
    foo(10);
    foo(10);
    foo(10);
    foo(10);
    foo(10);
    foo(10);
    foo(10);
    foo(10);
    foo(10);
    return foo(b); 
}

