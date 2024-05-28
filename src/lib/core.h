
#include "uart.h"
#include "mbox.h"

#define str_size 40
#define ENTER_KEY 10
#define BACKSPACE_KEY 8
#define DELETE_KEY 127
#define SPACE_CHAR 32

const char *readUserInput();
void set_text_color(char text[]);
void set_background_color(char background[]);
void clearTerminal();
void displayMenu();
void wait_ms(unsigned int n);