#include "./lib/screen.h"
#include "data/imagedata.h"
#include "data/videodata.h"
#include "lib/framebf.h"

// frame resolution to display silde
const int frame_width = MAX_DISPLAY_WIDTH;
const int frame_height = 730;

// Array of all bitmaps for convenience. 
const unsigned long* image_epd_bitmap[] = {
	epd_bitmap_image
};

const unsigned int image_width[] = {
    1920
};

const unsigned int image_height[] = {
    1080
};

const unsigned long* frame_epd_bitmap[] = {
	epd_bitmap_frame[15]
};

const unsigned int video_width = 615;
const unsigned int video_height = 480;

int media_pointer = 0; // default start with image no.0; media_pointer = -1 means displaying video
int image_x = 0;
int image_y = 0;

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
void change_display(char c) {
    
    if (c == 's') {
        if (media_pointer >= 0) {
            image_y -= SCROLL_STEP;
            if (image_y < frame_height - image_height[media_pointer])
                image_y = frame_height - image_height[media_pointer];
            printImage(image_x, image_y, media_pointer);
        }   
    }
    else if (c == 'w') {
        if (media_pointer >= 0) {
            image_y += SCROLL_STEP;
            if (image_y > 0)
                image_y = 0;
            printImage(image_x, image_y, media_pointer);
        }    
    }
    else if (c == 'r') {
        if (media_pointer == -1) 
            displayVideo();
    }
}



void displayVideo() {
    int frame_pointer = 0;
                    
    while (frame_pointer < frame_arr_len) {
        int i = 0;
        for (int y = 0; y < video_height; y++)
            for (int x = 0; x < video_width; x++) {
                if (x >= 0 && y >= 0 && x < frame_width && y < frame_height)
                    drawPixelARGB32(x, y, frame_epd_bitmap[frame_pointer][i]);
                i++;
            }
        frame_pointer++;
        wait_msec(70);
    }
}

void wait_msec(unsigned int n) {
    register unsigned long f, t, r, expiredTime;
    // Get the current counter frequency (Hz)
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate expire value for counter
    expiredTime = t + ( (f/1000)*n )/1000;
    do {
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    } while(r < expiredTime);
}