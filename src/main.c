#include "uart.h"
#include "core.h"

uart_config config = {
    .data_bits = 8,
    .stop_bits = 1,
    .cts_rts = 0,
    .parity = 0,
    .baudrate = 115200
};

void main()
{
    uart_configure(&config);

    char buffer[100];
    int index = 0;
    while (1)
    {
        char c = uart_getc();
        uart_sendc(c);
    }
}