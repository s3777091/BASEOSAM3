#include "core.h"


void displayMenu()
{
    set_text_color("blue");
    uart_puts(
        "\n\n\tEnter a number to choose command:\n"
        "\t4.\tDisplay a video\n"
        "\t5.\tPlay game\n"
        "\t0.\tClear the screen\n");
    set_text_color("green");
}

// -------------------------------------------------------------- //
void clearTerminal()
{
    uart_puts("\033[H\033[J");
}

// -------------------------------------------------------------- //
void wait_ms(unsigned int n)
{
    register unsigned long f, t, r;

    // Get the current counter frequency
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f));
    // Read the current counter
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t));
    // Calculate expire value for counter
    t += ((f / 1000) * n) / 1000;
    do
    {
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r));
    } while (r < t);
}


const char *readUserInput() {
    static char command[str_size];
    int count = 0;
    char c;

    while (1) {
        c = uart_getc();

        if (c == ENTER_KEY) {
            command[count] = '\0';
            uart_puts("\n");
            return command;
        }
        else if (c == BACKSPACE_KEY || c == DELETE_KEY) {
            if (count > 0) {
                count--;
                command[count] = '\0';

                // Properly erase the character on terminal
                uart_puts("\b \b");
            }
        }
        else if (c >= SPACE_CHAR && count < str_size - 1) {
            command[count] = c;
            uart_sendc(c);
            count++;
        }
    }
}

void set_text_color(char text[])
{
    if (string_compare(text, "black") == 0)
    {
        uart_puts("\033[30;1m");
    }
    else if (string_compare(text, "red") == 0)
    {
        uart_puts("\033[31;1m");
    }
    else if (string_compare(text, "green") == 0)
    {
        uart_puts("\033[32;1m");
    }
    else if (string_compare(text, "yellow") == 0)
    {
        uart_puts("\033[33;1m");
    }
    else if (string_compare(text, "blue") == 0)
    {
        uart_puts("\033[34;1m");
    }
    else if (string_compare(text, "purple") == 0)
    {
        uart_puts("\033[35;1m");
    }
    else if (string_compare(text, "cyan") == 0)
    {
        uart_puts("\033[36;1m");
    }
    else if (string_compare(text, "white") == 0)
    {
        uart_puts("\033[37;1m");
    }
}

void set_background_color(char background[])
{
    if (string_compare(background, "black") == 0)
    {
        uart_puts("\033[40;1m");
    }
    else if (string_compare(background, "red") == 0)
    {
        uart_puts("\033[41;1m");
    }
    else if (string_compare(background, "green") == 0)
    {
        uart_puts("\033[42;1m");
    }
    else if (string_compare(background, "yellow") == 0)
    {
        uart_puts("\033[43;1m");
    }
    else if (string_compare(background, "blue") == 0)
    {
        uart_puts("\033[44;1m");
    }
    else if (string_compare(background, "purple") == 0)
    {
        uart_puts("\033[45;1m");
    }
    else if (string_compare(background, "cyan") == 0)
    {
        uart_puts("\033[46;1m");
    }
    else if (string_compare(background, "white") == 0)
    {
        uart_puts("\033[47;1m");
    }
}