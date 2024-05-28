#include "./lib/main.h"

void captureAndClear(char c) {
    clearScreen(COLORS.BLACK);
    uart_sendc(c);
    uart_sendc('\n');
}

void main() {
    uart_configure(&config);
    colorsInit();
    framebf_init(SCR_WIDTH, SCR_HEIGHT);
    displayMenu();
    drawOnScreen();

    while (1) {
        uart_puts("\n\n");
        char c = uart_getc();

        switch (c) {
            case '1':
                clearTerminal();
                displayMenu();
                break;
            case '2':
                captureAndClear(c);
                // Display image
                media_pointer = 0; // Ensure media_pointer is set to display the first image
                image_x = 0;
                image_y = 0;
                uart_puts("W for scroll up, S for down\ncurrent image is maximum on top so press S for scroll down\n");
                printImage(image_x, image_y, media_pointer);
                break;
            case '3':
                captureAndClear(c);
                // Display video
                playVideo();
                break;
            case '4':
                captureAndClear(c);
                uart_puts("team member active");
                drawOnScreen();
                break;
            case '5':
                captureAndClear(c);
                showGameMenu();
                uart_puts("Welcome to snake worm\n");
                playGame();
                break;
            case '0':
                captureAndClear(c);
                clearTerminal();
                uart_puts("All cleared!");
                break;
            case 'q':
                captureAndClear(c);
                uart_puts("Do you enjoy this game :)");
            case 's':
            case 'w':
            case 'r':
                change_display(c);
                break;
            default:
                captureAndClear(c);
                uart_puts("Invalid command. Please try again");
                break;
        }
    }
}
