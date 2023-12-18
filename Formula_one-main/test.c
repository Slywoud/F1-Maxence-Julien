#include <stdio.h>
#include <time.h>
#include <ncurses.h>

#define ANSI_COLOR_RED     "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main (){
    printf("hello" ANSI_COLOR_RED " world" ANSI_COLOR_RESET "\n");

    return 0;
}