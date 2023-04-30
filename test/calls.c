#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL)); // Seed the random number generator with the current time

    int choice = rand() % 32; // Generate a random number between 0 and 31

    switch (choice) {
        case 0:
            printf("Addition\n");
            break;
        case 1:
            printf("Subtraction\n");
            break;
        case 2:
            printf("Multiplication\n");
            break;
        case 3:
            printf("Division\n");
            break;
        case 4:
            printf("Modulus\n");
            break;
        case 5:
            printf("Bitwise AND\n");
            break;
        case 6:
            printf("Bitwise OR\n");
            break;
        case 7:
            printf("Bitwise XOR\n");
            break;
        case 8:
            printf("Left shift\n");
            break;
        case 9:
            printf("Right shift\n");
            break;
        case 10:
            printf("Logical AND\n");
            break;
        case 11:
            printf("Logical OR\n");
            break;
        case 12:
            printf("Less than\n");
            break;
        case 13:
            printf("Greater than\n");
            break;
        case 14:
            printf("Less than or equal to\n");
            break;
        case 15:
            printf("Greater than or equal to\n");
            break;
        case 16:
            printf("Equal to\n");
            break;
        case 17:
            printf("Not equal to\n");
            break;
        case 18:
            printf("Unary plus\n");
            break;
        case 19:
            printf("Unary minus\n");
            break;
        case 20:
            printf("Increment\n");
            break;
        case 21:
            printf("Decrement\n");
            break;
        case 22:
            printf("Assignment\n");
            break;
        case 23:
            printf("Compound addition\n");
            break;
        case 24:
            printf("Compound subtraction\n");
            break;
        case 25:
            printf("Compound multiplication\n");
            break;
        case 26:
            printf("Compound division\n");
            break;
        case 27:
            printf("Conditional operator\n");
            break;
        case 28:
            printf("Comma operator\n");
            break;
        case 29:
            printf("Sizeof operator\n");
            break;
        case 30:
            printf("Cast operator\n");
            break;
        case 31:
            printf("Pointer operator\n");
            break;
        default:
            printf("Invalid choice\n");
            break;
    }

    return 0;
}
