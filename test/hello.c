//=============================================================================
// FILE:
//      hello.c
//
// DESCRIPTION:
//      Sample input file for CallCounter analysis.
//
// License: MIT
//=============================================================================
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char** argv) {
    volatile int a = foo(argc);
    int b = bar(argc);
    
    // fez();
    // int ii = 0;
    // for (ii = 0; ii < 10; ii++)
    //     foo(ii);


    // int input = rand() % 64 + 1; // generate random number between 1 and 64
    // int value = rand() % 10 + 1; // generate random value between 1 and 10

    // switch(input) {
    //     case 1:
    //         printf("Add %d to value\n", value);
    //         value += value;
    //         break;
    //     case 2:
    //         printf("Subtract %d from value\n", value);
    //         value -= value;
    //         break;
    //     case 3:
    //         printf("Multiply value by %d\n", value);
    //         value *= value;
    //         break;
    //     case 4:
    //         printf("Divide value by %d\n", value);
    //         value /= value;
    //         break;
    //     // Repeat for cases 5 through 64
    //     case 5:
    //         printf("Add %d to value\n", value);
    //         value += value;
    //         break;
    //     // ...
    //     case 64:
    //         printf("Divide value by %d\n", value);
    //         value /= value;
    //         break;
    //     default:
    //         printf("Invalid input\n");
    //         break;
    // }

    return b;
}
