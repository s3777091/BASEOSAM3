// ----------------------------------- framebf.c -------------------------------------
#include "./lib/framebf.h"


//Screen info
unsigned int width, height, pitch;

/* Frame buffer address
* (declare as pointer of unsigned char to access each byte) */
unsigned char *fb;

/**
* Set physical screen resolution
*/
void physical_framebf_init(int w, int h)
{
	mbox[0] = 35 * 4; // Length of message in bytes
	mbox[1] = MBOX_REQUEST;

	mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
	mbox[3] = 8;				 // Value size in bytes
	mbox[4] = 0;				 // REQUEST CODE = 0
	mbox[5] = w;				 // Value(width)
	mbox[6] = h;				 // Value(height)

	mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
	mbox[8] = 8;
	mbox[9] = 0;
	mbox[10] = 0;
	mbox[11] = 0;

	mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
	mbox[13] = 8;
	mbox[14] = 0;
	mbox[15] = 0; // x offset
	mbox[16] = 0; // y offset

	mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth
	mbox[18] = 4;
	mbox[19] = 0;
	mbox[20] = COLOR_DEPTH; //Bits per pixel

	mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
	mbox[22] = 4;
	mbox[23] = 0;
	mbox[24] = PIXEL_ORDER;

	mbox[25] = MBOX_TAG_GETFB; //Get frame buffer
	mbox[26] = 8;
	mbox[27] = 0;
	mbox[28] = 16; //alignment in 16 bytes
	mbox[29] = 0;  //will return Frame Buffer size in bytes

	mbox[30] = MBOX_TAG_GETPITCH; //Get pitch
	mbox[31] = 4;
	mbox[32] = 0;
	mbox[33] = 0; //Will get pitch value here

	mbox[34] = MBOX_TAG_LAST;

	// Call Mailbox
	if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ?
		&& mbox[20] == COLOR_DEPTH			//got correct color depth ?
		&& mbox[24] == PIXEL_ORDER			//got correct pixel order ?
		&& mbox[28] != 0					//got a valid address for frame buffer ?
	)
	{

		/* Convert GPU address to ARM address (clear higher address bits)
	 * Frame Buffer is located in RAM memory, which VideoCore MMU
	 * maps it to bus address space starting at 0xC0000000.
	 * Software accessing RAM directly use physical addresses
	 * (based at 0x00000000)
	 */
		mbox[28] &= 0x3FFFFFFF;

		// Access frame buffer as 1 byte per each address
		fb = (unsigned char *)((unsigned long)mbox[28]);
		uart_puts("Got allocated Frame Buffer at RAM physical address: ");
		uart_hex(mbox[28]);
		uart_puts("\n");

		uart_puts("Frame Buffer Size (bytes): ");
		uart_dec(mbox[29]);
		uart_puts("\n");

		width = mbox[5];  // Actual physical width
		height = mbox[6]; // Actual physical height
		pitch = mbox[33]; // Number of bytes per line
	}
	else
	{
		uart_puts("Unable to get a frame buffer with provided setting\n");
	}
}

/**
* Set virtual screen resolution
*/
void virtual_framebf_init(int w, int h)
{
	mbox[0] = 35 * 4; // Length of message in bytes
	mbox[1] = MBOX_REQUEST;

	mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
	mbox[3] = 8;				 // Value size in bytes
	mbox[4] = 0;				 // REQUEST CODE = 0
	mbox[5] = 0;				 // Value(width)
	mbox[6] = 0;				 // Value(height)

	mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
	mbox[8] = 8;
	mbox[9] = 0;
	mbox[10] = w;
	mbox[11] = h;

	mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
	mbox[13] = 8;
	mbox[14] = 0;
	mbox[15] = 0; // x offset
	mbox[16] = 0; // y offset

	mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth
	mbox[18] = 4;
	mbox[19] = 0;
	mbox[20] = COLOR_DEPTH; //Bits per pixel

	mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
	mbox[22] = 4;
	mbox[23] = 0;
	mbox[24] = PIXEL_ORDER;

	mbox[25] = MBOX_TAG_GETFB; //Get frame buffer
	mbox[26] = 8;
	mbox[27] = 0;
	mbox[28] = 16; //alignment in 16 bytes
	mbox[29] = 0;  //will return Frame Buffer size in bytes

	mbox[30] = MBOX_TAG_GETPITCH; //Get pitch
	mbox[31] = 4;
	mbox[32] = 0;
	mbox[33] = 0; //Will get pitch value here

	mbox[34] = MBOX_TAG_LAST;

	// Call Mailbox
	if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ?
		&& mbox[20] == COLOR_DEPTH			//got correct color depth ?
		&& mbox[24] == PIXEL_ORDER			//got correct pixel order ?
		&& mbox[28] != 0					//got a valid address for frame buffer ?
	)
	{

		/* Convert GPU address to ARM address (clear higher address bits)
	 * Frame Buffer is located in RAM memory, which VideoCore MMU
	 * maps it to bus address space starting at 0xC0000000.
	 * Software accessing RAM directly use physical addresses
	 * (based at 0x00000000)
	 */
		mbox[28] &= 0x3FFFFFFF;

		// Access frame buffer as 1 byte per each address
		fb = (unsigned char *)((unsigned long)mbox[28]);
		uart_puts("Got allocated Frame Buffer at RAM physical address: ");
		uart_hex(mbox[28]);
		uart_puts("\n");

		uart_puts("Frame Buffer Size (bytes): ");
		uart_dec(mbox[29]);
		uart_puts("\n");

		width = mbox[5];  // Actual physical width
		height = mbox[6]; // Actual physical height
		pitch = mbox[33]; // Number of bytes per line
	}
	else
	{
		uart_puts("Unable to get a frame buffer with provided setting\n");
	}
}

/**
* Set screen to W x H 
*/
void framebf_init(int w, int h)
{
	mbox[0] = 35 * 4; // Length of message in bytes
	mbox[1] = MBOX_REQUEST;

	mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
	mbox[3] = 8;				 // Value size in bytes
	mbox[4] = 0;				 // REQUEST CODE = 0
	mbox[5] = w;				 // Value(width)
	mbox[6] = h;				 // Value(height)

	mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
	mbox[8] = 8;
	mbox[9] = 0;
	mbox[10] = w;
	mbox[11] = h;

	mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
	mbox[13] = 8;
	mbox[14] = 0;
	mbox[15] = 0; // x offset
	mbox[16] = 0; // y offset

	mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth
	mbox[18] = 4;
	mbox[19] = 0;
	mbox[20] = COLOR_DEPTH; //Bits per pixel

	mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
	mbox[22] = 4;
	mbox[23] = 0;
	mbox[24] = PIXEL_ORDER;

	mbox[25] = MBOX_TAG_GETFB; //Get frame buffer
	mbox[26] = 8;
	mbox[27] = 0;
	mbox[28] = 16; //alignment in 16 bytes
	mbox[29] = 0;  //will return Frame Buffer size in bytes

	mbox[30] = MBOX_TAG_GETPITCH; //Get pitch
	mbox[31] = 4;
	mbox[32] = 0;
	mbox[33] = 0; //Will get pitch value here

	mbox[34] = MBOX_TAG_LAST;

	// Call Mailbox
	if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ?
		&& mbox[20] == COLOR_DEPTH			//got correct color depth ?
		&& mbox[24] == PIXEL_ORDER			//got correct pixel order ?
		&& mbox[28] != 0					//got a valid address for frame buffer ?
	)
	{

		/* Convert GPU address to ARM address (clear higher address bits)
	 * Frame Buffer is located in RAM memory, which VideoCore MMU
	 * maps it to bus address space starting at 0xC0000000.
	 * Software accessing RAM directly use physical addresses
	 * (based at 0x00000000)
	 */
		mbox[28] &= 0x3FFFFFFF;

		// Access frame buffer as 1 byte per each address
		fb = (unsigned char *)((unsigned long)mbox[28]);
		set_text_color("red");
		uart_puts("\nFrame Buffer allocated at: ");
		uart_hex(mbox[28]);
		uart_puts("\nFrame Buffer Size: ");
		uart_dec(mbox[29]);
		uart_puts(" bytes\n");

		width = mbox[5];  // Actual physical width
		height = mbox[6]; // Actual physical height
		pitch = mbox[33]; // Number of bytes per line
	}
	else
	{
		uart_puts("Unable to get a frame buffer with provided setting\n");
	}
}

void drawPixel(int x, int y, unsigned char attr)
{
	int offs = (y * pitch) + (x * 4);
	*((unsigned int *)(fb + offs)) = vgapal[attr & 0x0f];
}

void drawPixelARGB32(int x, int y, unsigned int attr)
{
	int offs = (y * pitch) + (4 * x);
	*((unsigned int *)(fb + offs)) = attr;
}

void drawRect(int x1, int y1, int x2, int y2, unsigned int attr, int fill)
{
	for (int y = y1; y <= y2; y++)
		for (int x = x1; x <= x2; x++)
		{
			if ((x == x1 || x == x2) || (y == y1 || y == y2))
				drawPixel(x, y, attr);
			else if (fill)
				drawPixel(x, y, attr);
		}
}

void drawLine(int x1, int y1, int x2, int y2, unsigned char attr)
{
	int dx, dy, p, x, y;

	dx = x2 - x1;
	dy = y2 - y1;
	x = x1;
	y = y1;
	p = 2 * dy - dx;

	while (x < x2)
	{
		if (p >= 0)
		{
			drawPixel(x, y, attr);
			y++;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			drawPixel(x, y, attr);
			p = p + 2 * dy;
		}
		x++;
	}
}

void drawCircle(int x0, int y0, int radius, unsigned char attr, int fill)
{
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		if (fill)
		{
			drawLine(x0 - y, y0 + x, x0 + y, y0 + x, (attr & 0xf0) >> 4);
			drawLine(x0 - x, y0 + y, x0 + x, y0 + y, (attr & 0xf0) >> 4);
			drawLine(x0 - x, y0 - y, x0 + x, y0 - y, (attr & 0xf0) >> 4);
			drawLine(x0 - y, y0 - x, x0 + y, y0 - x, (attr & 0xf0) >> 4);
		}
		drawPixel(x0 - y, y0 + x, attr);
		drawPixel(x0 + y, y0 + x, attr);
		drawPixel(x0 - x, y0 + y, attr);
		drawPixel(x0 + x, y0 + y, attr);
		drawPixel(x0 - x, y0 - y, attr);
		drawPixel(x0 + x, y0 - y, attr);
		drawPixel(x0 - y, y0 - x, attr);
		drawPixel(x0 + y, y0 - x, attr);

		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}

		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}

void clearScreen(int color)
{
	drawRect(0, 0, 1024, 768, color, 1);
}

void drawImage(unsigned int image[], int x, int y, int w, int h)
{
	int count = 0;

	while (y < h)
	{
		while (x < w)
		{
			int offs = (y * pitch) + (x * 4); //print array
			*((unsigned int *)(fb + offs)) = image[count];

			x++;
			count++;
		}
		y++;
		x = 0;
	}
}

void drawChar(unsigned char ch, int x, int y, unsigned char attr)
{
	unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

	for (int i = 0; i < FONT_HEIGHT; i++)
	{
		for (int j = 0; j < FONT_WIDTH; j++)
		{
			unsigned char mask = 1 << j;
			unsigned char col = (*glyph & mask) ? attr & 0x0f : (attr & 0xf0) >> 4;

			drawPixel(x + j, y + i, col);
		}
		glyph += FONT_BPL;
	}
}

void drawString(int x, int y, char *s, unsigned char attr)
{
	while (*s)
	{
		if (*s == '\r')
		{
			x = 0;
		}
		else if (*s == '\n')
		{
			x = 0;
			y += FONT_HEIGHT;
		}
		else
		{
			drawChar(*s, x, y, attr);
			x += FONT_WIDTH;
		}
		s++;
	}
}

void drawSquare(int x, int y, int size, unsigned char attr) {
    drawLine(x, y, x + size, y, attr);           // Top side
    drawLine(x, y, x, y + size, attr);           // Left side
    drawLine(x + size, y, x + size, y + size, attr); // Right side
    drawLine(x, y + size, x + size, y + size, attr); // Bottom side
}

void drawOnScreen()
{
	int v_align = 120;
	int h_align = 200;

	drawString(SCR_WIDTH / 2 - 70, h_align, "Team VIP PRO", 0xd);
	drawLine(SCR_WIDTH / 2 - 80, h_align + 20, SCR_WIDTH / 2 + 80, 220, 0x0f);

	drawString(SCR_WIDTH / 2 - v_align, h_align + 50, "Group 15 - Authors:", 0xa);

	// Authors
	drawString(SCR_WIDTH / 2 - v_align, h_align + 100, "Huynh Dac Tan Dat", 0xa);
	drawString(SCR_WIDTH / 2 - v_align + 175, h_align + 100, "$ s3777091 $", 0x05);

	drawString(SCR_WIDTH / 2 - v_align, h_align + 150, "Duong Minh Nam", 0xa);
	drawString(SCR_WIDTH / 2 - v_align  + 175, h_align + 150, "$ s3878202 $", 0x05);

	drawString(SCR_WIDTH / 2 - v_align, h_align + 200, "Lai Phuc Anh", 0xa);
	drawString(SCR_WIDTH / 2 - v_align + 175, h_align + 200, "$ s3878693 $", 0x05);

	drawString(SCR_WIDTH / 2 - v_align, h_align + 250, "Phan Vinh Loc", 0xa);
	drawString(SCR_WIDTH / 2 - v_align + 175, h_align + 250, "$ s3938497 $", 0x05);

	drawCircle(1024, 700, 250, 0x09, 0);
	drawCircle(0, 0, 30, 0x69, 1);
}

void drawImageTiled(unsigned int image[32*32], int x, int y, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int srcX = j % 32;
            int srcY = i % 32;
            int srcIndex = srcY * 32 + srcX; // Calculate the index for the 1D array
            int offs = ((y + i) * pitch) + ((x + j) * 4); // Calculate offset
            *((unsigned int *)(fb + offs)) = image[srcIndex]; // Set pixel color
        }
    }
}

void showGameMenu() {
    int square_size = 500;
    int square_x = (SCR_WIDTH - square_size) / 2;
    int square_y = (SCR_HEIGHT - square_size) / 2;
    drawSquare(square_x, square_y, square_size, 0x0F); // Draw a big square

    int text_x = SCR_WIDTH / 2 - 150;
    int text_y = SCR_HEIGHT / 2 - 100;

    // Draw the welcome message
    drawString(text_x, text_y, "WELCOME TO SNAKE WORM", 0xd);
    text_y += 40;
    drawString(text_x - 50, text_y, "Press any key to start the game", 0xa);
    text_y += 60;

    // Draw the instructions
    drawString(text_x - 50, text_y, "Instructions:", 0xd);
    text_y += 40;
    drawString(text_x, text_y, "Using 's' to go down", 0xa);
    text_y += 30;
    drawString(text_x, text_y, "Using 'w' to go up", 0xa);
    text_y += 30;
    drawString(text_x, text_y, "Using 'd' to go right", 0xa);
    text_y += 30;
    drawString(text_x, text_y, "Using 'a' to go left", 0xa);
    text_y += 30;
    drawString(text_x, text_y, "Press 'Q' to quit", 0xa);
    text_y += 30;
    drawString(text_x, text_y, "Press 'R' to reload map", 0xa);

	drawCircle(1024, 700, 250, 0x09, 0);
	drawCircle(0, 0, 30, 0x69, 1);

}

