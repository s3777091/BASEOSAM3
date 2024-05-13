#include "uart.h"
#include "core.h"
#include "mbox.h"
#include "framebf.h"
#include "game.h"

uart_config config = {
    .data_bits = 8,
    .stop_bits = 1,
    .cts_rts = 0,
    .parity = 0,
    .baudrate = 115200
};


struct Colors
{
    unsigned int RED, GREEN, BLUE, YELLOW, CYAN, PINK, BLACK, WHITE;
} COLORS;

void colorsInit()
{
    COLORS.RED = 0x00AA0000;
    COLORS.GREEN = 0x0000BB00;
    COLORS.BLUE = 0x000000CC;
    COLORS.YELLOW = 0x00FFFF00;
    COLORS.CYAN = 0x00A2DDFA;
    COLORS.PINK = 0x00FA4380;
    COLORS.BLACK = 0;
    COLORS.WHITE = 0x00FFFFFF;
}

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
                playVideo();
                break;
            case '5':
                playGame();
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
