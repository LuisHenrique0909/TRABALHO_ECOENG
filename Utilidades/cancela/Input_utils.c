#include "Input_utils.h"
#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

int getch_linux() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

int input_com_esc(char *buffer, int tamanho) {
    int idx = 0;

    while (1) {
        int c;

#ifdef _WIN32
        c = getch();
#else
        c = getch_linux();
#endif

        // ESC → cancelar
        if (c == 27) {
            buffer[0] = '\0';
            return 0;
        }

        // ENTER → finalizar entrada
        if (c == '\r' || c == '\n') {
            buffer[idx] = '\0';
            printf("\n");
            return 1;
        }

        // Backspace
        if (c == 8 || c == 127) {
            if (idx > 0) {
                printf("\b \b");
                idx--;
            }
            continue;
        }

        // Armazena caractere normal
        if (idx < tamanho - 1) {
            buffer[idx++] = c;
            putchar(c);
        }
    }
}