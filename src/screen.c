#include "./lib/screen.h"
#include "data/imagedata.h"
#include "lib/framebf.h"

// frame resolution to display screen
const int frame_width = MAX_DISPLAY_WIDTH;
const int frame_height = 830;

// Array of all bitmaps for convenience.
const unsigned long* image_epd_bitmap[] = {
    epd_bitmap_image
};

const unsigned int image_width[] = {
    1024
};

const unsigned int image_height[] = {
    830
};

int media_pointer = 0; 
int image_x = 0;
int image_y = 0;

//Function to display image
void printImage(int image_x, int image_y, int media_pointer) {
    int i = 0;
    int w = image_width[media_pointer];
    int h = image_height[media_pointer];  
    
    // print image
    for (int y = image_y; y < (image_y + h); y++)
        for (int x = image_x; x < (image_x + w); x++) {
            if (x >= 0 && y >= 0 && x < frame_width && y < frame_height)
                drawPixelARGB32(x, y, image_epd_bitmap[media_pointer][i]);
            i++;
        }
}
//Scrollable function for oversize image
void change_display(char c) {
    if (media_pointer >= 0) {
        if (c == 's') {
            image_y -= SCROLL_STEP;
            if (image_y < frame_height - image_height[media_pointer])
                image_y = frame_height - image_height[media_pointer];
        } else if (c == 'w') {
            image_y += SCROLL_STEP;
            if (image_y > 0)
                image_y = 0;
        }
        printImage(image_x, image_y, media_pointer);
    }
}