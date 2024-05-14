#include "./lib/uart.h"

uart_type uart_configure(uart_config *config)
{
    unsigned int r;

    if (config->data_bits < 5u || config->data_bits > 8u)
    {
        return UART_INVALID_ARGUMENT_WORDSIZE;
    }

    if (config->stop_bits == 0u || config->stop_bits > 2u)
    {
        return UART_INVALID_ARGUMENT_STOP_BITS;
    }
    if (config->baudrate < 110u || config->baudrate > 460800u)
    {
        return UART_INVALID_ARGUMENT_BAUDRATE;
    }

    /* Turn off UART0 */
    UART0_CR = 0x0;

    /* Setup GPIO pins 14 and 15 */

    /* Set GPIO14 and GPIO15 to be pl011 TX/RX which is ALT0	*/
    r = GPFSEL1;
    r &= ~((7 << 12) | (7 << 15));      // clear bits 17-12 (FSEL15, FSEL14)
    r |= (0b100 << 12) | (0b100 << 15); // Set value 0b100 (select ALT0: TXD0/RXD0)
    GPFSEL1 = r;

    /* enable GPIO 14, 15 */
#ifdef RPI3    // RPI3
    GPPUD = 0; // No pull up/down control
    // Toogle clock to flush GPIO setup
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    } // waiting 150 cycles
    GPPUDCLK0 = (1 << 14) | (1 << 15); // enable clock for GPIO 14, 15
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    } // waiting 150 cycles
    GPPUDCLK0 = 0; // flush GPIO setup

#else // RPI4
    r = GPIO_PUP_PDN_CNTRL_REG0;
    r &= ~((3 << 28) | (3 << 30)); // No resistor is selected for GPIO 14, 15
    GPIO_PUP_PDN_CNTRL_REG0 = r;
#endif

    /* Mask all interrupts. */
    UART0_IMSC = 0;

    /* Clear pending interrupts. */
    UART0_ICR = 0x7FF;

    float baudrate_divisor = (float)48000000 / (16 * config->baudrate);
    int integer_part = (int)baudrate_divisor;
    int fraction_part = (int)((float)(baudrate_divisor - integer_part) * 64 + 0.5);

    UART0_IBRD = integer_part;
    UART0_FBRD = fraction_part;

    switch (config->data_bits)
    {
    case 5:
        UART0_LCRH |= UART0_LCRH_WLEN_5BIT;
        break;
    case 6:
        UART0_LCRH |= UART0_LCRH_WLEN_6BIT;
        break;
    case 7:
        UART0_LCRH |= UART0_LCRH_WLEN_7BIT;
        break;
    case 8:
        UART0_LCRH |= UART0_LCRH_WLEN_8BIT;
        break;
    }

    // Configure parity according to config->parity
    if (config->parity == 0u) // Parity none
    {
        UART0_LCRH &= ~UART0_LCRH_PEN; // Clear parity enable bit
    }
    else if (config->parity == 1u) // Parity odd
    {
        UART0_LCRH |= UART0_LCRH_PEN;  // Set parity enable bit
        UART0_LCRH &= ~UART0_LCRH_EPS; // Clear parity select bit for odd parity
    }
    else if (config->parity == 2u) // Parity even
    {
        UART0_LCRH |= UART0_LCRH_PEN; // Set parity enable bit
        UART0_LCRH |= UART0_LCRH_EPS; // Set parity select bit for even parity
    }

    if (config->stop_bits == 1u)
    {
        UART0_LCRH &= ~UART0_LCRH_STP2;
    }
    else if (config->stop_bits == 2u)
    {
        UART0_LCRH |= UART0_LCRH_STP2;
    }

    if (config->cts_rts == 1u)
    {
        UART0_CR |= (UART0_CR_CTSEN | UART0_CR_RTSEN); // Enable CTS and RTS
    }
    else
    {
        UART0_CR &= ~(UART0_CR_CTSEN | UART0_CR_RTSEN); // Disable CTS and RTS
    }

    /* Enable UART0, receive, and transmit */
    UART0_CR = 0x301; // enable Tx, Rx, FIFO

    return UART_OK;
}

/**
 * Send a character
 */
void uart_sendc(char c)
{
    /* Check Flags Register */
    /* And wait until transmitter is not full */
    do
    {
        asm volatile("nop");
    } while (UART0_FR & UART0_FR_TXFF);

    /* Write our data byte out to the data register */
    UART0_DR = c;
}

/**
 * Receive a character
 */
char uart_getc()
{
    char c = 0;

    /* Check Flags Register */
    /* Wait until Receiver is not empty
     * (at least one byte data in receive fifo)*/
    do
    {
        asm volatile("nop");
    } while (UART0_FR & UART0_FR_RXFE);

    /* read it and return */
    c = (unsigned char)(UART0_DR);

    /* convert carriage return to newline */
    return (c == '\r' ? '\n' : c);
}

void uart_puts(const char *s)
{
    while (*s)
    {
        // convert newline to carriage return + newline
        if (*s == '\n')
            uart_sendc('\r');
        uart_sendc(*s++);
    }
}

void uart_display_os()
{
    uart_puts("DatOs> ");
}

void uart_clear_screen()
{
    uart_puts("\033[2J\033[1;1H");
}

void uart_puts_clear_line(char *s)
{
    uart_puts("\r\033[K");

    uart_display_os(); // Display the command prompt or other starter text

    while (*s)
    {
        if (*s == '\n')
        {
            uart_puts("\r\n"); // Send both carriage return and newline together
        }
        else
        {
            uart_sendc(*s);
        }
        s++;
    }
}

/**
 * Display a value in hexadecimal format
 */
void uart_hex(unsigned int num)
{
    uart_puts("0x");
    for (int pos = 28; pos >= 0; pos = pos - 4)
    {

        // Get highest 4-bit nibble
        char digit = (num >> pos) & 0xF;

        /* Convert to ASCII code */
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        digit += (digit > 9) ? (-10 + 'A') : '0';
        uart_sendc(digit);
    }
}

/*
**
* Display a value in decimal format
*/
void uart_dec(int num)
{
    // A string to store the digit characters
    char str[33] = "";

    // Calculate the number of digits
    int len = 1;
    int temp = num;
    while (temp >= 10)
    {
        len++;
        temp = temp / 10;
    }

    // Store into the string and print out
    for (int i = 0; i < len; i++)
    {
        int digit = num % 10; // get last digit
        num = num / 10;       // remove last digit from the number
        str[len - (i + 1)] = digit + '0';
    }
    str[len] = '\0';

    uart_puts(str);
}

/**
 * Comparing 2 strings.
 */
int string_compare(char *a, char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
        {
            return 1; // strings are not equal
        }
        a++;
        b++;
    }
    return *a || *b ? 1 : 0; // check if both strings ended, if not they're not equal
}

int string_compare_n(const char *s1, const char *s2, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (s1[i] != s2[i])
        {
            return 1; // strings are different
        }
    }
    return 0;
}
/**
 * Substring.
 */
void substring(char s[], char sub[], int p, int l)
{
    int c = 0;
    while (c < l && s[p + c] != '\0')
    {
        sub[c] = s[p + c];
        c++;
    }
    sub[c] = '\0';
}

/**
 * String copy.
 */
void strcpy(char *src, char *dest)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}

int string_len(const char *s)
{
    int length = 0;
    while (*s != '\0')
    {
        length++;
        s++;
    }
    return length;
}

void int_str(int num, char *str)
{
    int i = 0;
    if (num == 0)
    {
        str[i++] = '0';
    }
    else
    {
        while (num > 0)
        {
            str[i++] = (num % 10) + '0';
            num /= 10;
        }
    }
    str[i] = '\0'; // Null-terminate the string

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int str_int(const char *str)
{
    int result = 0;
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}