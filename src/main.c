#include "./lib/main.h"

void main()
{
    uart_configure(&config);
    colorsInit();
    framebf_init(SCR_WIDTH, SCR_HEIGHT);
    displayMenu();
    drawOnScreen();

    while (1)
    {
        uart_puts("\n\n");
        char c = uart_getc();
        uart_sendc(c);
        uart_sendc('\n');

        switch (c)
        {
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                playGame();
                clearScreen(COLORS.BLACK);
                displayMenu();
                drawOnScreen();
                break;
            case '0':
                clearScreen(COLORS.BLACK);
                uart_puts("All cleared!");
                break;
            default:
                uart_puts("Invalid command. Please try again");
                break;
        }
    }
}
